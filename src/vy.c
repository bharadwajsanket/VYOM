// Vyom Programming Language
// Created by Sanket Bharadwaj
// Vyom v0.7 — Strict Core + Developer Quality

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 2048
#define MAX_LINE  512
#define MAX_VARS  256
#define MAX_FUNCS 128
#define MAX_CALL_DEPTH 64
#define MAX_ARGS  8
#define VYOM_VERSION "0.7"

// ================= TYPES =================

typedef enum { V_NUM, V_STR } ValType;

typedef struct {
    char name[64];
    ValType type;
    double num;
    char str[256];
    int explicit;      // is this a typed variable?
    int is_const;      // is this a const variable?
} Var;

typedef struct {
    char name[64];
    int start;
    int end;
    char params[MAX_ARGS][64];
    int param_count;
} Func;

typedef struct {
    Var locals[MAX_VARS];
    int local_count;
    int is_loop_scope;  // NEW: track if this is a loop block scope
} CallFrame;

typedef struct {
    char text[MAX_LINE];
    int indent;
    int line_num;
} Line;

// ================= STORAGE =================

Line program[MAX_LINES];
int line_count = 0;

Var globals[MAX_VARS];
int global_count = 0;

Func funcs[MAX_FUNCS];
int func_count = 0;

CallFrame call_stack[MAX_CALL_DEPTH];
int call_depth = 0;

double return_value = 0;
int did_return = 0;
int did_break = 0;     // NEW: for break statement
int did_continue = 0;  // NEW: for continue statement

int current_line = 0;
char current_filename[256] = "";  // NEW: for __file__ introspection

// ================= HELPERS =================

int get_indent(const char *s) {
    int n = 0;
    while (*s == ' ' || *s == '\t') { n++; s++; }
    return n;
}

char* trim(char *s) {
    while (isspace(*s)) s++;
    char *e = s + strlen(s) - 1;
    while (e >= s && isspace(*e)) *e-- = 0;
    return s;
}

int find_block_end(int i) {
    int base = program[i].indent;
    i++;
    while (i < line_count && program[i].indent > base) i++;
    return i;
}

void error(const char *msg, const char *name) {
    printf("Error (line %d): %s", current_line, msg);
    if (name) printf(" '%s'", name);
    printf("\n");
    exit(1);
}

// ================= SCOPE MANAGEMENT =================

// Push a new scope (for loops)
void push_scope(int is_loop) {
    if (call_depth >= MAX_CALL_DEPTH)
        error("scope stack overflow", NULL);
    
    call_stack[call_depth].local_count = 0;
    call_stack[call_depth].is_loop_scope = is_loop;
    call_depth++;
}

// Pop a scope (after loops)
void pop_scope() {
    if (call_depth == 0)
        error("internal error: scope underflow", NULL);
    call_depth--;
}

// Check if we're currently inside any loop
int inside_loop() {
    for (int i = call_depth - 1; i >= 0; i--) {
        if (call_stack[i].is_loop_scope) return 1;
    }
    return 0;
}

// ================= VARIABLES =================

int find_global(const char *name) {
    for (int i = global_count - 1; i >= 0; i--)
        if (!strcmp(globals[i].name, name)) return i;
    return -1;
}

int find_local(const char *name) {
    // Search from innermost to outermost scope
    for (int depth = call_depth - 1; depth >= 0; depth--) {
        CallFrame *f = &call_stack[depth];
        for (int i = f->local_count - 1; i >= 0; i--) {
            if (!strcmp(f->locals[i].name, name)) {
                // Found in this scope at this depth
                return depth * MAX_VARS + i;  // Encode depth + index
            }
        }
    }
    return -1;
}

int get_var(const char *name, Var *out) {
    // Check for introspection constants
    if (!strcmp(name, "__version__")) {
        out->type = V_STR;
        strcpy(out->str, VYOM_VERSION);
        out->is_const = 1;
        strcpy(out->name, "__version__");
        return 1;
    }
    if (!strcmp(name, "__file__")) {
        out->type = V_STR;
        strcpy(out->str, current_filename);
        out->is_const = 1;
        strcpy(out->name, "__file__");
        return 1;
    }
    
    int li = find_local(name);
    if (li != -1) {
        int depth = li / MAX_VARS;
        int idx = li % MAX_VARS;
        *out = call_stack[depth].locals[idx];
        return 1;
    }
    int gi = find_global(name);
    if (gi != -1) {
        *out = globals[gi];
        return 1;
    }
    return 0;
}

