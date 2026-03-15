/*
 * Vyom Programming Language Interpreter
 * Version 1.0 — Clean Rewrite
 *
 * Architecture:
 *   - Source loading & line table
 *   - Value / Variable system
 *   - Scope stack
 *   - Expression evaluator  (precedence-climbing, no strtok)
 *   - Statement executor
 *   - Function registry
 *   - Builtins: print, len, range, exit
 *   - Arrays: 1D and 2D, numeric and string
 */

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 *  CONSTANTS
 * ================================================================ */

#define VYOM_VERSION "1.0"
#define MAX_LINES 4096
#define MAX_LINE 1024
#define MAX_VARS 512
#define MAX_FUNCS 256
#define MAX_CALL_DEPTH 128
#define MAX_ARGS 16
#define MAX_ARRAY_TOTAL 65536
#define MAX_STR 512

/* ================================================================
 *  FORWARD DECLARATIONS
 * ================================================================ */

typedef struct Value Value;
typedef struct Var Var;
typedef struct Scope Scope;
typedef struct Func Func;
typedef struct Line Line;

static Value eval_expr(const char *src);
static void exec_stmt(int line_idx);
static void exec_block(int start, int end);

/* ================================================================
 *  SOURCE LINE TABLE
 * ================================================================ */

struct Line
{
    char text[MAX_LINE]; /* trimmed text (no leading whitespace) */
    int indent;          /* leading space count of original line */
    int lineno;          /* 1-based source line number           */
};

static Line g_lines[MAX_LINES];
static int g_nlines = 0;

/* current execution line number (for error messages) */
static int g_cur_lineno = 0;

/* source filename */
static char g_filename[512] = "";

/* ================================================================
 *  ERROR HANDLING
 * ================================================================ */

/* Forward declare so cleanup can call it */
static void cleanup_all(void);

_Noreturn static void
vyom_error(const char *fmt, ...)
{
    va_list ap;
    fprintf(stderr, "Error (line %d): ", g_cur_lineno);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    cleanup_all();
    exit(1);
}

/* ================================================================
 *  VALUE SYSTEM
 * ================================================================ */

typedef enum
{
    VT_NUM,
    VT_STR,
    VT_ARR
} ValType;

/*
 * Arrays are heap-allocated and reference-counted via a wrapper so
 * that get/set operations never accidentally free live data.
 */
typedef struct ArrayData
{
    int refcnt;
    ValType elem_type;
    int size; /* total elements  */
    int rows; /* 0 → 1-D array   */
    int cols; /* 0 → 1-D array   */
    union
    {
        double *nums; /* elem_type == VT_NUM */
        char **strs;  /* elem_type == VT_STR */
    } data;
} ArrayData;

struct Value
{
    ValType type;
    double num;
    char str[MAX_STR];
    ArrayData *arr; /* non-NULL only when type == VT_ARR */
};

/* ------------------------------------------------------------------ */
/* Array reference counting                                            */
/* ------------------------------------------------------------------ */

static ArrayData *
arr_alloc(ValType elem_type, int rows, int cols)
{
    ArrayData *a = calloc(1, sizeof(ArrayData));
    if (!a)
        vyom_error("out of memory");

    int total = (cols > 0) ? rows * cols : rows;
    if (total <= 0 || total > MAX_ARRAY_TOTAL)
        vyom_error("array size out of range (%d)", total);

    a->refcnt = 1;
    a->elem_type = elem_type;
    a->size = total;
    a->rows = (cols > 0) ? rows : 0;
    a->cols = (cols > 0) ? cols : 0;

    if (elem_type == VT_NUM)
    {
        a->data.nums = calloc(total, sizeof(double));
        if (!a->data.nums)
            vyom_error("out of memory");
    }
    else
    {
        a->data.strs = calloc(total, sizeof(char *));
        if (!a->data.strs)
            vyom_error("out of memory");
    }
    return a;
}

static void
arr_decref(ArrayData *a)
{
    if (!a)
        return;
    if (--a->refcnt > 0)
        return;

    if (a->elem_type == VT_STR && a->data.strs)
    {
        for (int i = 0; i < a->size; i++)
            free(a->data.strs[i]);
        free(a->data.strs);
    }
    else if (a->elem_type == VT_NUM)
    {
        free(a->data.nums);
    }
    free(a);
}

static void
arr_incref(ArrayData *a)
{
    if (a)
        a->refcnt++;
}

/* ================================================================
 *  VARIABLE STORAGE
 * ================================================================ */

struct Var
{
    char name[64];
    Value val;
    int is_const;
    int is_typed; /* declared with int/double/string */
};

/* ------------------------------------------------------------------ */
/* Scope stack                                                          */
/* ------------------------------------------------------------------ */

struct Scope
{
    Var vars[MAX_VARS];
    int nv;
    int is_func; /* top of a function call frame? */
    int is_loop; /* is this a loop block?         */
};

static Scope g_scopes[MAX_CALL_DEPTH];
static int g_ndepth = 0; /* number of active scopes */

static int g_loop_depth = 0; /* count of active loop scopes */

static void
scope_push(int is_func, int is_loop)
{
    if (g_ndepth >= MAX_CALL_DEPTH)
        vyom_error("call stack overflow");
    g_scopes[g_ndepth].nv = 0;
    g_scopes[g_ndepth].is_func = is_func;
    g_scopes[g_ndepth].is_loop = is_loop;
    g_ndepth++;
    if (is_loop)
        g_loop_depth++;
}

static void
scope_pop(void)
{
    if (g_ndepth == 0)
        vyom_error("internal: scope underflow");
    Scope *s = &g_scopes[g_ndepth - 1];
    if (s->is_loop)
        g_loop_depth--;
    for (int i = 0; i < s->nv; i++)
        arr_decref(s->vars[i].val.arr);
    s->nv = 0;
    g_ndepth--;
}

/* Globals live in g_scopes[0] (pushed at startup, never popped) */
static Scope *
global_scope(void)
{
    return &g_scopes[0];
}

/* current function-local scope (top of stack) */
static Scope *
local_scope(void)
{
    return &g_scopes[g_ndepth - 1];
}

/* ------------------------------------------------------------------ */
/* Variable lookup (inner-to-outer)                                    */
/* ------------------------------------------------------------------ */

/*
 * Returns pointer into the actual Var storage so callers can modify in
 * place.  Returns NULL if not found.
 */
static Var *
var_find(const char *name)
{
    /* Walk scopes from innermost to outermost */
    for (int d = g_ndepth - 1; d >= 0; d--)
    {
        Scope *s = &g_scopes[d];
        for (int i = s->nv - 1; i >= 0; i--)
            if (!strcmp(s->vars[i].name, name))
                return &s->vars[i];
        /* Don't cross function boundaries for locals */
        if (d > 0 && g_scopes[d].is_func)
            break;
    }
    /* always check globals */
    Scope *gs = global_scope();
    for (int i = gs->nv - 1; i >= 0; i--)
        if (!strcmp(gs->vars[i].name, name))
            return &gs->vars[i];
    return NULL;
}

/*
 * Get a copy of a variable's value.  Increments array refcnt.
 */
static int
var_get(const char *name, Value *out)
{
    /* built-in constants */
    if (!strcmp(name, "__version__"))
    {
        out->type = VT_STR;
        strncpy(out->str, VYOM_VERSION, MAX_STR - 1);
        out->arr = NULL;
        return 1;
    }
    if (!strcmp(name, "__file__"))
    {
        out->type = VT_STR;
        strncpy(out->str, g_filename, MAX_STR - 1);
        out->arr = NULL;
        return 1;
    }

    Var *v = var_find(name);
    if (!v)
        return 0;
    *out = v->val;
    arr_incref(out->arr);
    return 1;
}

/*
 * Declare/assign a variable.
 *
 * Rules:
 *   1. const variables cannot be reassigned.
 *   2. typed variables cannot change type.
 *   3. A typed declaration inside a function ALWAYS creates a new local
 *      (even if a global exists with the same name).
 *   4. An untyped assignment inside a function updates an existing var
 *      (global or outer local) if found; otherwise creates a new local.
 *   5. An untyped assignment at global scope updates or creates global.
 */
static void
var_set(const char *name, Value val, int is_typed, int is_const)
{
    if (!strcmp(name, "__version__") || !strcmp(name, "__file__"))
        vyom_error("cannot reassign built-in constant '%s'", name);

    /* Typed declaration: always create/replace in current scope */
    if (is_typed)
    {
        Scope *s = local_scope();
        /* Check same-scope shadowing legality: if a global with same name
           exists, that's fine for a typed local declaration. */
        for (int i = 0; i < s->nv; i++)
        {
            if (!strcmp(s->vars[i].name, name))
            {
                if (s->vars[i].is_const)
                    vyom_error("cannot reassign const variable '%s'", name);
                if (s->vars[i].is_typed && s->vars[i].val.type != val.type)
                    vyom_error("type mismatch: cannot change type of '%s'", name);
                arr_decref(s->vars[i].val.arr);
                s->vars[i].val = val;
                s->vars[i].is_const = is_const;
                arr_incref(val.arr);
                return;
            }
        }
        /* New variable in current scope */
        if (s->nv >= MAX_VARS)
            vyom_error("too many variables in scope");
        Var *nv = &s->vars[s->nv++];
        strncpy(nv->name, name, 63);
        nv->val = val;
        nv->is_typed = 1;
        nv->is_const = is_const;
        arr_incref(val.arr);
        return;
    }

    /* Untyped: find existing variable anywhere in scope chain */
    Var *existing = var_find(name);
    if (existing)
    {
        if (existing->is_const)
            vyom_error("cannot reassign const variable '%s'", name);
        if (existing->is_typed && existing->val.type != val.type)
            vyom_error("type mismatch: cannot change type of '%s'", name);
        arr_decref(existing->val.arr);
        existing->val = val;
        arr_incref(val.arr);
        return;
    }

    /* New variable in current scope */
    Scope *s = local_scope();
    if (s->nv >= MAX_VARS)
        vyom_error("too many variables in scope");
    Var *nv = &s->vars[s->nv++];
    strncpy(nv->name, name, 63);
    nv->val = val;
    nv->is_typed = 0;
    nv->is_const = is_const;
    arr_incref(val.arr);
}

