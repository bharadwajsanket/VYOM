// Vyom Programming Language
// Created by Sanket Bharadwaj
// Vyom v0.5 — Control Flow + Comparisons (FIXED)

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
#define VYOM_VERSION "Vyom v0.5 (control flow, comparisons, logical ops)"

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
        values[i] = eval_condition(args[i]);
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

// ================= EXPRESSION EVALUATION PIPELINE =================
// Pipeline: eval_condition -> eval_or -> eval_and -> eval_comparison -> eval_expr -> eval_term
// Precedence (low to high): or < and < comparison < arithmetic < term

// LEVEL 5: eval_term — variables, numbers, 'not', function calls
double eval_term(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);

    // Empty expression in term context - return 0 (false) as default
    if (*str == '\0') {
        return 0;
    }

    // Handle parenthesized expressions (unwrap and re-evaluate through pipeline)
    if (*str == '(' && str[strlen(str) - 1] == ')') {
        char inner[MAX_LINE];
        strncpy(inner, str + 1, strlen(str) - 2);
        inner[strlen(str) - 2] = 0;
        return eval_condition(trim(inner));
    }

    // Handle 'not' operator (highest precedence unary operator)
    if (!strncmp(str, "not", 3) && (str[3] == ' ' || str[3] == '\t' || str[3] == '(')) {
        double val = eval_term(trim(str + 3));
        return (val == 0) ? 1 : 0;
    }

    // Check for function call
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

    // Check for variable
    Var v;
    if (get_var(str, &v)) {
        if (v.type != V_NUM) error("not a number", str);
        return v.num;
    }

    // Check for numeric literal
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
    // Scan right-to-left for + and - (lowest precedence)
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

    // Scan right-to-left for * and / (higher precedence)
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

    // No arithmetic operator found, evaluate as term
    return eval_term(str);
}

// LEVEL 3: eval_comparison — comparison operators (== != < > <= >=)
double eval_comparison(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);

    int depth = 0;
    int found_comparison = 0;
    
    // Scan left-to-right for comparison operators
    for (char *p = str; *p; p++) {
        if (*p == '(') depth++;
        else if (*p == ')') depth--;
        else if (depth == 0) {
            // Check for two-character operators first
            if (p[1] && ((p[0] == '=' && p[1] == '=') ||
                         (p[0] == '!' && p[1] == '=') ||
                         (p[0] == '<' && p[1] == '=') ||
                         (p[0] == '>' && p[1] == '='))) {
                
                // Prevent chained comparisons
                if (found_comparison)
                    error("chained comparisons not allowed", NULL);
                found_comparison = 1;
                
                char op[3] = {p[0], p[1], 0};
                char left[MAX_LINE], right[MAX_LINE];
                strncpy(left, str, p - str);
                left[p - str] = 0;
                strcpy(right, p + 2);
                
                double a = eval_expr(trim(left));
                double b = eval_expr(trim(right));
                
                if (!strcmp(op, "==")) return (a == b) ? 1 : 0;
                if (!strcmp(op, "!=")) return (a != b) ? 1 : 0;
                if (!strcmp(op, "<=")) return (a <= b) ? 1 : 0;
                if (!strcmp(op, ">=")) return (a >= b) ? 1 : 0;
            }
            // Check for single-character operators
            else if (*p == '<' || *p == '>') {
                // Prevent chained comparisons
                if (found_comparison)
                    error("chained comparisons not allowed", NULL);
                found_comparison = 1;
                
                char op = *p;
                char left[MAX_LINE], right[MAX_LINE];
                strncpy(left, str, p - str);
                left[p - str] = 0;
                strcpy(right, p + 1);
                
                double a = eval_expr(trim(left));
                double b = eval_expr(trim(right));
                
                if (op == '<') return (a < b) ? 1 : 0;
                if (op == '>') return (a > b) ? 1 : 0;
            }
        }
    }
    
    // No comparison operator found, evaluate as arithmetic expression
    return eval_expr(str);
}

// LEVEL 2: eval_and — logical AND with short-circuit
double eval_and(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    int depth = 0;
    
    // Scan left-to-right for 'and' operator
    for (char *p = str; *p; p++) {
        if (*p == '(') depth++;
        else if (*p == ')') depth--;
        else if (depth == 0 && p[0] == 'a' && p[1] == 'n' && p[2] == 'd' &&
                 (p[3] == ' ' || p[3] == '\t' || p[3] == '(' || p[3] == '\0')) {
            
            char left[MAX_LINE], right[MAX_LINE];
            strncpy(left, str, p - str);
            left[p - str] = 0;
            strcpy(right, p + 3);
            
            // Short-circuit: if left is false, don't evaluate right
            double lval = eval_and(trim(left));
            if (lval == 0) return 0;
            
            double rval = eval_and(trim(right));
            return (rval != 0) ? 1 : 0;
        }
    }
    
    // No 'and' found, evaluate as comparison
    return eval_comparison(str);
}

// LEVEL 1: eval_or — logical OR with short-circuit
double eval_or(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    int depth = 0;
    
    // Scan left-to-right for 'or' operator
    for (char *p = str; *p; p++) {
        if (*p == '(') depth++;
        else if (*p == ')') depth--;
        else if (depth == 0 && p[0] == 'o' && p[1] == 'r' &&
                 (p[2] == ' ' || p[2] == '\t' || p[2] == '(' || p[2] == '\0')) {
            
            char left[MAX_LINE], right[MAX_LINE];
            strncpy(left, str, p - str);
            left[p - str] = 0;
            strcpy(right, p + 2);
            
            // Short-circuit: if left is true, don't evaluate right
            double lval = eval_or(trim(left));
            if (lval != 0) return 1;
            
            double rval = eval_or(trim(right));
            return (rval != 0) ? 1 : 0;
        }
    }
    
    // No 'or' found, evaluate as 'and'
    return eval_and(str);
}