void set_var(Var v) {
    // Check for introspection constants
    if (!strcmp(v.name, "__version__") || !strcmp(v.name, "__file__")) {
        error("cannot reassign built-in constant", v.name);
    }
    
    int gi = find_global(v.name);
    int li = find_local(v.name);

    // If variable exists locally (in any scope), modify it
    if (li != -1) {
        int depth = li / MAX_VARS;
        int idx = li % MAX_VARS;
        
        // Check const
        if (call_stack[depth].locals[idx].is_const) {
            error("cannot reassign const variable", v.name);
        }
        
        // Check type
        if (call_stack[depth].locals[idx].explicit &&
            call_stack[depth].locals[idx].type != v.type) {
            error("cannot change type of variable", v.name);
        }
        
        // Preserve const and explicit flags
        v.is_const = call_stack[depth].locals[idx].is_const;
        v.explicit = call_stack[depth].locals[idx].explicit;
        
        call_stack[depth].locals[idx] = v;
        return;
    }

    // Variable doesn't exist locally
    // Check if it exists globally
    if (gi != -1) {
        // Variable exists globally
        
        // Check if we're in some local scope
        if (call_depth > 0) {
            // If this is an explicitly typed or const variable, it's a declaration (shadowing error)
            // Otherwise, it's just modifying the global
            if (v.explicit || v.is_const) {
                error("variable shadows global variable", v.name);
            }
        }
        
        // Modify global
        if (globals[gi].is_const)
            error("cannot reassign const variable", v.name);
        if (globals[gi].explicit && globals[gi].type != v.type)
            error("cannot change type of variable", v.name);
        
        // Preserve const and explicit flags
        v.is_const = globals[gi].is_const;
        v.explicit = globals[gi].explicit;
        
        globals[gi] = v;
        return;
    }

    // Variable doesn't exist anywhere - create new
    if (call_depth > 0) {
        // Create in current local scope
        CallFrame *f = &call_stack[call_depth - 1];
        f->locals[f->local_count++] = v;
        return;
    }

    // Create in global scope
    globals[global_count++] = v;
}

// ================= FORWARD DECLARATIONS =================

int find_func(const char *name);
double eval_condition(const char *s);
void exec_statement(char *t);
void exec_block(int start, int end);

// ================= FUNCTION CALLS =================

int find_func(const char *name) {
    for (int i = 0; i < func_count; i++)
        if (!strcmp(funcs[i].name, name)) return i;
    return -1;
}

// -------- function call (expression context) --------
double call_function_expr(Func *f, char args[MAX_ARGS][64], int argc) {
    if (argc != f->param_count)
        error("incorrect argument count for function", f->name);

    if (call_depth >= MAX_CALL_DEPTH)
        error("call stack overflow", NULL);

    double values[MAX_ARGS];
    for (int i = 0; i < argc; i++) {
        values[i] = eval_condition(args[i]);
    }

    call_stack[call_depth].local_count = 0;
    call_stack[call_depth].is_loop_scope = 0;  // function scope, not loop
    call_depth++;

    for (int i = 0; i < argc; i++) {
        if (find_global(f->params[i]) != -1)
            error("parameter shadows global variable", f->params[i]);

        Var v = {0};
        strcpy(v.name, f->params[i]);
        v.type = V_NUM;
        v.num = values[i];
        v.explicit = 0;
        v.is_const = 0;
        set_var(v);
    }

    did_return = 0;

    for (int i = f->start; i < f->end && !did_return; i++) {
        current_line = program[i].line_num;
        char tmp[MAX_LINE];
        strcpy(tmp, program[i].text);
        exec_statement(trim(tmp));
    }

    call_depth--;

    if (!did_return)
        error("function missing return statement", f->name);

    return return_value;
}

// -------- function call (statement context) --------
void call_function_stmt(Func *f, char args[MAX_ARGS][64], int argc) {
    if (call_depth >= MAX_CALL_DEPTH)
        error("call stack overflow", NULL);

    double values[MAX_ARGS];
    for (int i = 0; i < argc; i++) {
        values[i] = eval_condition(args[i]);
    }

    call_stack[call_depth].local_count = 0;
    call_stack[call_depth].is_loop_scope = 0;  // function scope, not loop
    call_depth++;

    for (int i = 0; i < argc; i++) {
        if (find_global(f->params[i]) != -1)
            error("parameter shadows global variable", f->params[i]);

        Var v = {0};
        strcpy(v.name, f->params[i]);
        v.type = V_NUM;
        v.num = values[i];
        v.explicit = 0;
        v.is_const = 0;
        set_var(v);
    }

    did_return = 0;

    for (int i = f->start; i < f->end; i++) {
        current_line = program[i].line_num;
        char tmp[MAX_LINE];
        strcpy(tmp, program[i].text);
        exec_statement(trim(tmp));
        if (did_return) break;
    }

    call_depth--;
    did_return = 0; // ignore return value
}

// ================= EXPRESSION EVALUATION PIPELINE =================

// LEVEL 5: eval_term — variables, numbers, 'not', function calls
double eval_term(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);

    if (*str == '\0') {
        return 0;
    }

    if (*str == '(' && str[strlen(str) - 1] == ')') {
        char inner[MAX_LINE];
        strncpy(inner, str + 1, strlen(str) - 2);
        inner[strlen(str) - 2] = 0;
        return eval_condition(trim(inner));
    }

    if (!strncmp(str, "not", 3) && (str[3] == ' ' || str[3] == '\t' || str[3] == '(')) {
        double val = eval_term(trim(str + 3));
        return (val == 0) ? 1 : 0;
    }

    char *lp = strchr(str, '(');
    if (lp) {
        char fname[64];
        strncpy(fname, str, lp - str);
        fname[lp - str] = 0;
        trim(fname);

        int fi = find_func(fname);
        if (fi != -1) {
            char *rp = strrchr(str, ')');
            if (!rp) error("unmatched parentheses", NULL);

            char args_str[MAX_LINE];
            strncpy(args_str, lp + 1, rp - lp - 1);
            args_str[rp - lp - 1] = 0;

            char args[MAX_ARGS][64];
            int argc = 0;
            char *tok = strtok(args_str, ",");
            while (tok && argc < MAX_ARGS) {
                strcpy(args[argc++], trim(tok));
                tok = strtok(NULL, ",");
            }

            return call_function_expr(&funcs[fi], args, argc);
        }
    }

    Var v;
    if (get_var(str, &v)) {
        if (v.type != V_NUM) error("not a number", str);
        return v.num;
    }

    if (isdigit(*str) || (*str == '-' && isdigit(str[1])))
        return atof(str);

    error("name not defined", str);
    return 0;
}

