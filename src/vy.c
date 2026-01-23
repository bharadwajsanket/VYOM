// Vyom Programming Language
// Created by Sanket Bharadwaj
// Vyom v0.4.1 — Functions Complete (Calls + Return + Local Scope)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 2048
#define MAX_LINE 512
#define MAX_VARS 256
#define MAX_FUNCS 128
#define MAX_CALL_DEPTH 64
#define MAX_ARGS 8

#define VYOM_VERSION "Vyom v0.4.1 (functions complete)"

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

void parse_args(const char *args_str, char args[MAX_ARGS][64], int *argc) {
    *argc = 0;
    if (strlen(args_str) == 0) return;
    
    char temp[MAX_LINE];
    strcpy(temp, args_str);
    
    int paren_depth = 0;
    char *start = temp;
    
    for (char *p = temp; ; p++) {
        if (*p == '(') paren_depth++;
        else if (*p == ')') paren_depth--;
        else if ((*p == ',' && paren_depth == 0) || *p == 0) {
            char save = *p;
            *p = 0;
            strcpy(args[(*argc)++], trim(start));
            if (save == 0) break;
            start = p + 1;
        }
    }
}

// ================= VARIABLES =================
int find_local(const char *name) {
    if (call_depth == 0) return -1;
    CallFrame *f = &call_stack[call_depth - 1];
    for (int i = f->local_count - 1; i >= 0; i--)
        if (!strcmp(f->locals[i].name, name)) return i;
    return -1;
}

int find_global(const char *name) {
    for (int i = global_count - 1; i >= 0; i--)
        if (!strcmp(globals[i].name, name)) return i;
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
    int li = find_local(v.name);
    if (li != -1) {
        if (call_stack[call_depth - 1].locals[li].explicit && 
            call_stack[call_depth - 1].locals[li].type != v.type) {
            printf("Line %d: Type Error: cannot change type of '%s'\n", current_line, v.name);
            return;
        }
        call_stack[call_depth - 1].locals[li] = v;
        return;
    }

    if (call_depth > 0) {
        call_stack[call_depth - 1].locals[call_stack[call_depth - 1].local_count++] = v;
        return;
    }

    int gi = find_global(v.name);
    if (gi != -1) {
        if (globals[gi].explicit && globals[gi].type != v.type) {
            printf("Line %d: Type Error: cannot change type of '%s'\n", current_line, v.name);
            return;
        }
        globals[gi] = v;
    } else {
        globals[global_count++] = v;
    }
}

// ================= EXPRESSIONS =================
double eval_expr(const char *s, int *ok);

int find_func(const char *name) {
    for (int i = 0; i < func_count; i++)
        if (!strcmp(funcs[i].name, name)) return i;
    return -1;
}

void exec_statement(char *t);

double call_function(Func *f, char args[MAX_ARGS][64], int argc, int needs_return) {
    if (argc != f->param_count) {
        printf("Line %d: Type Error: function '%s' expects %d arguments, got %d\n", 
               current_line, f->name, f->param_count, argc);
        return 0;
    }

    if (call_depth >= MAX_CALL_DEPTH) {
        printf("Line %d: Error: call stack overflow\n", current_line);
        return 0;
    }

    // CRITICAL: Evaluate arguments BEFORE creating new call frame
    double arg_values[MAX_ARGS];
    for (int i = 0; i < argc; i++) {
        int ok;
        arg_values[i] = eval_expr(args[i], &ok);
        if (!ok) {
            return 0;
        }
    }

    // NOW create new call frame
    call_stack[call_depth].local_count = 0;
    call_depth++;

    // Set parameters with pre-evaluated values
    for (int i = 0; i < argc; i++) {
        Var v;
        strcpy(v.name, f->params[i]);
        v.explicit = 0;
        v.type = V_NUM;
        v.num = arg_values[i];
        set_var(v);
    }

    return_value = 0;
    did_return = 0;

    for (int i = f->start; i < f->end && !did_return; i++) {
        current_line = program[i].line_num;
        char buf[MAX_LINE];
        strcpy(buf, program[i].text);
        char *t = trim(buf);

        if (*t == '#' || !*t) continue;

        exec_statement(t);
    }

    call_depth--;
    
    if (needs_return && !did_return) {
        printf("Line %d: Error: function '%s' missing return statement\n", current_line, f->name);
        return 0;
    }
    
    did_return = 0;
    return return_value;
}

double eval_term(const char *s, int *ok);