/* ================================================================
 *  FUNCTION REGISTRY
 * ================================================================ */

struct Func
{
    char name[64];
    char params[MAX_ARGS][64];
    int nparams;
    int body_start; /* index into g_lines[] */
    int body_end;
};

static Func g_funcs[MAX_FUNCS];
static int g_nfuncs = 0;

static Func *
func_find(const char *name)
{
    for (int i = 0; i < g_nfuncs; i++)
        if (!strcmp(g_funcs[i].name, name))
            return &g_funcs[i];
    return NULL;
}

/* ================================================================
 *  CONTROL FLOW FLAGS
 * ================================================================ */

static int g_ret_flag = 0;
static Value g_ret_val = {0};
static int g_brk_flag = 0;
static int g_cont_flag = 0;
static int g_func_depth = 0; /* counts active function call frames */
#define MAX_FUNC_DEPTH 64

/* ================================================================
 *  BLOCK HELPERS
 * ================================================================ */

/*
 * Return the index of the first line that is NOT part of the block
 * starting at line `i`.  A block is all lines strictly more indented
 * than program[i].
 */
static int
block_end(int i)
{
    int base = g_lines[i].indent;
    int j = i + 1;
    while (j < g_nlines && g_lines[j].indent > base)
        j++;
    return j;
}

/* ================================================================
 *  TOKENIZER HELPERS  (no strtok — all re-entrant)
 * ================================================================ */

static const char *
skip_ws(const char *p)
{
    while (*p == ' ' || *p == '\t')
        p++;
    return p;
}

/*
 * Copy trimmed version of [s, e) into dst (dst must be >= e-s+1 bytes).
 */
static void
trimcpy(char *dst, const char *s, const char *e)
{
    while (s < e && isspace((unsigned char)*s))
        s++;
    while (e > s && isspace((unsigned char)*(e - 1)))
        e--;
    int n = (int)(e - s);
    if (n < 0)
        n = 0;
    memcpy(dst, s, n);
    dst[n] = '\0';
}

/*
 * Scan forward from `p` counting brackets/parens/strings.
 * Returns pointer to the first unbalanced `)` or `]` at depth 0,
 * or pointer to next unquoted `,` at depth 0 if sep=','.
 * Returns NULL if end of string reached without finding it.
 */
static const char *
scan_to(const char *p, char target)
{
    int depth = 0;
    int in_str = 0;
    while (*p)
    {
        if (*p == '"')
        {
            in_str = !in_str;
            p++;
            continue;
        }
        if (in_str)
        {
            p++;
            continue;
        }
        if (*p == '(' || *p == '[')
        {
            depth++;
            p++;
            continue;
        }
        if (*p == ')' || *p == ']')
        {
            if (depth == 0)
                return p;
            depth--;
            p++;
            continue;
        }
        if (depth == 0 && *p == target)
            return p;
        p++;
    }
    return NULL;
}

/*
 * Split `src` by unquoted commas at depth 0 into argv[].
 * Returns argument count.  Each argv[i] is null-terminated trimmed string.
 * Caller provides storage: char argv[MAX_ARGS][MAX_LINE].
 */
static int
split_args(const char *src, char argv[][MAX_LINE], int maxargs)
{
    int n = 0;
    const char *p = src;

    while (*p)
    {
        p = skip_ws(p);
        if (!*p)
            break;

        /* find next comma at depth 0 */
        int depth = 0;
        int in_str = 0;
        const char *start = p;

        while (*p)
        {
            if (*p == '"')
            {
                in_str = !in_str;
                p++;
                continue;
            }
            if (in_str)
            {
                p++;
                continue;
            }
            if (*p == '(' || *p == '[')
            {
                depth++;
                p++;
                continue;
            }
            if (*p == ')' || *p == ']')
            {
                depth--;
                p++;
                continue;
            }
            if (depth == 0 && *p == ',')
                break;
            p++;
        }

        if (n >= maxargs)
            vyom_error("too many arguments (max %d)", maxargs);
        trimcpy(argv[n++], start, p);
        if (*p == ',')
            p++;
    }
    return n;
}

/* ================================================================
 *  EXPRESSION EVALUATOR
 * ================================================================ */

/*
 * Precedence levels (higher = binds tighter):
 *   0 : or
 *   1 : and
 *   2 : not
 *   3 : == != < > <= >=
 *   4 : + -
 *   5 : * / // %
 *   6 : unary -  (handled in eval_primary)
 *   7 : primary (literals, variables, calls, indexing)
 */

static Value eval_at_prec(const char *src, int prec);

/* ------------------------------------------------------------------ */
/* Primary: literals, identifiers, function calls, indexing, (...)    */
/* ------------------------------------------------------------------ */

static Value
make_num(double n)
{
    Value v = {0};
    v.type = VT_NUM;
    v.num = n;
    return v;
}

static Value
make_str(const char *s)
{
    Value v = {0};
    v.type = VT_STR;
    strncpy(v.str, s, MAX_STR - 1);
    return v;
}

/* Helper: evaluate a string that is exactly one balanced expression.
   Used when we've already isolated the expression text.              */
static Value
eval_expr(const char *src)
{
    return eval_at_prec(src, 0);
}