// LEVEL 4: eval_expr — arithmetic (+ - * /)
double eval_expr(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);

    int depth = 0;
    for (char *p = str + strlen(str) - 1; p >= str; p--) {
        if (*p == ')') depth++;
        else if (*p == '(') depth--;
        else if (depth == 0 && (*p == '+' || *p == '-')) {
            char left[MAX_LINE], right[MAX_LINE];
            strncpy(left, str, p - str);
            left[p - str] = 0;
            strcpy(right, p + 1);

            double a = eval_expr(left);
            double b = eval_expr(right);

            if (*p == '+') return a + b;
            if (*p == '-') return a - b;
        }
    }

    depth = 0;
    for (char *p = str + strlen(str) - 1; p >= str; p--) {
        if (*p == ')') depth++;
        else if (*p == '(') depth--;
        else if (depth == 0 && (*p == '*' || *p == '/')) {
            char left[MAX_LINE], right[MAX_LINE];
            strncpy(left, str, p - str);
            left[p - str] = 0;
            strcpy(right, p + 1);

            double a = eval_expr(left);
            double b = eval_expr(right);

            if (*p == '*') return a * b;
            if (*p == '/') {
                if (b == 0) error("division by zero", NULL);
                return a / b;
            }
        }
    }

    return eval_term(str);
}

// Helper: evaluate expression and return type info
typedef struct {
    double num_val;
    char str_val[256];
    ValType type;
} EvalResult;

EvalResult eval_typed_expr(const char *s) {
    EvalResult result;
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    // Check if this is JUST a string literal (single quotes pair, nothing else)
    if (*str == '"') {
        // Count quotes to see if we have more than one pair
        int quote_count = 0;
        int escaped = 0;
        for (char *p = str; *p; p++) {
            if (*p == '\\' && !escaped) {
                escaped = 1;
                continue;
            }
            if (*p == '"' && !escaped) {
                quote_count++;
            }
            escaped = 0;
        }
        
        // If exactly 2 quotes (one pair) and starts/ends with quote, it's a pure literal
        if (quote_count == 2 && str[strlen(str) - 1] == '"') {
            result.type = V_STR;
            strncpy(result.str_val, str + 1, strlen(str) - 2);
            result.str_val[strlen(str) - 2] = 0;
            result.num_val = 0;
            return result;
        }
    }
    
    // Check for variable that might be a string (simple identifier only)
    if (!strchr(str, ' ') && !strchr(str, '\t') && !strchr(str, '(') && 
        !strchr(str, '+') && !strchr(str, '-') && !strchr(str, '*') && 
        !strchr(str, '/') && !strchr(str, '<') && !strchr(str, '>') && 
        !strchr(str, '=') && !strchr(str, '!') && !strchr(str, '"')) {
        Var v;
        if (get_var(str, &v) && v.type == V_STR) {
            result.type = V_STR;
            strcpy(result.str_val, v.str);
            result.num_val = 0;
            return result;
        }
    }
    
    // Otherwise it's a numeric expression
    result.type = V_NUM;
    result.num_val = eval_condition(str);
    result.str_val[0] = 0;
    return result;
}

// LEVEL 3: eval_comparison — comparison operators (== != < > <= >=)
double eval_comparison(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);

    int depth = 0;
    int in_string = 0;
    int found_comparison = 0;
    
    for (char *p = str; *p; p++) {
        // Track string literals
        if (*p == '"' && (p == str || p[-1] != '\\')) {
            in_string = !in_string;
            continue;
        }
        
        // Skip if inside string
        if (in_string) continue;
        
        if (*p == '(') depth++;
        else if (*p == ')') depth--;
        else if (depth == 0) {
            if (p[1] && ((p[0] == '=' && p[1] == '=') ||
                         (p[0] == '!' && p[1] == '=') ||
                         (p[0] == '<' && p[1] == '=') ||
                         (p[0] == '>' && p[1] == '='))) {
                
                if (found_comparison)
                    error("chained comparisons not allowed", NULL);
                found_comparison = 1;
                
                char op[3] = {p[0], p[1], 0};
                char left[MAX_LINE], right[MAX_LINE];
                strncpy(left, str, p - str);
                left[p - str] = 0;
                strcpy(right, p + 2);
                
                EvalResult lval = eval_typed_expr(trim(left));
                EvalResult rval = eval_typed_expr(trim(right));
                
                // STRICT TYPE CHECKING for comparisons
                if (lval.type != rval.type) {
                    error("cannot compare different types (number vs string)", NULL);
                }
                
                if (lval.type == V_STR) {
                    // String comparison
                    int cmp = strcmp(lval.str_val, rval.str_val);
                    if (!strcmp(op, "==")) return (cmp == 0) ? 1 : 0;
                    if (!strcmp(op, "!=")) return (cmp != 0) ? 1 : 0;
                    if (!strcmp(op, "<=")) return (cmp <= 0) ? 1 : 0;
                    if (!strcmp(op, ">=")) return (cmp >= 0) ? 1 : 0;
                } else {
                    // Numeric comparison
                    double a = lval.num_val;
                    double b = rval.num_val;
                    
                    if (!strcmp(op, "==")) return (a == b) ? 1 : 0;
                    if (!strcmp(op, "!=")) return (a != b) ? 1 : 0;
                    if (!strcmp(op, "<=")) return (a <= b) ? 1 : 0;
                    if (!strcmp(op, ">=")) return (a >= b) ? 1 : 0;
                }
            }
            else if (*p == '<' || *p == '>') {
                if (found_comparison)
                    error("chained comparisons not allowed", NULL);
                found_comparison = 1;
                
                char op = *p;
                char left[MAX_LINE], right[MAX_LINE];
                strncpy(left, str, p - str);
                left[p - str] = 0;
                strcpy(right, p + 1);
                
                EvalResult lval = eval_typed_expr(trim(left));
                EvalResult rval = eval_typed_expr(trim(right));
                
                // STRICT TYPE CHECKING for comparisons
                if (lval.type != rval.type) {
                    error("cannot compare different types (number vs string)", NULL);
                }
                
                if (lval.type == V_STR) {
                    // String comparison
                    int cmp = strcmp(lval.str_val, rval.str_val);
                    if (op == '<') return (cmp < 0) ? 1 : 0;
                    if (op == '>') return (cmp > 0) ? 1 : 0;
                } else {
                    // Numeric comparison
                    double a = lval.num_val;
                    double b = rval.num_val;
                    
                    if (op == '<') return (a < b) ? 1 : 0;
                    if (op == '>') return (a > b) ? 1 : 0;
                }
            }
        }
    }
    
    return eval_expr(str);
}