double eval_expr(const char *s, int *ok) {
    *ok = 1;
    
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    char *plus = NULL;
    char *minus = NULL;
    char *mult = NULL;
    char *divide = NULL;
    
    int paren_depth = 0;
    for (char *p = str; *p; p++) {
        if (*p == '(') paren_depth++;
        else if (*p == ')') paren_depth--;
        else if (paren_depth == 0) {
            if (*p == '+') plus = p;
            else if (*p == '-' && p != str) minus = p;
            else if (*p == '*') mult = p;
            else if (*p == '/') divide = p;
        }
    }
    
    if (plus) {
        char left_str[MAX_LINE], right_str[MAX_LINE];
        strncpy(left_str, str, plus - str);
        left_str[plus - str] = 0;
        strcpy(right_str, plus + 1);
        
        double left = eval_expr(left_str, ok);
        if (!*ok) return 0;
        double right = eval_expr(right_str, ok);
        if (!*ok) return 0;
        return left + right;
    }
    
    if (minus) {
        char left_str[MAX_LINE], right_str[MAX_LINE];
        strncpy(left_str, str, minus - str);
        left_str[minus - str] = 0;
        strcpy(right_str, minus + 1);
        
        double left = eval_expr(left_str, ok);
        if (!*ok) return 0;
        double right = eval_expr(right_str, ok);
        if (!*ok) return 0;
        return left - right;
    }
    
    if (mult) {
        char left_str[MAX_LINE], right_str[MAX_LINE];
        strncpy(left_str, str, mult - str);
        left_str[mult - str] = 0;
        strcpy(right_str, mult + 1);
        
        double left = eval_term(left_str, ok);
        if (!*ok) return 0;
        double right = eval_term(right_str, ok);
        if (!*ok) return 0;
        return left * right;
    }
    
    if (divide) {
        char left_str[MAX_LINE], right_str[MAX_LINE];
        strncpy(left_str, str, divide - str);
        left_str[divide - str] = 0;
        strcpy(right_str, divide + 1);
        
        double left = eval_term(left_str, ok);
        if (!*ok) return 0;
        double right = eval_term(right_str, ok);
        if (!*ok) return 0;
        if (right == 0) {
            printf("Line %d: Error: division by zero\n", current_line);
            *ok = 0;
            return 0;
        }
        return left / right;
    }
    
    return eval_term(str, ok);
}

double eval_term(const char *s, int *ok) {
    *ok = 1;
    
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    char *lparen = strchr(str, '(');
    if (lparen) {
        char fname[64];
        int len = lparen - str;
        strncpy(fname, str, len);
        fname[len] = 0;
        trim(fname);
        
        int fi = find_func(fname);
        if (fi != -1) {
            char *q = strrchr(str, ')');
            if (!q) {
                printf("Line %d: Error: unmatched parentheses\n", current_line);
                *ok = 0;
                return 0;
            }
            
            char args_str[MAX_LINE];
            strncpy(args_str, lparen + 1, q - lparen - 1);
            args_str[q - lparen - 1] = 0;
            
            char args[MAX_ARGS][64];
            int ac;
            parse_args(trim(args_str), args, &ac);

            return call_function(&funcs[fi], args, ac, 1);
        }
    }

    Var v;
    if (get_var(str, &v)) {
        if (v.type == V_NUM) return v.num;
        printf("Line %d: Type Error: '%s' is not a number\n", current_line, str);
        *ok = 0;
        return 0;
    }
    
    if (isdigit(*str) || (*str == '-' && isdigit(str[1]))) return atof(str);

    printf("Line %d: Name Error: '%s' is not defined\n", current_line, str);
    *ok = 0;
    return 0;
}

