// Vyom Programming Language
// Created by Sanket Bharadwaj
// Vyom v0.4.3 — Stable (Functions, Calls, Return, Local Scope, NO SHADOWING)

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
#define VYOM_VERSION "Vyom v0.4.3 (stable, no shadowing)"

// ================= TYPES =================

typedef enum { V_NUM, V_STR } ValType;

typedef struct {
    char name[64];
    ValType type;
    double num;
    char str[256];
    int explicit;
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

int current_line = 0;

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
    printf("Line %d: Error: %s", current_line, msg);
    if (name) printf(" '%s'", name);
    printf("\n");
    exit(1);
}

// ================= VARIABLES =================

int find_global(const char *name) {
    for (int i = global_count - 1; i >= 0; i--)
        if (!strcmp(globals[i].name, name)) return i;
    return -1;
}

int find_local(const char *name) {
    if (call_depth == 0) return -1;
    CallFrame *f = &call_stack[call_depth - 1];
    for (int i = f->local_count - 1; i >= 0; i--)
        if (!strcmp(f->locals[i].name, name)) return i;
    return -1;
}

int get_var(const char *name, Var *out) {
    int li = find_local(name);
    if (li != -1) {
        *out = call_stack[call_depth - 1].locals[li];
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
    int gi = find_global(v.name);
    int li = find_local(v.name);

    // ❌ SHADOWING NOT ALLOWED
    if (call_depth > 0 && gi != -1 && li == -1) {
        error("variable shadows global variable", v.name);
    }

    if (li != -1) {
        if (call_stack[call_depth - 1].locals[li].explicit &&
            call_stack[call_depth - 1].locals[li].type != v.type) {
            error("cannot change type of variable", v.name);
        }
        call_stack[call_depth - 1].locals[li] = v;
        return;
    }

    if (call_depth > 0) {
        call_stack[call_depth - 1].locals[
            call_stack[call_depth - 1].local_count++
        ] = v;
        return;
    }

    if (gi != -1) {
        if (globals[gi].explicit && globals[gi].type != v.type)
            error("cannot change type of variable", v.name);
        globals[gi] = v;
    } else {
        globals[global_count++] = v;
    }
}

// ================= EXPRESSIONS =================

int find_func(const char *name) {
    for (int i = 0; i < func_count; i++)
        if (!strcmp(funcs[i].name, name)) return i;
    return -1;
}

double eval_expr(const char *s, int *ok);
void exec_statement(char *t);

// -------- function call (expression context) --------
double call_function_expr(Func *f, char args[MAX_ARGS][64], int argc) {
    if (argc != f->param_count)
        error("incorrect argument count for function", f->name);

    if (call_depth >= MAX_CALL_DEPTH)
        error("call stack overflow", NULL);

    double values[MAX_ARGS];
    for (int i = 0; i < argc; i++) {
        int ok;
        values[i] = eval_expr(args[i], &ok);
    }

    call_stack[call_depth].local_count = 0;
    call_depth++;

    for (int i = 0; i < argc; i++) {
        if (find_global(f->params[i]) != -1)
            error("parameter shadows global variable", f->params[i]);

        Var v = {0};
        strcpy(v.name, f->params[i]);
        v.type = V_NUM;
        v.num = values[i];
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
        int ok;
        values[i] = eval_expr(args[i], &ok);
    }

    call_stack[call_depth].local_count = 0;
    call_depth++;

    for (int i = 0; i < argc; i++) {
        if (find_global(f->params[i]) != -1)
            error("parameter shadows global variable", f->params[i]);

        Var v = {0};
        strcpy(v.name, f->params[i]);
        v.type = V_NUM;
        v.num = values[i];
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

// ================= EXPRESSIONS =================

double eval_term(const char *s, int *ok) {
    *ok = 1;
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);

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

double eval_expr(const char *s, int *ok) {
    *ok = 1;
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);

    int depth = 0;
    for (char *p = str; *p; p++) {
        if (*p == '(') depth++;
        else if (*p == ')') depth--;
        else if (depth == 0 && strchr("+-*/", *p)) {
            char left[MAX_LINE], right[MAX_LINE];
            strncpy(left, str, p - str);
            left[p - str] = 0;
            strcpy(right, p + 1);

            double a = eval_expr(left, ok);
            double b = eval_expr(right, ok);

            if (*p == '+') return a + b;
            if (*p == '-') return a - b;
            if (*p == '*') return a * b;
            if (*p == '/') {
                if (b == 0) error("division by zero", NULL);
                return a / b;
            }
        }
    }
    return eval_term(str, ok);
}

// ================= EXECUTION =================

void exec_statement(char *t) {
    if (!*t || *t == '#') return;

    if (!strncmp(t, "return", 6)) {
        if (call_depth == 0)
            error("return outside function", NULL);
        int ok;
        return_value = eval_expr(t + 6, &ok);
        did_return = 1;
        return;
    }

    if (!strncmp(t, "print", 5)) {
        char *p = trim(t + 5);
        if (*p == '"' && p[strlen(p) - 1] == '"') {
            p[strlen(p) - 1] = 0;
            printf("%s\n", p + 1);
        } else {
            int ok;
            printf("%g\n", eval_expr(p, &ok));
        }
        return;
    }

    char *eq = strchr(t, '=');
    if (eq) {
        *eq = 0;
        char *lhs = trim(t);
        char *rhs = trim(eq + 1);

        Var v = {0};
        strcpy(v.name, lhs);

        if (*rhs == '"' && rhs[strlen(rhs) - 1] == '"') {
            rhs[strlen(rhs) - 1] = 0;
            strcpy(v.str, rhs + 1);
            v.type = V_STR;
        } else {
            int ok;
            v.num = eval_expr(rhs, &ok);
            v.type = V_NUM;
        }

        set_var(v);
        return;
    }

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
        printf("%s\n", VYOM_VERSION);
        return 0;
    }

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
        if (!strncmp(trim(tmp), "def", 3)) {
            i = find_block_end(i) - 1;
            continue;
        }
        exec_statement(trim(tmp));
    }

    return 0;
}