// LEVEL 2: eval_and — logical AND with short-circuit
double eval_and(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    int depth = 0;
    
    for (char *p = str; *p; p++) {
        if (*p == '(') depth++;
        else if (*p == ')') depth--;
        else if (depth == 0 && p[0] == 'a' && p[1] == 'n' && p[2] == 'd' &&
                 (p[3] == ' ' || p[3] == '\t' || p[3] == '(' || p[3] == '\0')) {
            
            char left[MAX_LINE], right[MAX_LINE];
            strncpy(left, str, p - str);
            left[p - str] = 0;
            strcpy(right, p + 3);
            
            double lval = eval_and(trim(left));
            if (lval == 0) return 0;
            
            double rval = eval_and(trim(right));
            return (rval != 0) ? 1 : 0;
        }
    }
    
    return eval_comparison(str);
}

// LEVEL 1: eval_or — logical OR with short-circuit
double eval_or(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    int depth = 0;
    
    for (char *p = str; *p; p++) {
        if (*p == '(') depth++;
        else if (*p == ')') depth--;
        else if (depth == 0 && p[0] == 'o' && p[1] == 'r' &&
                 (p[2] == ' ' || p[2] == '\t' || p[2] == '(' || p[2] == '\0')) {
            
            char left[MAX_LINE], right[MAX_LINE];
            strncpy(left, str, p - str);
            left[p - str] = 0;
            strcpy(right, p + 2);
            
            double lval = eval_or(trim(left));
            if (lval != 0) return 1;
            
            double rval = eval_or(trim(right));
            return (rval != 0) ? 1 : 0;
        }
    }
    
    return eval_and(str);
}

// LEVEL 0: eval_condition — entry point for all condition/expression evaluation
double eval_condition(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    if (*str == '\0') {
        return 0;
    }
    
    return eval_or(str);
}

// ================= CONTROL FLOW EXECUTION =================