// LEVEL 0: eval_condition — entry point for all condition/expression evaluation
double eval_condition(const char *s) {
    char buf[MAX_LINE];
    strcpy(buf, s);
    char *str = trim(buf);
    
    // Guard: empty expressions evaluate to false (0)
    if (*str == '\0') {
        return 0;
    }
    
    return eval_or(str);
}

// ================= CONTROL FLOW EXECUTION =================

// Execute if/elif/else control flow starting at index i
void exec_if_block(int i) {
    int base_indent = program[i].indent;
    
    // Parse if condition
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
    
    // Find block boundaries
    int block_start = i + 1;
    int block_end = find_block_end(i);
    
    int executed = 0;
    
    // Execute if block if condition is true
    if (cond != 0) {
        int j = block_start;
        while (j < block_end) {
            char tmp[MAX_LINE];
            strcpy(tmp, program[j].text);
            char *s = trim(tmp);
            
            // Stop at elif/else at same indentation
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
    
    // Process elif/else branches
    int j = block_start;
    while (j < block_end && !executed) {
        char tmp[MAX_LINE];
        strcpy(tmp, program[j].text);
        char *s = trim(tmp);
        
        // Check for elif at same indentation
        if (program[j].indent == base_indent && !strncmp(s, "elif", 4)) {
            // Verify it's 'elif', not 'else if'
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
                    
                    // Stop at next elif/else at same indentation
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
        // Check for else at same indentation
        else if (program[j].indent == base_indent && !strncmp(s, "else", 4)) {
            if (s[4] != ':') {
                // Check for 'else if' (not allowed)
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
                
                // else block continues until end or same-level elif/else
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

    // Skip elif/else keywords (handled by exec_if_block)
    if (!strncmp(t, "elif", 4) || !strncmp(t, "else", 4)) {
        return;
    }

    // Handle 'if' statement
    if (!strncmp(t, "if", 2) && (t[2] == ' ' || t[2] == '\t')) {
        // Find current position in program
        for (int i = 0; i < line_count; i++) {
            if (program[i].line_num == current_line) {
                exec_if_block(i);
                return;
            }
        }
        error("if statement not found in program", NULL);
        return;
    }

    // Handle 'return' statement
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

    // Handle 'print' statement
    if (!strncmp(t, "print", 5) && (t[5] == ' ' || t[5] == '\t' || t[5] == '\0')) {
        if (t[5] == '\0') {
            printf("\n");
            return;
        }
        
        char *p = trim(t + 5);
        
        // Handle empty print argument as newline
        if (*p == '\0') {
            printf("\n");
            return;
        }
        
        if (*p == '"' && p[strlen(p) - 1] == '"') {
            p[strlen(p) - 1] = 0;
            printf("%s\n", p + 1);
        } else {
            printf("%g\n", eval_condition(p));
        }
        return;
    }

    // Handle assignment
    char *eq = strchr(t, '=');
    if (eq && eq > t) {
        // Make sure it's not ==, !=, <=, >=
        if (eq[-1] == '=' || eq[-1] == '!' || eq[-1] == '<' || eq[-1] == '>') {
            error("invalid statement (comparison is not assignment)", NULL);
        }
        if (eq[1] == '=') {
            error("invalid statement (comparison is not assignment)", NULL);
        }
        
        *eq = 0;
        char *lhs = trim(t);
        char *rhs = trim(eq + 1);

        // Validate non-empty RHS
        if (*rhs == '\0') {
            error("empty right-hand side in assignment", lhs);
        }

        // Check for chained assignment (a = b = c)
        if (strchr(rhs, '=')) {
            char *eq2 = strchr(rhs, '=');
            if (eq2 > rhs && eq2[-1] != '=' && eq2[-1] != '!' && 
                eq2[-1] != '<' && eq2[-1] != '>' && eq2[1] != '=') {
                error("chained assignment not allowed", NULL);
            }
        }

        Var v = {0};
        strcpy(v.name, lhs);

        if (*rhs == '"' && rhs[strlen(rhs) - 1] == '"') {
            rhs[strlen(rhs) - 1] = 0;
            strcpy(v.str, rhs + 1);
            v.type = V_STR;
        } else {
            v.num = eval_condition(rhs);
            v.type = V_NUM;
        }

        set_var(v);
        return;
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
        printf("%s\n", VYOM_VERSION);
        return 0;
    }

    if (argc < 2) {
        printf("Usage: %s <file.vy>\n", argv[0]);
        return 1;
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
        char *stmt = trim(tmp);
        
        // Skip empty lines and comments
        if (*stmt == '\0' || *stmt == '#') {
            continue;
        }
        
        // Skip function definitions (block already collected)
        if (!strncmp(stmt, "def", 3)) {
            i = find_block_end(i) - 1;
            continue;
        }
        
        // Execute if/elif/else as compound statement, then skip entire block
        if (!strncmp(stmt, "if", 2) && (stmt[2] == ' ' || stmt[2] == '\t')) {
            exec_statement(stmt);
            i = find_block_end(i) - 1;
            continue;
        }
        
        exec_statement(stmt);
    }
    return 0;
}