/* Return 1 if `name` looks like a valid identifier */
static int
is_ident_char(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

static Value
eval_primary(const char *src)
{
    const char *p = skip_ws(src);

    if (!*p)
        vyom_error("empty expression");

    /* ---- Parenthesised expression ---- */
    if (*p == '(')
    {
        /* find matching ')' */
        int depth = 1;
        int in_str = 0;
        const char *q = p + 1;
        while (*q && depth)
        {
            if (*q == '"')
            {
                in_str = !in_str;
                q++;
                continue;
            }
            if (in_str)
            {
                q++;
                continue;
            }
            if (*q == '(')
                depth++;
            else if (*q == ')')
                depth--;
            if (depth)
                q++;
        }
        if (depth)
            vyom_error("unmatched parentheses");
        char inner[MAX_LINE];
        trimcpy(inner, p + 1, q);
        return eval_expr(inner);
    }

    /* ---- Unary not ---- */
    if (!strncmp(p, "not", 3) && !is_ident_char(p[3]))
    {
        Value v = eval_at_prec(p + 3, 6);
        return make_num(v.num == 0.0 ? 1.0 : 0.0);
    }

    /* ---- Unary minus ---- */
    if (*p == '-' && (p[1] == '(' || isdigit((unsigned char)p[1]) ||
                      is_ident_char(p[1])))
    {
        Value v = eval_at_prec(p + 1, 6);
        if (v.type != VT_NUM)
            vyom_error("unary minus on non-numeric value");
        return make_num(-v.num);
    }

    /* ---- String literal ---- */
    if (*p == '"')
    {
        /* find closing unescaped '"' */
        const char *q = p + 1;
        char buf[MAX_STR];
        int bi = 0;
        while (*q && *q != '"')
        {
            if (*q == '\\')
            {
                q++;
                switch (*q)
                {
                case 'n':
                    buf[bi++] = '\n';
                    break;
                case 't':
                    buf[bi++] = '\t';
                    break;
                case '"':
                    buf[bi++] = '"';
                    break;
                case '\\':
                    buf[bi++] = '\\';
                    break;
                default:
                    buf[bi++] = '\\';
                    buf[bi++] = *q;
                    break;
                }
                q++;
            }
            else
            {
                if (bi >= MAX_STR - 1)
                    vyom_error("string literal too long");
                buf[bi++] = *q++;
            }
        }
        if (*q != '"')
            vyom_error("unterminated string literal");
        buf[bi] = '\0';
        return make_str(buf);
    }

    /* ---- Numeric literal ---- */
    if (isdigit((unsigned char)*p) ||
        (*p == '-' && isdigit((unsigned char)p[1])))
    {
        char *end;
        double n = strtod(p, &end);
        return make_num(n);
    }

    /* ---- Identifier, function call, array index ---- */
    if (is_ident_char(*p) || *p == '_')
    {
        /* extract identifier */
        const char *id_start = p;
        while (is_ident_char(*p))
            p++;
        char name[64];
        if (p - id_start >= 64)
            vyom_error("identifier too long");
        memcpy(name, id_start, p - id_start);
        name[p - id_start] = '\0';

        p = skip_ws(p);

        /* ---- len() builtin ---- */
        if (!strcmp(name, "len") && *p == '(')
        {
            /* find matching ')' */
            const char *rp = scan_to(p + 1, ')');
            if (!rp)
                vyom_error("unmatched parentheses in len()");
            char arg[MAX_LINE];
            trimcpy(arg, p + 1, rp);

            /* len(arr[i]) → column count for 2D */
            char *bracket = strchr(arg, '[');
            if (bracket)
            {
                char vname[64];
                trimcpy(vname, arg, bracket);
                Value av;
                if (!var_get(vname, &av))
                    vyom_error("undefined variable '%s'", vname);
                if (av.type != VT_ARR)
                    vyom_error("len() requires array");
                if (av.arr->cols == 0)
                    vyom_error("len(arr[i]) requires 2D array");
                double r = (double)av.arr->cols;
                arr_decref(av.arr);
                return make_num(r);
            }

            Value av;
            if (!var_get(arg, &av))
                vyom_error("undefined variable '%s'", arg);
            if (av.type == VT_ARR)
            {
                double r = (av.arr->cols > 0)
                               ? (double)av.arr->rows
                               : (double)av.arr->size;
                arr_decref(av.arr);
                return make_num(r);
            }
            if (av.type == VT_STR)
            {
                return make_num((double)strlen(av.str));
            }
            vyom_error("len() requires array or string");
        }

        /* ---- ascii() builtin ---- */
        if (!strcmp(name, "ascii") && *p == '(')
        {
            const char *rp = scan_to(p + 1, ')');
            if (!rp)
                vyom_error("unmatched parentheses in ascii()");
            char arg[MAX_LINE];
            trimcpy(arg, p + 1, rp);
            Value av = eval_expr(arg);
            if (av.type != VT_STR)
                vyom_error("ascii() requires a string argument");
            if (strlen(av.str) != 1)
                vyom_error("ascii() requires a single character");
            return make_num((double)(unsigned char)av.str[0]);
        }

        /* ---- char() builtin ---- */
        if (!strcmp(name, "char") && *p == '(')
        {
            const char *rp = scan_to(p + 1, ')');
            if (!rp)
                vyom_error("unmatched parentheses in char()");
            char arg[MAX_LINE];
            trimcpy(arg, p + 1, rp);
            Value av = eval_expr(arg);
            if (av.type != VT_NUM)
                vyom_error("char() requires a numeric argument");
            int code = (int)av.num;
            if (code < 0 || code > 255)
                vyom_error("char() argument out of range");
            char buf[2] = {(char)code, '\0'};
            return make_str(buf);
        }

        /* ---- input() builtin ---- */
        if (!strcmp(name, "input") && *p == '(')
        {
            const char *rp = scan_to(p + 1, ')');
            if (!rp)
                vyom_error("unmatched parentheses in input()");
            char ibuf[MAX_STR];
            if (fgets(ibuf, sizeof(ibuf), stdin))
            {
                int n = (int)strlen(ibuf);
                while (n > 0 && (ibuf[n - 1] == '\n' || ibuf[n - 1] == '\r'))
                    ibuf[--n] = '\0';
                return make_str(ibuf);
            }
            return make_str("");
        }

        /* ---- concat(a, b) builtin ---- */
        /* Joins two strings: concat("hello", "world") → "helloworld" */
        if (!strcmp(name, "concat") && *p == '(')
        {
            const char *rp = scan_to(p + 1, ')');
            if (!rp)
                vyom_error("unmatched parentheses in concat()");
            char args_buf[MAX_LINE];
            trimcpy(args_buf, p + 1, rp);
            char argv[MAX_ARGS][MAX_LINE];
            int argc = split_args(args_buf, argv, MAX_ARGS);
            if (argc != 2)
                vyom_error("concat() requires exactly 2 arguments");
            Value a = eval_expr(argv[0]);
            Value b = eval_expr(argv[1]);
            if (a.type != VT_STR)
                vyom_error("concat() argument 1 must be a string");
            if (b.type != VT_STR)
                vyom_error("concat() argument 2 must be a string");
            char result[MAX_STR];
            int alen = (int)strlen(a.str);
            int blen = (int)strlen(b.str);
            if (alen + blen >= MAX_STR)
                vyom_error("concat() result string too long");
            memcpy(result, a.str, alen);
            memcpy(result + alen, b.str, blen);
            result[alen + blen] = '\0';
            return make_str(result);
        }

        /* ---- Function call ---- */
        if (*p == '(')
        {
            Func *fn = func_find(name);
            if (!fn)
                vyom_error("undefined function '%s'", name);

            const char *rp = scan_to(p + 1, ')');
            if (!rp)
                vyom_error("unmatched parentheses calling '%s'", name);

            char args_str[MAX_LINE];
            trimcpy(args_str, p + 1, rp);

            char argv[MAX_ARGS][MAX_LINE];
            int argc = (*args_str) ? split_args(args_str, argv, MAX_ARGS) : 0;

            if (argc != fn->nparams)
                vyom_error("function '%s' expects %d args, got %d",
                           name, fn->nparams, argc);

            /* evaluate arguments before pushing scope */
            Value argvals[MAX_ARGS];
            for (int i = 0; i < argc; i++)
            {
                argvals[i] = eval_expr(argv[i]);
            }

            /* save flags */
            int old_ret = g_ret_flag;
            int old_brk = g_brk_flag;
            int old_cont = g_cont_flag;
            Value old_rval = g_ret_val;
            g_ret_flag = g_brk_flag = g_cont_flag = 0;

            if (g_func_depth >= MAX_FUNC_DEPTH)
                vyom_error("call stack overflow (max recursion depth %d)", MAX_FUNC_DEPTH);

            g_func_depth++;
            scope_push(1, 0);

            for (int i = 0; i < argc; i++)
            {
                /* parameters are untyped locals */
                var_set(fn->params[i], argvals[i], 0, 0);
                /* decref: var_set took ownership via incref */
                arr_decref(argvals[i].arr);
            }

            exec_block(fn->body_start, fn->body_end);

            scope_pop();
            g_func_depth--;

            Value result = g_ret_val;
            int got_return = g_ret_flag;

            g_ret_flag = old_ret;
            g_brk_flag = old_brk;
            g_cont_flag = old_cont;
            g_ret_val = old_rval;

            if (!got_return)
                vyom_error("function '%s' missing return statement", name);

            return result;
        }

        /* ---- Array / string indexing ---- */
        if (*p == '[')
        {
            Value base;
            if (!var_get(name, &base))
                vyom_error("undefined variable '%s'", name);

            /* first index */
            const char *rb = scan_to(p + 1, ']');
            if (!rb)
                vyom_error("unmatched '[' in index expression");
            char idx_expr[MAX_LINE];
            trimcpy(idx_expr, p + 1, rb);
            Value iv = eval_expr(idx_expr);
            if (iv.type != VT_NUM)
                vyom_error("array index must be numeric");
            int idx1 = (int)iv.num;
            if (iv.num != (double)idx1)
                vyom_error("array index must be integer");

            p = skip_ws(rb + 1);

            /* string indexing */
            if (base.type == VT_STR)
            {
                int slen = (int)strlen(base.str);
                if (idx1 < 0 || idx1 >= slen)
                    vyom_error("string index %d out of bounds (length %d)", idx1, slen);
                char ch[2] = {base.str[idx1], '\0'};
                return make_str(ch);
            }

            if (base.type != VT_ARR)
                vyom_error("indexing requires array or string");

            ArrayData *a = base.arr;

            /* 2-D array: expect second [j] */
            if (a->cols > 0)
            {
                if (*p != '[')
                    vyom_error("2D array requires two indices");
                const char *rb2 = scan_to(p + 1, ']');
                if (!rb2)
                    vyom_error("unmatched '[' in 2D index");
                char idx2_expr[MAX_LINE];
                trimcpy(idx2_expr, p + 1, rb2);
                Value iv2 = eval_expr(idx2_expr);
                if (iv2.type != VT_NUM)
                    vyom_error("array index must be numeric");
                int idx2 = (int)iv2.num;

                if (idx1 < 0 || idx1 >= a->rows)
                    vyom_error("2D array row index %d out of bounds (%d rows)", idx1, a->rows);
                if (idx2 < 0 || idx2 >= a->cols)
                    vyom_error("2D array col index %d out of bounds (%d cols)", idx2, a->cols);

                double val = a->data.nums[idx1 * a->cols + idx2];
                arr_decref(base.arr);
                return make_num(val);
            }

            /* 1-D array */
            if (idx1 < 0 || idx1 >= a->size)
                vyom_error("array index %d out of bounds (size %d)", idx1, a->size);

            Value result = {0};
            if (a->elem_type == VT_NUM)
            {
                result = make_num(a->data.nums[idx1]);
            }
            else
            {
                const char *s = a->data.strs[idx1] ? a->data.strs[idx1] : "";
                result = make_str(s);
            }
            arr_decref(base.arr);
            return result;
        }

        /* ---- Plain variable ---- */
        Value v;
        if (!var_get(name, &v))
            vyom_error("undefined variable '%s'", name);
        if (v.type == VT_ARR)
            vyom_error("cannot use array '%s' directly in expression (use index)", name);
        return v;
    }

    vyom_error("unexpected token in expression: %.20s", src);
    /* unreachable */
    return make_num(0);
}

/* ------------------------------------------------------------------ */
/* Binary operator scanning: find rightmost operator at given prec    */
/* level that is NOT inside brackets/strings.                         */
/*                                                                     */
/* We scan left-to-right for keyword operators (or, and) and          */
/* right-to-left for arithmetic to get left-associativity correctly.  */
/* ------------------------------------------------------------------ */

/*
 * Scan left-to-right for `or` / `and` keyword at depth 0.
 * Returns pointer to start of keyword token, or NULL.
 */
static const char *
find_keyword_op(const char *s, const char *kw, int kwlen)
{
    int depth = 0;
    int in_str = 0;
    const char *p = s;

    while (*p)
    {
        if (*p == '"')
        {
            in_str = !in_str;
            p++;
            continue;
        }
        if (in_str)
        {
            p++;
            continue;
        }
        if (*p == '(' || *p == '[')
        {
            depth++;
            p++;
            continue;
        }
        if (*p == ')' || *p == ']')
        {
            depth--;
            p++;
            continue;
        }

        if (depth == 0)
        {
            if (!strncmp(p, kw, kwlen) && !is_ident_char(p[kwlen]))
            {
                /* also verify it's not in the middle of an identifier */
                if (p == s || !is_ident_char(p[-1]))
                    return p;
            }
        }
        p++;
    }
    return NULL;
}

/*
 * Scan right-to-left for a binary arithmetic/comparison operator at
 * depth 0.  Returns pointer to the operator character, or NULL.
 * `ops` is a null-terminated list of single chars to look for.
 * `skip_eq_follow` = 1 means skip `==`, `!=`, `<=`, `>=`.
 */
typedef struct
{
    const char *pos;
    int len;
    char op1;
    char op2;
} OpMatch;

static OpMatch
find_binary_op_rtl(const char *s, int level)
{
    OpMatch none = {NULL, 0, 0, 0};
    int len = (int)strlen(s);
    int depth = 0;
    int in_str = 0;

    /* build reverse scan */
    for (int i = len - 1; i >= 0; i--)
    {
        char c = s[i];

        /* string tracking (very simplified — fine for valid source) */
        if (c == '"')
        {
            in_str = !in_str;
            continue;
        }
        if (in_str)
            continue;

        if (c == ')' || c == ']')
        {
            depth++;
            continue;
        }
        if (c == '(' || c == '[')
        {
            depth--;
            continue;
        }

        if (depth != 0)
            continue;

        /* Level 4: + and - (binary) */
        if (level == 4 && (c == '+' || c == '-'))
        {
            /* skip if this is a unary operator:
               preceded only by whitespace or another operator */
            int j = i - 1;
            while (j >= 0 && (s[j] == ' ' || s[j] == '\t'))
                j--;
            if (j < 0)
                continue; /* unary at start */
            char prev = s[j];
            if (prev == '+' || prev == '-' || prev == '*' || prev == '/' ||
                prev == '(' || prev == '[' || prev == ',' || prev == '%')
                continue;
            OpMatch m = {s + i, 1, c, 0};
            return m;
        }

        /* Level 5: * / // % */
        if (level == 5)
        {
            if (c == '*')
            {
                OpMatch m = {s + i, 1, '*', 0};
                return m;
            }
            if (c == '%')
            {
                OpMatch m = {s + i, 1, '%', 0};
                return m;
            }
            if (c == '/')
            {
                /* Check for // (integer division) */
                if (i > 0 && s[i - 1] == '/')
                {
                    OpMatch m = {s + i - 1, 2, '/', '/'};
                    return m;
                }
                /* plain / — but make sure we didn't just pass the second / of // */
                if (i + 1 < len && s[i + 1] == '/')
                    continue;
                OpMatch m = {s + i, 1, '/', 0};
                return m;
            }
        }

        /* Level 3: comparisons */
        if (level == 3)
        {
            if (i + 1 < len)
            {
                if ((c == '=' && s[i + 1] == '=') ||
                    (c == '!' && s[i + 1] == '=') ||
                    (c == '<' && s[i + 1] == '=') ||
                    (c == '>' && s[i + 1] == '='))
                {
                    OpMatch m = {s + i, 2, c, s[i + 1]};
                    return m;
                }
            }
            /* single-char < > — make sure next char isn't = */
            if ((c == '<' || c == '>') &&
                (i + 1 >= len || s[i + 1] != '='))
            {
                OpMatch m = {s + i, 1, c, 0};
                return m;
            }
        }
    }
    return none;
}

/* ------------------------------------------------------------------ */
/* Main precedence evaluator                                           */
/* ------------------------------------------------------------------ */

static Value
eval_at_prec(const char *raw, int prec)
{
    char src_buf[MAX_LINE];
    strncpy(src_buf, raw, MAX_LINE - 1);
    src_buf[MAX_LINE - 1] = '\0';
    const char *src = skip_ws(src_buf);

    /* -- Level 0: or -- */
    if (prec <= 0)
    {
        const char *op = find_keyword_op(src, "or", 2);
        if (op)
        {
            char left[MAX_LINE], right[MAX_LINE];
            trimcpy(left, src, op);
            trimcpy(right, op + 2, src + strlen(src));
            Value lv = eval_at_prec(left, 0);
            if (lv.num != 0.0)
                return make_num(1.0);
            Value rv = eval_at_prec(right, 0);
            return make_num(rv.num != 0.0 ? 1.0 : 0.0);
        }
    }

    /* -- Level 1: and -- */
    if (prec <= 1)
    {
        const char *op = find_keyword_op(src, "and", 3);
        if (op)
        {
            char left[MAX_LINE], right[MAX_LINE];
            trimcpy(left, src, op);
            trimcpy(right, op + 3, src + strlen(src));
            Value lv = eval_at_prec(left, 1);
            if (lv.num == 0.0)
                return make_num(0.0);
            Value rv = eval_at_prec(right, 1);
            return make_num(rv.num != 0.0 ? 1.0 : 0.0);
        }
    }

    /* -- Level 2: not -- */
    if (prec <= 2)
    {
        const char *p = skip_ws(src);
        if (!strncmp(p, "not", 3) && !is_ident_char(p[3]))
        {
            Value v = eval_at_prec(p + 3, 2);
            return make_num(v.num == 0.0 ? 1.0 : 0.0);
        }
    }

    /* -- Level 3: comparisons -- */
    if (prec <= 3)
    {
        OpMatch m = find_binary_op_rtl(src, 3);
        if (m.pos)
        {
            char left[MAX_LINE], right[MAX_LINE];
            trimcpy(left, src, m.pos);
            trimcpy(right, m.pos + m.len, src + strlen(src));

            /* For comparisons we need typed evaluation */
            Value lv = eval_at_prec(left, 4);
            Value rv = eval_at_prec(right, 4);

            if (lv.type != rv.type)
                vyom_error("cannot compare different types");

            if (lv.type == VT_STR)
            {
                int cmp = strcmp(lv.str, rv.str);
                if (m.op1 == '=' && m.op2 == '=')
                    return make_num(cmp == 0);
                if (m.op1 == '!' && m.op2 == '=')
                    return make_num(cmp != 0);
                if (m.op1 == '<' && m.op2 == '=')
                    return make_num(cmp <= 0);
                if (m.op1 == '>' && m.op2 == '=')
                    return make_num(cmp >= 0);
                if (m.op1 == '<')
                    return make_num(cmp < 0);
                if (m.op1 == '>')
                    return make_num(cmp > 0);
            }
            else
            {
                double a = lv.num, b = rv.num;
                if (m.op1 == '=' && m.op2 == '=')
                    return make_num(a == b);
                if (m.op1 == '!' && m.op2 == '=')
                    return make_num(a != b);
                if (m.op1 == '<' && m.op2 == '=')
                    return make_num(a <= b);
                if (m.op1 == '>' && m.op2 == '=')
                    return make_num(a >= b);
                if (m.op1 == '<')
                    return make_num(a < b);
                if (m.op1 == '>')
                    return make_num(a > b);
            }
        }
    }

    /* -- Level 4: + - -- */
    if (prec <= 4)
    {
        OpMatch m = find_binary_op_rtl(src, 4);
        if (m.pos)
        {
            char left[MAX_LINE], right[MAX_LINE];
            trimcpy(left, src, m.pos);
            trimcpy(right, m.pos + m.len, src + strlen(src));
            Value lv = eval_at_prec(left, 4);
            Value rv = eval_at_prec(right, 5);
            if (lv.type != VT_NUM || rv.type != VT_NUM)
                vyom_error("'+'/'-' requires numeric operands");
            return make_num(m.op1 == '+' ? lv.num + rv.num
                                         : lv.num - rv.num);
        }
    }

    /* -- Level 5: * / // % -- */
    if (prec <= 5)
    {
        OpMatch m = find_binary_op_rtl(src, 5);
        if (m.pos)
        {
            char left[MAX_LINE], right[MAX_LINE];
            trimcpy(left, src, m.pos);
            trimcpy(right, m.pos + m.len, src + strlen(src));
            Value lv = eval_at_prec(left, 5);
            Value rv = eval_at_prec(right, 6);
            if (lv.type != VT_NUM || rv.type != VT_NUM)
                vyom_error("arithmetic requires numeric operands");
            if (m.op1 == '*')
                return make_num(lv.num * rv.num);
            if (m.op1 == '%')
            {
                if (rv.num == 0.0)
                    vyom_error("modulo by zero");
                return make_num(fmod(lv.num, rv.num));
            }
            if (m.op1 == '/' && m.op2 == '/')
            {
                if (rv.num == 0.0)
                    vyom_error("integer division by zero");
                return make_num((double)(long long)(lv.num / rv.num));
            }
            if (m.op1 == '/')
            {
                if (rv.num == 0.0)
                    vyom_error("division by zero");
                return make_num(lv.num / rv.num);
            }
        }
    }

    /* -- Primary -- */
    return eval_primary(src);
}

/* ================================================================
 *  CLEANUP (called on error and normal exit)
 * ================================================================ */

static void
cleanup_all(void)
{
    /* pop all scopes (which decrefs arrays) */
    while (g_ndepth > 0)
        scope_pop();
}

/* ================================================================
 *  STATEMENT EXECUTION
 * ================================================================ */

/* Strip trailing line comment, respecting strings */
static void
strip_comment(char *line)
{
    int in_str = 0;
    for (char *p = line; *p; p++)
    {
        if (*p == '"')
            in_str = !in_str;
        if (!in_str && *p == '#')
        {
            *p = '\0';
            break;
        }
    }
    /* right-trim */
    int n = (int)strlen(line);
    while (n > 0 && isspace((unsigned char)line[n - 1]))
        line[--n] = '\0';
}

static int if_chain_end(int i); /* forward */

/* (skip_if_chain is now if_chain_end — defined above with exec_if) */
static int skip_if_chain(int i, int end)
{
    (void)end;
    return if_chain_end(i);
}

/* Execute a range of lines [start, end) at current scope */
static void
exec_block(int start, int end)
{
    int i = start;
    while (i < end)
    {
        if (g_ret_flag || g_brk_flag || g_cont_flag)
            break;

        char line[MAX_LINE];
        strncpy(line, g_lines[i].text, MAX_LINE - 1);
        line[MAX_LINE - 1] = '\0';
        strip_comment(line);

        const char *t = skip_ws(line);
        if (!*t)
        {
            i++;
            continue;
        }

        /* elif/else are handled by exec_if, not as standalone statements */
        if (!strncmp(t, "elif", 4) || !strncmp(t, "else", 4))
        {
            i++;
            continue;
        }

        g_cur_lineno = g_lines[i].lineno;
        exec_stmt(i);

        /* skip compound block lines so we don't re-execute elif/else/body */
        if (!strncmp(t, "if", 2) && (t[2] == ' ' || t[2] == '\t'))
        {
            i = skip_if_chain(i, end);
            continue;
        }
        if (!strncmp(t, "while", 5) && (t[5] == ' ' || t[5] == '\t'))
        {
            i = block_end(i);
            continue;
        }
        if (!strncmp(t, "for", 3) && (t[3] == ' ' || t[3] == '\t'))
        {
            i = block_end(i);
            continue;
        }
        i++;
    }
}

/* ------------------------------------------------------------------ */
/* print() — evaluates each argument, handles arrays via indexing     */
/* ------------------------------------------------------------------ */

static void
exec_print(const char *args_src)
{
    if (!*args_src)
    {
        printf("\n");
        return;
    }

    char argv[MAX_ARGS][MAX_LINE];
    int argc = split_args(args_src, argv, MAX_ARGS);

    for (int i = 0; i < argc; i++)
    {
        if (i)
            printf(" ");

        /* Check if it's an array variable — print its contents */
        const char *a = skip_ws(argv[i]);

        /* Check for bare array name (no indexing, no operators) */
        {
            const char *p = a;
            while (is_ident_char(*p))
                p++;
            const char *after = skip_ws(p);
            if (*after == '\0')
            {
                /* plain identifier — could be array */
                Value v;
                if (var_get(a, &v) && v.type == VT_ARR)
                {
                    /* print all elements space-separated */
                    ArrayData *ad = v.arr;
                    if (ad->cols > 0)
                    {
                        /* 2D: print row by row */
                        for (int r = 0; r < ad->rows; r++)
                        {
                            if (r)
                                printf("\n");
                            for (int c = 0; c < ad->cols; c++)
                            {
                                if (c)
                                    printf(" ");
                                printf("%g", ad->data.nums[r * ad->cols + c]);
                            }
                        }
                    }
                    else
                    {
                        for (int k = 0; k < ad->size; k++)
                        {
                            if (k)
                                printf(" ");
                            if (ad->elem_type == VT_NUM)
                                printf("%g", ad->data.nums[k]);
                            else
                                printf("%s", ad->data.strs[k] ? ad->data.strs[k] : "");
                        }
                    }
                    arr_decref(v.arr);
                    continue;
                }
                if (v.type == VT_ARR)
                    arr_decref(v.arr); /* decref if we got it */
            }
        }

        /* General expression */
        Value v = eval_expr(argv[i]);
        if (v.type == VT_NUM)
        {
            /* Print integers without decimal point */
            if (v.num == (long long)v.num &&
                v.num >= -1e15 && v.num <= 1e15)
                printf("%lld", (long long)v.num);
            else
                printf("%g", v.num);
        }
        else
        {
            printf("%s", v.str);
        }
        arr_decref(v.arr);
    }
    printf("\n");
}

/* ------------------------------------------------------------------ */
/* if / elif / else                                                    */
/* ------------------------------------------------------------------ */

/*
 * Return the line index just past the entire if/elif/else chain starting
 * at line i.  This skips the body of each clause AND the elif/else
 * header lines themselves (which are at the same indent as `if`).
 */
static int
if_chain_end(int i)
{
    int base = g_lines[i].indent;
    /* Start past the `if` body */
    int j = block_end(i);
    /* Keep consuming elif/else clauses at the same indent */
    while (j < g_nlines && g_lines[j].indent == base)
    {
        const char *s = skip_ws(g_lines[j].text);
        if (!strncmp(s, "elif", 4) || !strncmp(s, "else", 4))
            j = block_end(j);
        else
            break;
    }
    return j;
}

static void
exec_if(int i)
{
    int base = g_lines[i].indent;

    /*
     * Collect all clause header lines for this if-chain.
     * headers[0] = the `if` line itself
     * headers[1..n] = any `elif` or `else` lines at the same indent
     */
    int headers[64];
    int nheaders = 0;
    headers[nheaders++] = i;

    /* Scan forward: after the if body, look for elif/else at same indent */
    int j = block_end(i);
    while (j < g_nlines && g_lines[j].indent == base && nheaders < 64)
    {
        const char *s = skip_ws(g_lines[j].text);
        if (!strncmp(s, "elif", 4) || !strncmp(s, "else", 4))
        {
            headers[nheaders++] = j;
            j = block_end(j);
        }
        else
        {
            break;
        }
    }
    /* j is now the first line past the whole chain */
    int chain_end = j;

    /* Process each clause in order */
    for (int ci = 0; ci < nheaders; ci++)
    {
        int hi = headers[ci];

        char hline[MAX_LINE];
        strncpy(hline, g_lines[hi].text, MAX_LINE - 1);
        strip_comment(hline);
        char *ht = (char *)skip_ws(hline);

        /* Body of this clause runs from hi+1 to the next header (or chain_end) */
        int body_start = hi + 1;
        int body_end = (ci + 1 < nheaders) ? headers[ci + 1] : chain_end;

        /* ---- if clause ---- */
        if (!strncmp(ht, "if", 2) && (ht[2] == ' ' || ht[2] == '\t'))
        {
            char *cond_src = ht + 2;
            /* find last depth-0 colon */
            char *last_colon = NULL;
            int depth = 0, in_s = 0;
            for (char *pp = cond_src; *pp; pp++)
            {
                if (*pp == '"')
                {
                    in_s = !in_s;
                    continue;
                }
                if (in_s)
                    continue;
                if (*pp == '(' || *pp == '[')
                {
                    depth++;
                    continue;
                }
                if (*pp == ')' || *pp == ']')
                {
                    depth--;
                    continue;
                }
                if (depth == 0 && *pp == ':')
                    last_colon = pp;
            }
            if (!last_colon)
                vyom_error("missing ':' in if statement");
            *last_colon = '\0';

            double cv = eval_expr(skip_ws(cond_src)).num;
            if (cv != 0.0)
            {
                exec_block(body_start, body_end);
                return;
            }
            continue;
        }

        /* ---- elif clause ---- */
        if (!strncmp(ht, "elif", 4) && (ht[4] == ' ' || ht[4] == '\t'))
        {
            char *cond_src = ht + 4;
            char *last_colon = NULL;
            int depth = 0, in_s = 0;
            for (char *pp = cond_src; *pp; pp++)
            {
                if (*pp == '"')
                {
                    in_s = !in_s;
                    continue;
                }
                if (in_s)
                    continue;
                if (*pp == '(' || *pp == '[')
                {
                    depth++;
                    continue;
                }
                if (*pp == ')' || *pp == ']')
                {
                    depth--;
                    continue;
                }
                if (depth == 0 && *pp == ':')
                    last_colon = pp;
            }
            if (!last_colon)
                vyom_error("missing ':' in elif");
            *last_colon = '\0';

            double cv = eval_expr(skip_ws(cond_src)).num;
            if (cv != 0.0)
            {
                exec_block(body_start, body_end);
                return;
            }
            continue;
        }

        /* ---- else clause ---- */
        if (!strncmp(ht, "else", 4))
        {
            exec_block(body_start, body_end);
            return;
        }
    }
    /* No clause matched — nothing to execute */
}

/* ------------------------------------------------------------------ */
/* while loop                                                          */
/* ------------------------------------------------------------------ */

static void
exec_while(int i)
{
    char line[MAX_LINE];
    strncpy(line, g_lines[i].text, MAX_LINE - 1);
    strip_comment(line);
    char *t = (char *)skip_ws(line);

    char *cond_src = t + 5; /* skip "while" */
    char *colon = strrchr(cond_src, ':');
    if (!colon)
        vyom_error("missing ':' in while statement");
    *colon = '\0';

    char cond_buf[MAX_LINE];
    strncpy(cond_buf, skip_ws(cond_src), MAX_LINE - 1);

    int body_start = i + 1;
    int body_end = block_end(i);

    scope_push(0, 1);

    while (eval_expr(cond_buf).num != 0.0)
    {
        g_brk_flag = 0;
        g_cont_flag = 0;
        exec_block(body_start, body_end);
        if (g_ret_flag || g_brk_flag)
            break;
        g_cont_flag = 0;
    }

    scope_pop();
    g_brk_flag = 0;
    g_cont_flag = 0;
}

/* ------------------------------------------------------------------ */
/* for(;;) C-style loop                                               */
/* ------------------------------------------------------------------ */

/*
 * Execute a single simple statement string (used for for-loop init/step).
 * Supports: [typed] var = expr   and   var = expr
 */
static int try_assignment(const char *t, int is_typed, ValType decl_type, int is_const); /* forward */
static void
exec_simple_stmt(const char *stmt_src)
{
    if (!stmt_src || !*stmt_src)
        return;

    char buf[MAX_LINE];
    strncpy(buf, stmt_src, MAX_LINE - 1);
    buf[MAX_LINE - 1] = '\0';
    char *t = (char *)skip_ws(buf);
    if (!*t)
        return;

    int is_typed = 0;
    ValType decl_type = VT_NUM;

    if (!strncmp(t, "const", 5) && (t[5] == ' ' || t[5] == '\t'))
        t = (char *)skip_ws(t + 5);

    if (!strncmp(t, "int", 3) && (t[3] == ' ' || t[3] == '\t'))
    {
        is_typed = 1;
        decl_type = VT_NUM;
        t = (char *)skip_ws(t + 3);
    }
    else if (!strncmp(t, "double", 6) && (t[6] == ' ' || t[6] == '\t'))
    {
        is_typed = 1;
        decl_type = VT_NUM;
        t = (char *)skip_ws(t + 6);
    }
    else if (!strncmp(t, "string", 6) && (t[6] == ' ' || t[6] == '\t'))
    {
        is_typed = 1;
        decl_type = VT_STR;
        t = (char *)skip_ws(t + 6);
    }

    if (!try_assignment(t, is_typed, decl_type, 0))
        vyom_error("invalid for-loop init/step: %s", stmt_src);
}

static void
exec_for_cstyle(int i)
{
    char line[MAX_LINE];
    strncpy(line, g_lines[i].text, MAX_LINE - 1);
    strip_comment(line);
    char *t = (char *)skip_ws(line);

    /* for ( init ; cond ; step ) : */
    char *paren_start = strchr(t, '(');
    if (!paren_start)
        vyom_error("for loop requires '('");

    const char *paren_end = scan_to(paren_start + 1, ')');
    if (!paren_end)
        vyom_error("unmatched '(' in for loop");

    char clause[MAX_LINE];
    trimcpy(clause, paren_start + 1, paren_end);

    /* Split by first and second ';' — watch for strings with semicolons
       (rare, but we scan depth-aware) */
    char *s1 = NULL, *s2 = NULL;
    {
        int depth = 0, in_str = 0;
        for (char *p = clause; *p; p++)
        {
            if (*p == '"')
            {
                in_str = !in_str;
                continue;
            }
            if (in_str)
                continue;
            if (*p == '(' || *p == '[')
            {
                depth++;
                continue;
            }
            if (*p == ')' || *p == ']')
            {
                depth--;
                continue;
            }
            if (depth == 0 && *p == ';')
            {
                if (!s1)
                {
                    s1 = p;
                }
                else if (!s2)
                {
                    s2 = p;
                    break;
                }
            }
        }
    }
    if (!s1 || !s2)
        vyom_error("for loop requires two semicolons");
    *s1 = '\0';
    *s2 = '\0';

    char init_src[MAX_LINE], cond_src[MAX_LINE], step_src[MAX_LINE];
    strncpy(init_src, skip_ws(clause), MAX_LINE - 1);
    strncpy(cond_src, skip_ws(s1 + 1), MAX_LINE - 1);
    strncpy(step_src, skip_ws(s2 + 1), MAX_LINE - 1);
    /* right-trim */
    for (char *p = init_src + strlen(init_src) - 1; p >= init_src && isspace((unsigned char)*p); *p-- = '\0')
        ;
    for (char *p = cond_src + strlen(cond_src) - 1; p >= cond_src && isspace((unsigned char)*p); *p-- = '\0')
        ;
    for (char *p = step_src + strlen(step_src) - 1; p >= step_src && isspace((unsigned char)*p); *p-- = '\0')
        ;

    int body_start = i + 1;
    int body_end = block_end(i);

    scope_push(0, 1);

    /* init */
    if (*init_src)
        exec_simple_stmt(init_src);

    while (!*cond_src || eval_expr(cond_src).num != 0.0)
    {
        g_brk_flag = 0;
        g_cont_flag = 0;

        exec_block(body_start, body_end);
        if (g_ret_flag || g_brk_flag)
            break;
        g_cont_flag = 0;

        /* step */
        if (*step_src)
            exec_simple_stmt(step_src);
        if (g_ret_flag)
            break;
    }

    scope_pop();
    g_brk_flag = 0;
    g_cont_flag = 0;
}

/* ------------------------------------------------------------------ */
/* for <var> in range(...)                                             */
/* ------------------------------------------------------------------ */

static void
exec_for_range(int i)
{
    char line[MAX_LINE];
    strncpy(line, g_lines[i].text, MAX_LINE - 1);
    strip_comment(line);
    char *t = (char *)skip_ws(line);

    /* for <var> in range(...): */
    char *after_for = t + 3;
    while (*after_for == ' ' || *after_for == '\t')
        after_for++;

    char *in_kw = strstr(after_for, " in ");
    if (!in_kw)
        vyom_error("for-in loop requires 'in' keyword");

    char loop_var[64];
    trimcpy(loop_var, after_for, in_kw);

    char *range_kw = strstr(in_kw + 4, "range");
    if (!range_kw)
        vyom_error("for-in requires range()");

    char *paren = strchr(range_kw, '(');
    if (!paren)
        vyom_error("range requires '('");

    const char *paren_end = scan_to(paren + 1, ')');
    if (!paren_end)
        vyom_error("unmatched '(' in range()");

    char range_args[MAX_LINE];
    trimcpy(range_args, paren + 1, paren_end);

    char argv[4][MAX_LINE];
    int argc = split_args(range_args, argv, 3);

    double start = 0, stop = 0, step = 1;
    if (argc == 1)
    {
        stop = eval_expr(argv[0]).num;
    }
    else if (argc == 2)
    {
        start = eval_expr(argv[0]).num;
        stop = eval_expr(argv[1]).num;
    }
    else if (argc == 3)
    {
        start = eval_expr(argv[0]).num;
        stop = eval_expr(argv[1]).num;
        step = eval_expr(argv[2]).num;
    }
    else
    {
        vyom_error("range() takes 1–3 arguments");
    }

    if (step == 0.0)
        vyom_error("range() step cannot be zero");

    int body_start = i + 1;
    int body_end = block_end(i);

    scope_push(0, 1);

    double idx = start;
    while ((step > 0 && idx < stop) || (step < 0 && idx > stop))
    {
        /* Clear per-iteration flags BEFORE assigning the loop variable
           so the variable is always set to the current iteration value
           with a clean flag state.                                      */
        g_brk_flag = 0;
        g_cont_flag = 0;

        /* Assign loop variable for THIS iteration */
        Value iv = make_num(idx);
        var_set(loop_var, iv, 0, 0);

        /* Execute loop body */
        exec_block(body_start, body_end);
        if (g_ret_flag || g_brk_flag)
            break;
        g_cont_flag = 0;

        idx += step;
    }

    scope_pop();
    g_brk_flag = 0;
    g_cont_flag = 0;
}

/* ------------------------------------------------------------------ */
/* Array declaration helpers                                           */
/* ------------------------------------------------------------------ */

/*
 * Parse and execute a typed array declaration.
 * `decl`     : text after the type keyword, e.g. "a[10]" or "m[3][4] = [[...]]"
 * `elem_type`: VT_NUM or VT_STR
 * `is_const` : const flag
 */
static void
exec_array_decl(const char *decl, ValType elem_type, int is_const)
{
    const char *bracket = strchr(decl, '[');
    if (!bracket)
        vyom_error("expected '[' in array declaration");

    char varname[64];
    trimcpy(varname, decl, bracket);

    const char *bracket_end = scan_to(bracket + 1, ']');
    if (!bracket_end)
        vyom_error("unmatched '[' in array declaration");

    char size_expr[MAX_LINE];
    trimcpy(size_expr, bracket + 1, bracket_end);

    Value sv = eval_expr(size_expr);
    if (sv.type != VT_NUM || sv.num != (int)sv.num || sv.num <= 0)
        vyom_error("array size must be a positive integer");
    int dim1 = (int)sv.num;

    const char *after_first = skip_ws(bracket_end + 1);

    /* 2D array? */
    if (*after_first == '[')
    {
        if (elem_type != VT_NUM)
            vyom_error("2D arrays only support numeric types");

        const char *bracket2_end = scan_to(after_first + 1, ']');
        if (!bracket2_end)
            vyom_error("unmatched '[' in 2D array declaration");

        char cols_expr[MAX_LINE];
        trimcpy(cols_expr, after_first + 1, bracket2_end);

        Value cv = eval_expr(cols_expr);
        if (cv.type != VT_NUM || cv.num != (int)cv.num || cv.num <= 0)
            vyom_error("array column size must be a positive integer");
        int dim2 = (int)cv.num;

        ArrayData *a = arr_alloc(VT_NUM, dim1, dim2);

        const char *after_decl = skip_ws(bracket2_end + 1);

        /* optional initializer */
        if (*after_decl == '=')
        {
            const char *init = skip_ws(after_decl + 1);
            if (*init != '[')
                vyom_error("2D array initializer must start with '['");

            /* outer bracket */
            const char *outer_end = scan_to(init + 1, ']');
            if (!outer_end)
                vyom_error("unmatched '[' in 2D array initializer");

            char init_inner[MAX_LINE];
            trimcpy(init_inner, init + 1, outer_end);

            /* split rows by '],' pattern — rows are sub-arrays */
            int row = 0;
            const char *p = skip_ws(init_inner);
            while (*p && row < dim1)
            {
                if (*p != '[')
                    break;
                const char *row_end = scan_to(p + 1, ']');
                if (!row_end)
                    vyom_error("unmatched '[' in 2D row initializer");

                char row_str[MAX_LINE];
                trimcpy(row_str, p + 1, row_end);

                char elems[MAX_ARRAY_TOTAL > 256 ? 256 : MAX_ARRAY_TOTAL][MAX_LINE];
                int nelem = *row_str ? split_args(row_str, elems,
                                                  dim2 < 256 ? dim2 : 256)
                                     : 0;

                for (int c = 0; c < nelem && c < dim2; c++)
                {
                    Value ev = eval_expr(elems[c]);
                    a->data.nums[row * dim2 + c] = ev.num;
                }
                row++;
                p = skip_ws(row_end + 1);
                if (*p == ',')
                    p = skip_ws(p + 1);
            }
        }

        Value vv = {0};
        vv.type = VT_ARR;
        vv.arr = a;
        var_set(varname, vv, 1, is_const);
        arr_decref(a); /* var_set did incref */
        return;
    }

    /* 1D array */
    ArrayData *a = arr_alloc(elem_type, dim1, 0);

    const char *after_decl = skip_ws(after_first);

    if (*after_decl == '=')
    {
        const char *init = skip_ws(after_decl + 1);
        if (*init != '[')
            vyom_error("array initializer must start with '['");

        const char *init_end = scan_to(init + 1, ']');
        if (!init_end)
            vyom_error("unmatched '[' in array initializer");

        char init_inner[MAX_LINE];
        trimcpy(init_inner, init + 1, init_end);

        /* split by commas */
        int max_elems = dim1 < 256 ? dim1 : 256;
        char elems[256][MAX_LINE];
        int nelem = *init_inner ? split_args(init_inner, elems, max_elems) : 0;

        for (int k = 0; k < nelem && k < dim1; k++)
        {
            Value ev = eval_expr(elems[k]);
            if (elem_type == VT_NUM)
            {
                a->data.nums[k] = ev.num;
            }
            else
            {
                if (ev.type != VT_STR)
                    vyom_error("string array requires string values");
                free(a->data.strs[k]);
                a->data.strs[k] = strdup(ev.str);
            }
        }
    }

    Value vv = {0};
    vv.type = VT_ARR;
    vv.arr = a;
    var_set(varname, vv, 1, is_const);
    arr_decref(a);
}

/* ------------------------------------------------------------------ */
/* Assignment (including array element assignment)                    */
/* ------------------------------------------------------------------ */

/*
 * Try to parse `t` as an assignment (lhs = rhs).
 * Returns 1 if handled, 0 otherwise.
 */
static int
try_assignment(const char *t, int is_typed, ValType decl_type, int is_const)
{
    /* Find '=' that is not ==, !=, <=, >= */
    int depth = 0;
    int in_str = 0;
    const char *eq = NULL;

    for (const char *p = t; *p; p++)
    {
        if (*p == '"')
        {
            in_str = !in_str;
            continue;
        }
        if (in_str)
            continue;
        if (*p == '(' || *p == '[')
        {
            depth++;
            continue;
        }
        if (*p == ')' || *p == ']')
        {
            depth--;
            continue;
        }
        if (depth != 0)
            continue;

        if (*p == '=' && p[1] != '=')
        {
            /* make sure previous char isn't !, <, > */
            if (p > t && (p[-1] == '!' || p[-1] == '<' || p[-1] == '>'))
                continue;
            eq = p;
            break;
        }
    }

    if (!eq)
        return 0;

    /* RHS */
    const char *rhs_src = skip_ws(eq + 1);
    if (!*rhs_src)
        vyom_error("empty right-hand side in assignment");

    /* LHS — check for array index */
    char lhs_buf[MAX_LINE];
    trimcpy(lhs_buf, t, eq);
    char *lhs = lhs_buf;

    char *bracket = strchr(lhs, '[');
    if (bracket)
    {
        /* array element assignment */
        char varname[64];
        trimcpy(varname, lhs, bracket);

        Var *varp = var_find(varname);
        if (!varp)
            vyom_error("undefined variable '%s'", varname);
        if (varp->is_const)
            vyom_error("cannot modify const array '%s'", varname);
        if (varp->val.type != VT_ARR)
            vyom_error("'%s' is not an array", varname);

        ArrayData *a = varp->val.arr;

        const char *rb1 = scan_to(bracket + 1, ']');
        if (!rb1)
            vyom_error("unmatched '[' in array assignment");

        char idx1_expr[MAX_LINE];
        trimcpy(idx1_expr, bracket + 1, rb1);
        Value iv1 = eval_expr(idx1_expr);
        if (iv1.type != VT_NUM || iv1.num != (int)iv1.num)
            vyom_error("array index must be an integer");
        int idx1 = (int)iv1.num;

        const char *after1 = skip_ws(rb1 + 1);

        /* 2D? */
        if (a->cols > 0)
        {
            if (*after1 != '[')
                vyom_error("2D array requires two indices");
            const char *rb2 = scan_to(after1 + 1, ']');
            if (!rb2)
                vyom_error("unmatched '[' in 2D array assignment");

            char idx2_expr[MAX_LINE];
            trimcpy(idx2_expr, after1 + 1, rb2);
            Value iv2 = eval_expr(idx2_expr);
            if (iv2.type != VT_NUM || iv2.num != (int)iv2.num)
                vyom_error("array column index must be an integer");
            int idx2 = (int)iv2.num;

            if (idx1 < 0 || idx1 >= a->rows)
                vyom_error("2D array row index %d out of bounds (%d rows)", idx1, a->rows);
            if (idx2 < 0 || idx2 >= a->cols)
                vyom_error("2D array col index %d out of bounds (%d cols)", idx2, a->cols);

            Value rv = eval_expr(rhs_src);
            if (rv.type != VT_NUM)
                vyom_error("2D array element must be numeric");
            a->data.nums[idx1 * a->cols + idx2] = rv.num;
            return 1;
        }

        /* 1D */
        if (idx1 < 0 || idx1 >= a->size)
            vyom_error("array index %d out of bounds (size %d)", idx1, a->size);

        Value rv = eval_expr(rhs_src);
        if (a->elem_type == VT_NUM)
        {
            if (rv.type != VT_NUM)
                vyom_error("numeric array requires numeric value");
            a->data.nums[idx1] = rv.num;
        }
        else
        {
            if (rv.type != VT_STR)
                vyom_error("string array requires string value");
            free(a->data.strs[idx1]);
            a->data.strs[idx1] = strdup(rv.str);
        }
        arr_decref(rv.arr);
        return 1;
    }

    /* Plain variable assignment */
    Value rv = eval_expr(rhs_src);

    if (is_typed && decl_type == VT_STR && rv.type != VT_STR)
        vyom_error("type mismatch: cannot assign non-string to string variable '%s'", lhs);
    if (is_typed && decl_type == VT_NUM && rv.type != VT_NUM)
        vyom_error("type mismatch: cannot assign non-numeric to numeric variable '%s'", lhs);

    var_set(lhs, rv, is_typed, is_const);
    arr_decref(rv.arr);
    return 1;
}

/* ------------------------------------------------------------------ */
/* Main statement dispatcher                                           */
/* ------------------------------------------------------------------ */

/*
 * `line_idx` is the index into g_lines[].
 * Pass -1 to use g_cur_lineno with an already-prepared statement string
 * (only used internally by for-loop init/step).
 */
static void
exec_stmt(int line_idx)
{
    char line[MAX_LINE];

    if (line_idx >= 0)
    {
        strncpy(line, g_lines[line_idx].text, MAX_LINE - 1);
        line[MAX_LINE - 1] = '\0';
        strip_comment(line);
        g_cur_lineno = g_lines[line_idx].lineno;
    }
    else
    {
        /* caller set up line via exec_block — shouldn't reach here normally */
        return;
    }

    char *t = (char *)skip_ws(line);
    if (!*t || *t == '#')
        return;

    /* ---- def: skip (already collected) ---- */
    if (!strncmp(t, "def", 3) && (t[3] == ' ' || t[3] == '\t'))
        return;

    /* ---- elif / else: handled by exec_if ---- */
    if (!strncmp(t, "elif", 4) || !strncmp(t, "else", 4))
        return;

    /* ---- break ---- */
    if (!strcmp(t, "break"))
    {
        if (g_loop_depth == 0)
            vyom_error("'break' outside of loop");
        g_brk_flag = 1;
        return;
    }

    /* ---- continue ---- */
    if (!strcmp(t, "continue"))
    {
        if (g_loop_depth == 0)
            vyom_error("'continue' outside of loop");
        g_cont_flag = 1;
        return;
    }

    /* ---- return ---- */
    if (!strncmp(t, "return", 6) && (t[6] == ' ' || t[6] == '\t' || t[6] == '\0'))
    {
        /* check we are inside a function frame (not just global scope) */
        int in_func = 0;
        for (int d = g_ndepth - 1; d >= 0; d--)
        {
            if (g_scopes[d].is_func)
            {
                in_func = 1;
                break;
            }
        }
        if (!in_func)
            vyom_error("'return' outside of function");
        const char *expr = skip_ws(t + 6);
        g_ret_val = *expr ? eval_expr(expr) : make_num(0.0);
        g_ret_flag = 1;
        return;
    }

    /* ---- exit() ---- */
    if (!strncmp(t, "exit", 4) && t[4] == '(')
    {
        const char *rp = scan_to(t + 5, ')');
        if (!rp)
            vyom_error("unmatched '(' in exit()");
        char arg[MAX_LINE];
        trimcpy(arg, t + 5, rp);
        int code = *arg ? (int)eval_expr(arg).num : 0;
        cleanup_all();
        exit(code);
    }

    /* ---- input() as statement (result discarded) ---- */
    if (!strncmp(t, "input", 5) && t[5] == '(')
    {
        const char *rp = scan_to(t + 6, ')');
        if (!rp)
            vyom_error("unmatched '(' in input()");
        char ibuf[MAX_STR];
        if (fgets(ibuf, sizeof(ibuf), stdin))
        { /* consume and discard */
        }
        return;
    }

    /* ---- print() ---- */
    if (!strncmp(t, "print", 5) && t[5] == '(')
    {
        const char *rp = scan_to(t + 6, ')');
        if (!rp)
            vyom_error("unmatched '(' in print()");
        char args[MAX_LINE];
        trimcpy(args, t + 6, rp);
        exec_print(args);
        return;
    }

    if (!strncmp(t, "print", 5) && (t[5] == ' ' || t[5] == '\t'))
        vyom_error("print requires parentheses: use print(...)");

    /* ---- if ---- */
    if (!strncmp(t, "if", 2) && (t[2] == ' ' || t[2] == '\t'))
    {
        exec_if(line_idx);
        return;
    }

    /* ---- while ---- */
    if (!strncmp(t, "while", 5) && (t[5] == ' ' || t[5] == '\t'))
    {
        exec_while(line_idx);
        return;
    }

    /* ---- for ---- */
    if (!strncmp(t, "for", 3) && (t[3] == ' ' || t[3] == '\t'))
    {
        /* distinguish for(;;) from for x in range() */
        const char *after = skip_ws(t + 3);
        if (*after == '(')
            exec_for_cstyle(line_idx);
        else
            exec_for_range(line_idx);
        return;
    }

    /* ---- const / typed declarations ---- */
    int is_const = 0;
    int is_typed = 0;
    ValType decl_type = VT_NUM;
    char *decl = t;

    if (!strncmp(decl, "const", 5) && (decl[5] == ' ' || decl[5] == '\t'))
    {
        is_const = 1;
        decl = (char *)skip_ws(decl + 5);
    }

    if (!strncmp(decl, "int", 3) && (decl[3] == ' ' || decl[3] == '\t'))
    {
        is_typed = 1;
        decl_type = VT_NUM;
        decl = (char *)skip_ws(decl + 3);
    }
    else if (!strncmp(decl, "double", 6) && (decl[6] == ' ' || decl[6] == '\t'))
    {
        is_typed = 1;
        decl_type = VT_NUM;
        decl = (char *)skip_ws(decl + 6);
    }
    else if (!strncmp(decl, "string", 6) && (decl[6] == ' ' || decl[6] == '\t'))
    {
        is_typed = 1;
        decl_type = VT_STR;
        decl = (char *)skip_ws(decl + 6);
    }

    /* Array declaration: typed keyword followed by name[...] */
    if (is_typed && strchr(decl, '['))
    {
        /* make sure the '[' comes before any '=' */
        const char *b = strchr(decl, '[');
        const char *e = strchr(decl, '=');
        if (!e || b < e)
        {
            exec_array_decl(decl, decl_type, is_const);
            return;
        }
    }

    /* Try assignment */
    if (try_assignment(decl, is_typed, decl_type, is_const))
        return;

    if (is_const)
        vyom_error("const variable must be initialized");

    /* Function call as statement */
    char *lp = strchr(t, '(');
    if (lp)
    {
        char fname[64];
        trimcpy(fname, t, lp);
        Func *fn = func_find(fname);
        if (fn)
        {
            const char *rp = scan_to(lp + 1, ')');
            if (!rp)
                vyom_error("unmatched '(' calling '%s'", fname);

            char args_str[MAX_LINE];
            trimcpy(args_str, lp + 1, rp);

            char argv[MAX_ARGS][MAX_LINE];
            int argc = *args_str ? split_args(args_str, argv, MAX_ARGS) : 0;

            if (argc != fn->nparams)
                vyom_error("function '%s' expects %d args, got %d",
                           fname, fn->nparams, argc);

            Value argvals[MAX_ARGS];
            for (int i = 0; i < argc; i++)
                argvals[i] = eval_expr(argv[i]);

            int old_ret = g_ret_flag;
            int old_brk = g_brk_flag;
            int old_cont = g_cont_flag;
            Value old_rval = g_ret_val;
            g_ret_flag = g_brk_flag = g_cont_flag = 0;

            if (g_func_depth >= MAX_FUNC_DEPTH)
                vyom_error("call stack overflow (max recursion depth %d)", MAX_FUNC_DEPTH);

            g_func_depth++;
            scope_push(1, 0);

            for (int i = 0; i < argc; i++)
            {
                var_set(fn->params[i], argvals[i], 0, 0);
                arr_decref(argvals[i].arr);
            }

            exec_block(fn->body_start, fn->body_end);
            scope_pop();
            g_func_depth--;

            g_ret_flag = old_ret;
            g_brk_flag = old_brk;
            g_cont_flag = old_cont;
            /* discard return value — statement call */
            arr_decref(g_ret_val.arr);
            g_ret_val = old_rval;
            return;
        }
    }

    vyom_error("unrecognised statement: %s", t);
}

/* ================================================================
 *  FUNCTION COLLECTION  (pre-pass before execution)
 * ================================================================ */

static void
collect_functions(void)
{
    for (int i = 0; i < g_nlines; i++)
    {
        char line[MAX_LINE];
        strncpy(line, g_lines[i].text, MAX_LINE - 1);
        strip_comment(line);
        char *t = (char *)skip_ws(line);

        if (strncmp(t, "def", 3) || (t[3] != ' ' && t[3] != '\t'))
            continue;

        char *after_def = (char *)skip_ws(t + 3);
        char *paren = strchr(after_def, '(');
        if (!paren)
            vyom_error("invalid function definition (missing '(')");

        if (g_nfuncs >= MAX_FUNCS)
            vyom_error("too many function definitions");
        Func *f = &g_funcs[g_nfuncs++];
        memset(f, 0, sizeof(Func));

        trimcpy(f->name, after_def, paren);

        const char *rparen = scan_to(paren + 1, ')');
        if (!rparen)
            vyom_error("unmatched '(' in function definition '%s'", f->name);

        char params_str[MAX_LINE];
        trimcpy(params_str, paren + 1, rparen);

        f->nparams = 0;
        if (*params_str)
        {
            char argv[MAX_ARGS][MAX_LINE];
            int n = split_args(params_str, argv, MAX_ARGS);
            for (int k = 0; k < n; k++)
                strncpy(f->params[k], argv[k], 63);
            f->nparams = n;
        }

        f->body_start = i + 1;
        f->body_end = block_end(i);
    }
}

/* ================================================================
 *  MAIN
 * ================================================================ */

int main(int argc, char **argv)
{
    if (argc == 2 && !strcmp(argv[1], "--version"))
    {
        printf("Vyom v%s\n", VYOM_VERSION);
        return 0;
    }

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <file.vy>\n", argv[0]);
        return 1;
    }

    strncpy(g_filename, argv[1], sizeof(g_filename) - 1);

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        fprintf(stderr, "Cannot open file: %s\n", argv[1]);
        return 1;
    }

    char buf[MAX_LINE];
    int lineno = 1;

    while (fgets(buf, sizeof(buf), fp))
    {
        if (g_nlines >= MAX_LINES)
        {
            fprintf(stderr, "Error: source file too long (max %d lines)\n", MAX_LINES);
            fclose(fp);
            return 1;
        }

        /* detect indent before trimming */
        int indent = 0;
        const char *p = buf;
        while (*p == ' ')
        {
            indent++;
            p++;
        }
        while (*p == '\t')
        {
            indent += 4;
            p++;
        }

        /* store trimmed text */
        strncpy(g_lines[g_nlines].text, p, MAX_LINE - 1);
        g_lines[g_nlines].text[MAX_LINE - 1] = '\0';
        /* strip trailing newline */
        int tlen = (int)strlen(g_lines[g_nlines].text);
        while (tlen > 0 && (g_lines[g_nlines].text[tlen - 1] == '\n' ||
                            g_lines[g_nlines].text[tlen - 1] == '\r'))
            g_lines[g_nlines].text[--tlen] = '\0';

        g_lines[g_nlines].indent = indent;
        g_lines[g_nlines].lineno = lineno++;
        g_nlines++;
    }
    fclose(fp);

    /* Push global scope */
    scope_push(0, 0);

    /* Collect all function definitions */
    collect_functions();

    /* Execute top-level statements */
    for (int i = 0; i < g_nlines; i++)
    {
        char line[MAX_LINE];
        strncpy(line, g_lines[i].text, MAX_LINE - 1);
        strip_comment(line);
        const char *t = skip_ws(line);

        if (!*t)
            continue;

        g_cur_lineno = g_lines[i].lineno;

        /* Skip function definitions */
        if (!strncmp(t, "def", 3) && (t[3] == ' ' || t[3] == '\t'))
        {
            i = block_end(i) - 1;
            continue;
        }

        /* Skip elif/else — handled by exec_if */
        if (!strncmp(t, "elif", 4) || !strncmp(t, "else", 4))
            continue;

        exec_stmt(i);

        /* skip past blocks */
        if (!strncmp(t, "if", 2) && (t[2] == ' ' || t[2] == '\t'))
        {
            i = skip_if_chain(i, g_nlines) - 1;
            continue;
        }
        if (!strncmp(t, "while", 5) && (t[5] == ' ' || t[5] == '\t'))
        {
            i = block_end(i) - 1;
            continue;
        }
        if (!strncmp(t, "for", 3) && (t[3] == ' ' || t[3] == '\t'))
        {
            i = block_end(i) - 1;
            continue;
        }
    }

    cleanup_all();
    return 0;
}