// ================= EXECUTION =================
void exec_statement(char *t) {
    if (!strncmp(t, "return", 6)) {
        if (call_depth == 0) {
            printf("Line %d: Error: 'return' outside function\n", current_line);
            return;
        }
        char expr[MAX_LINE];
        strcpy(expr, trim(t + 6));
        int ok;
        return_value = eval_expr(expr, &ok);
        did_return = 1;
        return;
    }

    if (!strncmp(t, "print", 5)) {
        t = trim(t + 5);
        
        if (*t == '"' && t[strlen(t) - 1] == '"') {
            t[strlen(t) - 1] = 0;
            printf("%s\n", t + 1);
            return;
        }
        
        Var v;
        if (get_var(t, &v)) {
            if (v.type == V_STR) printf("%s\n", v.str);
            else printf("%g\n", v.num);
            return;
        }
        
        int ok;
        double val = eval_expr(t, &ok);
        if (ok) printf("%g\n", val);
        return;
    }

    char *eq = strchr(t, '=');
    if (eq) {
        *eq = 0;
        char *lhs = trim(t);
        char *rhs = trim(eq + 1);
        
        Var v;
        v.explicit = 0;
        
        char type[32] = "";
        char name[64] = "";
        
        if (sscanf(lhs, "%31s %63s", type, name) == 2) {
            if (!strcmp(type, "int") || !strcmp(type, "double")) {
                v.explicit = 1;
                v.type = V_NUM;
                strcpy(v.name, name);
            } else if (!strcmp(type, "string")) {
                v.explicit = 1;
                v.type = V_STR;
                strcpy(v.name, name);
            } else {
                strcpy(v.name, lhs);
            }
        } else {
            strcpy(v.name, lhs);
        }

        if (*rhs == '"' && rhs[strlen(rhs) - 1] == '"') {
            if (v.explicit && v.type != V_STR) {
                printf("Line %d: Type Error: cannot assign string to non-string variable\n", current_line);
                return;
            }
            v.type = V_STR;
            rhs[strlen(rhs) - 1] = 0;
            strcpy(v.str, rhs + 1);
        } else {
            if (v.explicit && v.type != V_NUM) {
                printf("Line %d: Type Error: cannot assign number to non-number variable\n", current_line);
                return;
            }
            v.type = V_NUM;
            int ok;
            v.num = eval_expr(rhs, &ok);
            if (!ok) return;
        }
        
        set_var(v);
        return;
    }

    char *lparen = strchr(t, '(');
    if (lparen) {
        char fname[64];
        int len = lparen - t;
        strncpy(fname, t, len);
        fname[len] = 0;
        trim(fname);
        
        int fi = find_func(fname);
        if (fi != -1) {
            char *q = strrchr(t, ')');
            if (!q) return;
            
            char args_str[MAX_LINE];
            strncpy(args_str, lparen + 1, q - lparen - 1);
            args_str[q - lparen - 1] = 0;
            
            char args[MAX_ARGS][64];
            int ac;
            parse_args(trim(args_str), args, &ac);

            call_function(&funcs[fi], args, ac, 0);
            return;
        }
    }
}

void collect_functions() {
    for (int i = 0; i < line_count; i++) {
        char buf[MAX_LINE];
        strcpy(buf, program[i].text);
        char *t = trim(buf);

        if (*t == '#' || !*t) continue;

        if (!strncmp(t, "def", 3)) {
            Func *f = &funcs[func_count++];
            
            char temp[MAX_LINE];
            strcpy(temp, t + 3);
            char *lparen = strchr(temp, '(');
            if (!lparen) continue;
            
            *lparen = 0;
            strcpy(f->name, trim(temp));

            char *q = strchr(lparen + 1, ')');
            if (q) *q = 0;

            f->param_count = 0;
            char args_str[MAX_LINE];
            strcpy(args_str, lparen + 1);
            
            char args[MAX_ARGS][64];
            parse_args(trim(args_str), args, &f->param_count);
            
            for (int j = 0; j < f->param_count; j++) {
                strcpy(f->params[j], args[j]);
            }

            f->start = i + 1;
            f->end = find_block_end(i);
        }
    }
}

void run_top_level() {
    for (int i = 0; i < line_count; i++) {
        current_line = program[i].line_num;
        char buf[MAX_LINE];
        strcpy(buf, program[i].text);
        char *t = trim(buf);

        if (*t == '#' || !*t) continue;

        if (!strncmp(t, "def", 3)) {
            i = find_block_end(i) - 1;
            continue;
        }

        exec_statement(t);
    }
}

// ================= MAIN =================
int main(int argc, char **argv) {
    if (argc == 2 && !strcmp(argv[1], "--version")) {
        printf("%s\n", VYOM_VERSION);
        return 0;
    }

    if (argc < 2) {
        printf("Usage: vyom <file.vy>\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Cannot open file\n");
        return 1;
    }

    char buf[MAX_LINE];
    int line_num = 1;
    while (fgets(buf, sizeof(buf), f)) {
        int ind = get_indent(buf);
        strcpy(program[line_count].text, buf + ind);
        program[line_count].indent = ind;
        program[line_count].line_num = line_num++;
        line_count++;
    }
    fclose(f);

    collect_functions();
    run_top_level();
    
    return 0;
}