void exec_while_loop(int i) {
    int base_indent = program[i].indent;
    
    char buf[MAX_LINE];
    strcpy(buf, program[i].text);
    char *stmt = trim(buf);
    
    if (strncmp(stmt, "while", 5) != 0)
        error("expected while statement", NULL);
    
    char *cond_start = stmt + 5;
    while (*cond_start == ' ' || *cond_start == '\t') cond_start++;
    
    if (*cond_start != '(')
        error("while requires parentheses around condition", NULL);
    
    char *cond_end = strchr(cond_start, ')');
    if (!cond_end)
        error("unmatched parentheses in while condition", NULL);
    
    char *colon = cond_end + 1;
    while (*colon == ' ' || *colon == '\t') colon++;
    
    if (*colon != ':')
        error("missing colon after while condition", NULL);
    
    char condition[MAX_LINE];
    strncpy(condition, cond_start + 1, cond_end - cond_start - 1);
    condition[cond_end - cond_start - 1] = 0;
    
    int block_start = i + 1;
    int block_end = find_block_end(i);
    
    // Push loop scope
    push_scope(1);
    
    while (eval_condition(condition) != 0) {
        did_break = 0;
        did_continue = 0;
        
        int j = block_start;
        while (j < block_end) {
            char tmp[MAX_LINE];
            strcpy(tmp, program[j].text);
            char *s = trim(tmp);
            
            if (program[j].indent == base_indent &&
                (!strncmp(s, "if", 2) || !strncmp(s, "while", 5) || 
                 !strncmp(s, "for", 3) || !strncmp(s, "def", 3))) {
                break;
            }
            
            current_line = program[j].line_num;
            
            if (!strncmp(s, "if", 2) && (s[2] == ' ' || s[2] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            if (!strncmp(s, "while", 5) && (s[5] == ' ' || s[5] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            if (!strncmp(s, "for", 3) && (s[3] == ' ' || s[3] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            exec_statement(s);
            if (did_return || did_break || did_continue) break;
            j++;
        }
        
        if (did_return || did_break) break;
    }
    
    // Pop loop scope
    pop_scope();
    
    // Clear break flag (but not return)
    did_break = 0;
}

void exec_for_loop(int i) {
    int base_indent = program[i].indent;
    
    char buf[MAX_LINE];
    strcpy(buf, program[i].text);
    char *stmt = trim(buf);
    
    if (strncmp(stmt, "for", 3) != 0)
        error("expected for statement", NULL);
    
    char *paren_start = stmt + 3;
    while (*paren_start == ' ' || *paren_start == '\t') paren_start++;
    
    if (*paren_start != '(')
        error("for requires parentheses", NULL);
    
    char *paren_end = strrchr(paren_start, ')');
    if (!paren_end)
        error("unmatched parentheses in for loop", NULL);
    
    char *colon = paren_end + 1;
    while (*colon == ' ' || *colon == '\t') colon++;
    if (*colon != ':')
        error("missing colon after for loop", NULL);
    
    char for_clause[MAX_LINE];
    strncpy(for_clause, paren_start + 1, paren_end - paren_start - 1);
    for_clause[paren_end - paren_start - 1] = 0;
    trim(for_clause);
    
    char *sem1 = strchr(for_clause, ';');
    if (!sem1) error("for loop requires semicolons", NULL);
    *sem1 = 0;
    
    char *sem2 = strchr(sem1 + 1, ';');
    if (!sem2) error("for loop requires two semicolons", NULL);
    *sem2 = 0;
    
    char init_buf[MAX_LINE], cond_buf[MAX_LINE], step_buf[MAX_LINE];
    strcpy(init_buf, trim(for_clause));
    strcpy(cond_buf, trim(sem1 + 1));
    strcpy(step_buf, trim(sem2 + 1));
    
    char *init = init_buf;
    char *condition = cond_buf;
    char *step = step_buf;
    
    // Push loop scope
    push_scope(1);
    
    if (*init != '\0') {
        current_line = program[i].line_num;
        exec_statement(init);
        if (did_return) {
            pop_scope();
            return;
        }
    }
    
    int block_start = i + 1;
    int block_end = find_block_end(i);
    
    while (*condition == '\0' || eval_condition(condition) != 0) {
        did_break = 0;
        did_continue = 0;
        
        int j = block_start;
        while (j < block_end) {
            char tmp[MAX_LINE];
            strcpy(tmp, program[j].text);
            char *s = trim(tmp);
            
            if (program[j].indent == base_indent &&
                (!strncmp(s, "if", 2) || !strncmp(s, "while", 5) || 
                 !strncmp(s, "for", 3) || !strncmp(s, "def", 3))) {
                break;
            }
            
            current_line = program[j].line_num;
            
            if (!strncmp(s, "if", 2) && (s[2] == ' ' || s[2] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            if (!strncmp(s, "while", 5) && (s[5] == ' ' || s[5] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            if (!strncmp(s, "for", 3) && (s[3] == ' ' || s[3] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            exec_statement(s);
            if (did_return || did_break || did_continue) break;
            j++;
        }
        
        if (did_return || did_break) break;
        
        if (*step != '\0' && !did_continue) {
            current_line = program[i].line_num;
            char step_copy[MAX_LINE];
            strcpy(step_copy, step);
            exec_statement(step_copy);
            if (did_return) break;
        } else if (did_continue && *step != '\0') {
            // Execute step even on continue
            current_line = program[i].line_num;
            char step_copy[MAX_LINE];
            strcpy(step_copy, step);
            exec_statement(step_copy);
            if (did_return) break;
        }
    }
    
    // Pop loop scope
    pop_scope();
    
    did_break = 0;
}

void exec_for_in_range_loop(int i) {
    int base_indent = program[i].indent;
    
    char buf[MAX_LINE];
    strcpy(buf, program[i].text);
    char *stmt = trim(buf);
    
    if (strncmp(stmt, "for", 3) != 0)
        error("expected for statement", NULL);
    
    char *after_for = stmt + 3;
    while (*after_for == ' ' || *after_for == '\t') after_for++;
    
    char *in_keyword = strstr(after_for, " in ");
    if (!in_keyword) in_keyword = strstr(after_for, "\tin");
    if (!in_keyword) error("for-in loop requires 'in' keyword", NULL);
    
    char loop_var[64];
    strncpy(loop_var, after_for, in_keyword - after_for);
    loop_var[in_keyword - after_for] = 0;
    trim(loop_var);
    
    char *range_start = strstr(in_keyword, "range");
    if (!range_start) error("for-in requires range()", NULL);
    
    char *paren = strchr(range_start, '(');
    if (!paren) error("range requires parentheses", NULL);
    
    char *range_end = strchr(paren, ')');
    if (!range_end) error("unmatched parentheses in range()", NULL);
    
    char range_arg[MAX_LINE];
    strncpy(range_arg, paren + 1, range_end - paren - 1);
    range_arg[range_end - paren - 1] = 0;
    trim(range_arg);
    
    if (*range_arg == '\0')
        error("range() requires an argument", NULL);
    
    char *colon = range_end + 1;
    while (*colon == ' ' || *colon == '\t') colon++;
    if (*colon != ':')
        error("missing colon after for-in loop", NULL);
    
    double n = eval_condition(range_arg);
    if (n < 0) n = 0;
    
    int block_start = i + 1;
    int block_end = find_block_end(i);
    
    // Push loop scope
    push_scope(1);
    
    for (double idx = 0; idx < n; idx += 1) {
        Var v = {0};
        strcpy(v.name, loop_var);
        v.type = V_NUM;
        v.num = idx;
        v.explicit = 0;
        v.is_const = 0;
        set_var(v);
        
        if (did_return) break;
        
        did_break = 0;
        did_continue = 0;
        
        int j = block_start;
        while (j < block_end) {
            char tmp[MAX_LINE];
            strcpy(tmp, program[j].text);
            char *s = trim(tmp);
            
            if (program[j].indent == base_indent &&
                (!strncmp(s, "if", 2) || !strncmp(s, "while", 5) || 
                 !strncmp(s, "for", 3) || !strncmp(s, "def", 3))) {
                break;
            }
            
            current_line = program[j].line_num;
            
            if (!strncmp(s, "if", 2) && (s[2] == ' ' || s[2] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            if (!strncmp(s, "while", 5) && (s[5] == ' ' || s[5] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            if (!strncmp(s, "for", 3) && (s[3] == ' ' || s[3] == '\t')) {
                exec_statement(s);
                if (did_return || did_break || did_continue) break;
                j = find_block_end(j);
                continue;
            }
            
            exec_statement(s);
            if (did_return || did_break || did_continue) break;
            j++;
        }
        
        if (did_return || did_break) break;
    }
    
    // Pop loop scope
    pop_scope();
    
    did_break = 0;
}

void exec_if_block(int i) {
    int base_indent = program[i].indent;
    
    char buf[MAX_LINE];
    strcpy(buf, program[i].text);
    char *stmt = trim(buf);
    
    if (strncmp(stmt, "if", 2) != 0)
        error("expected if statement", NULL);
    
    char *cond_start = stmt + 2;
    while (*cond_start == ' ' || *cond_start == '\t') cond_start++;
    
    char *colon = strchr(cond_start, ':');
    if (!colon) error("missing colon after if condition", NULL);
    *colon = 0;
    
    double cond = eval_condition(trim(cond_start));
    
    int block_start = i + 1;
    int block_end = find_block_end(i);
    
    int executed = 0;
    
    if (cond != 0) {
        int j = block_start;
        while (j < block_end) {
            char tmp[MAX_LINE];
            strcpy(tmp, program[j].text);
            char *s = trim(tmp);
            
            if (program[j].indent == base_indent &&
                (!strncmp(s, "elif", 4) || !strncmp(s, "else", 4))) {
                break;
            }
            
            current_line = program[j].line_num;
            exec_statement(s);
            if (did_return) return;
            j++;
        }
        executed = 1;
    }
    
    int j = block_start;
    while (j < block_end && !executed) {
        char tmp[MAX_LINE];
        strcpy(tmp, program[j].text);
        char *s = trim(tmp);
        
        if (program[j].indent == base_indent && !strncmp(s, "elif", 4)) {
            if (s[4] != ' ' && s[4] != '\t' && s[4] != ':')
                error("invalid elif syntax", NULL);
            
            char *elif_cond = s + 4;
            while (*elif_cond == ' ' || *elif_cond == '\t') elif_cond++;
            
            char *elif_colon = strchr(elif_cond, ':');
            if (!elif_colon) error("missing colon after elif condition", NULL);
            *elif_colon = 0;
            
            current_line = program[j].line_num;
            double elif_val = eval_condition(trim(elif_cond));
            
            if (elif_val != 0) {
                int k = j + 1;
                while (k < block_end) {
                    char tmp2[MAX_LINE];
                    strcpy(tmp2, program[k].text);
                    char *s2 = trim(tmp2);
                    
                    if (program[k].indent == base_indent &&
                        (!strncmp(s2, "elif", 4) || !strncmp(s2, "else", 4))) {
                        break;
                    }
                    
                    current_line = program[k].line_num;
                    exec_statement(s2);
                    if (did_return) return;
                    k++;
                }
                executed = 1;
                break;
            }
        }
        else if (program[j].indent == base_indent && !strncmp(s, "else", 4)) {
            if (s[4] != ':') {
                if (s[4] == ' ' || s[4] == '\t') {
                    char *after_else = s + 4;
                    while (*after_else == ' ' || *after_else == '\t') after_else++;
                    if (!strncmp(after_else, "if", 2))
                        error("use 'elif' instead of 'else if'", NULL);
                }
                error("missing colon after else", NULL);
            }
            
            int k = j + 1;
            while (k < block_end) {
                char tmp2[MAX_LINE];
                strcpy(tmp2, program[k].text);
                char *s2 = trim(tmp2);
                
                if (program[k].indent == base_indent &&
                    (!strncmp(s2, "elif", 4) || !strncmp(s2, "else", 4))) {
                    error("elif/else after else", NULL);
                }
                
                current_line = program[k].line_num;
                exec_statement(s2);
                if (did_return) return;
                k++;
            }
            break;
        }
        
        j++;
    }
}

// ================= STATEMENT EXECUTION =================

void exec_statement(char *t) {
    if (!*t || *t == '#') return;

    if (!strncmp(t, "elif", 4) || !strncmp(t, "else", 4)) {
        return;
    }

    // Handle 'break' statement
    if (!strcmp(t, "break")) {
        if (!inside_loop())
            error("'break' used outside of loop", NULL);
        did_break = 1;
        return;
    }

    // Handle 'continue' statement
    if (!strcmp(t, "continue")) {
        if (!inside_loop())
            error("'continue' used outside of loop", NULL);
        did_continue = 1;
        return;
    }

    if (!strncmp(t, "while", 5) && (t[5] == ' ' || t[5] == '\t')) {
        for (int i = 0; i < line_count; i++) {
            if (program[i].line_num == current_line) {
                exec_while_loop(i);
                return;
            }
        }
        error("while statement not found in program", NULL);
        return;
    }

    if (!strncmp(t, "for", 3) && (t[3] == ' ' || t[3] == '\t')) {
        char *in_keyword = strstr(t, " in ");
        if (!in_keyword) in_keyword = strstr(t, "\tin");
        
        for (int i = 0; i < line_count; i++) {
            if (program[i].line_num == current_line) {
                if (in_keyword) {
                    exec_for_in_range_loop(i);
                } else {
                    exec_for_loop(i);
                }
                return;
            }
        }
        error("for statement not found in program", NULL);
        return;
    }

    if (!strncmp(t, "if", 2) && (t[2] == ' ' || t[2] == '\t')) {
        for (int i = 0; i < line_count; i++) {
            if (program[i].line_num == current_line) {
                exec_if_block(i);
                return;
            }
        }
        error("if statement not found in program", NULL);
        return;
    }

    if (!strncmp(t, "return", 6) && (t[6] == ' ' || t[6] == '\t' || t[6] == '\0')) {
        if (call_depth == 0)
            error("return outside function", NULL);
        
        if (t[6] == '\0') {
            return_value = 0;
        } else {
            return_value = eval_condition(trim(t + 6));
        }
        did_return = 1;
        return;
    }

    // Handle 'print()' function - NOW REQUIRES PARENTHESES
    if (!strncmp(t, "print", 5)) {
        // Check what comes after 'print'
        if (t[5] == ' ' || t[5] == '\t') {
            // Old syntax: print x
            error("print requires parentheses: use print(...)", NULL);
        }
        
        if (t[5] == '(') {
            // New syntax: print(...)
            char *lp = t + 5;
            char *rp = strrchr(t, ')');
            if (!rp) error("unmatched parentheses in print()", NULL);
            
            char args_str[MAX_LINE];
            strncpy(args_str, lp + 1, rp - lp - 1);
            args_str[rp - lp - 1] = 0;
            
            // Handle empty print() - prints blank line
            if (*trim(args_str) == '\0') {
                printf("\n");
                return;
            }
            
            // Parse comma-separated arguments
            char *p = args_str;
            int first = 1;
            
            while (*p) {
                // Find next comma (respecting parentheses and quotes)
                int depth = 0;
                int in_string = 0;
                char *start = p;
                
                while (*p) {
                    if (*p == '"' && (p == start || p[-1] != '\\')) {
                        in_string = !in_string;
                    }
                    if (!in_string) {
                        if (*p == '(') depth++;
                        else if (*p == ')') depth--;
                        else if (*p == ',' && depth == 0) break;
                    }
                    p++;
                }
                
                char arg[MAX_LINE];
                strncpy(arg, start, p - start);
                arg[p - start] = 0;
                trim(arg);
                
                // Print space between arguments
                if (!first) printf(" ");
                first = 0;
                
                // Check if it's a string literal
                if (*arg == '"' && arg[strlen(arg) - 1] == '"') {
                    arg[strlen(arg) - 1] = 0;
                    printf("%s", arg + 1);
                } else {
                    // Evaluate as expression and check type
                    EvalResult res = eval_typed_expr(arg);
                    if (res.type == V_STR) {
                        printf("%s", res.str_val);
                    } else {
                        printf("%g", res.num_val);
                    }
                }
                
                if (*p == ',') p++;
            }
            
            printf("\n");
            return;
        }
        
        if (t[5] == '\0') {
            // Just "print" with nothing after
            error("print requires parentheses: use print(...)", NULL);
        }
    }

    // Handle variable declaration with type or const
    // Patterns: int x = ..., double x = ..., string x = ..., const x = ..., const int x = ...
    int is_const = 0;
    int is_typed = 0;
    ValType declared_type = V_NUM;
    char *decl_start = t;
    
    // Check for 'const'
    if (!strncmp(t, "const", 5) && (t[5] == ' ' || t[5] == '\t')) {
        is_const = 1;
        decl_start = trim(t + 5);
    }
    
    // Check for type
    if (!strncmp(decl_start, "int", 3) && (decl_start[3] == ' ' || decl_start[3] == '\t')) {
        is_typed = 1;
        declared_type = V_NUM;
        decl_start = trim(decl_start + 3);
    } else if (!strncmp(decl_start, "double", 6) && (decl_start[6] == ' ' || decl_start[6] == '\t')) {
        is_typed = 1;
        declared_type = V_NUM;
        decl_start = trim(decl_start + 6);
    } else if (!strncmp(decl_start, "string", 6) && (decl_start[6] == ' ' || decl_start[6] == '\t')) {
        is_typed = 1;
        declared_type = V_STR;
        decl_start = trim(decl_start + 6);
    }
    
    // Handle assignment (including typed/const declarations)
    char *eq = strchr(decl_start, '=');
    if (eq && eq > decl_start) {
        if (eq[-1] == '=' || eq[-1] == '!' || eq[-1] == '<' || eq[-1] == '>') {
            error("invalid statement (comparison is not assignment)", NULL);
        }
        if (eq[1] == '=') {
            error("invalid statement (comparison is not assignment)", NULL);
        }
        
        *eq = 0;
        char *lhs = trim(decl_start);
        char *rhs = trim(eq + 1);

        if (*rhs == '\0') {
            error("empty right-hand side in assignment", lhs);
        }
        
        // Const variables must be initialized
        if (is_const && !get_var(lhs, &(Var){0})) {
            // New const variable - OK
        }

        if (strchr(rhs, '=')) {
            char *eq2 = strchr(rhs, '=');
            if (eq2 > rhs && eq2[-1] != '=' && eq2[-1] != '!' && 
                eq2[-1] != '<' && eq2[-1] != '>' && eq2[1] != '=') {
                error("chained assignment not allowed", NULL);
            }
        }

        Var v = {0};
        strcpy(v.name, lhs);
        v.is_const = is_const;
        v.explicit = is_typed;

        // Check if RHS is a pure string literal (not an expression with strings)
        int is_string_literal = 0;
        if (*rhs == '"') {
            // Count unescaped quotes
            int quote_count = 0;
            int escaped = 0;
            for (char *p = rhs; *p; p++) {
                if (*p == '\\' && !escaped) {
                    escaped = 1;
                    continue;
                }
                if (*p == '"' && !escaped) {
                    quote_count++;
                }
                escaped = 0;
            }
            // Pure literal if exactly 2 quotes and ends with quote
            if (quote_count == 2 && rhs[strlen(rhs) - 1] == '"') {
                is_string_literal = 1;
            }
        }

        if (is_string_literal) {
            if (is_typed && declared_type != V_STR) {
                error("type mismatch: cannot assign string to non-string variable", lhs);
            }
            rhs[strlen(rhs) - 1] = 0;
            strcpy(v.str, rhs + 1);
            v.type = V_STR;
        } else {
            if (is_typed && declared_type != V_NUM) {
                error("type mismatch: cannot assign number to string variable", lhs);
            }
            v.num = eval_condition(rhs);
            v.type = V_NUM;
        }

        set_var(v);
        return;
    }
    
    // Check for const without initialization
    if (is_const) {
        error("const variable must be initialized", NULL);
    }

    // Handle function call
    char *lp = strchr(t, '(');
    if (lp) {
        char fname[64];
        strncpy(fname, t, lp - t);
        fname[lp - t] = 0;
        trim(fname);

        int fi = find_func(fname);
        if (fi != -1) {
            char *rp = strrchr(t, ')');
            if (!rp) error("unmatched parentheses", NULL);

            char args_str[MAX_LINE];
            strncpy(args_str, lp + 1, rp - lp - 1);
            args_str[rp - lp - 1] = 0;

            char args[MAX_ARGS][64];
            int argc = 0;
            char *tok = strtok(args_str, ",");
            while (tok && argc < MAX_ARGS) {
                strcpy(args[argc++], trim(tok));
                tok = strtok(NULL, ",");
            }

            call_function_stmt(&funcs[fi], args, argc);
            return;
        }
    }

    error("unsupported statement", t);
}

// ================= FUNCTION COLLECTION =================

void collect_functions() {
    for (int i = 0; i < line_count; i++) {
        char buf[MAX_LINE];
        strcpy(buf, program[i].text);
        char *t = trim(buf);

        if (!strncmp(t, "def", 3)) {
            Func *f = &funcs[func_count++];
            char *p = strchr(t, '(');
            *p = 0;
            strcpy(f->name, trim(t + 3));

            char *q = strchr(p + 1, ')');
            *q = 0;

            f->param_count = 0;
            char *tok = strtok(p + 1, ",");
            while (tok)
                strcpy(f->params[f->param_count++], trim(tok)),
                tok = strtok(NULL, ",");

            f->start = i + 1;
            f->end = find_block_end(i);
        }
    }
}

// ================= MAIN =================

int main(int argc, char **argv) {
    if (argc == 2 && !strcmp(argv[1], "--version")) {
        printf("Vyom v%s\n", VYOM_VERSION);
        return 0;
    }

    if (argc < 2) {
        printf("Usage: %s <file.vy>\n", argv[0]);
        return 1;
    }

    // Store filename for __file__ introspection
    strncpy(current_filename, argv[1], sizeof(current_filename) - 1);
    current_filename[sizeof(current_filename) - 1] = 0;

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Cannot open file\n");
        return 1;
    }

    char buf[MAX_LINE];
    int ln = 1;
    while (fgets(buf, sizeof(buf), f)) {
        int ind = get_indent(buf);
        strcpy(program[line_count].text, buf + ind);
        program[line_count].indent = ind;
        program[line_count].line_num = ln++;
        line_count++;
    }
    fclose(f);

    collect_functions();

    for (int i = 0; i < line_count; i++) {
        current_line = program[i].line_num;
        char tmp[MAX_LINE];
        strcpy(tmp, program[i].text);
        char *stmt = trim(tmp);
        
        if (*stmt == '\0' || *stmt == '#') {
            continue;
        }
        
        if (!strncmp(stmt, "def", 3)) {
            i = find_block_end(i) - 1;
            continue;
        }
        
        if (!strncmp(stmt, "if", 2) && (stmt[2] == ' ' || stmt[2] == '\t')) {
            exec_statement(stmt);
            i = find_block_end(i) - 1;
            continue;
        }
        
        if (!strncmp(stmt, "while", 5) && (stmt[5] == ' ' || stmt[5] == '\t')) {
            exec_statement(stmt);
            i = find_block_end(i) - 1;
            continue;
        }
        
        if (!strncmp(stmt, "for", 3) && (stmt[3] == ' ' || stmt[3] == '\t')) {
            exec_statement(stmt);
            i = find_block_end(i) - 1;
            continue;
        }
        
        exec_statement(stmt);
    }
    return 0;
}