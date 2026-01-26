# 🌌 Vyom — Language Syntax Guide (v0.6)

Vyom is a small, indentation-based scripting language written in C.
It is designed to be **explicit, predictable, and easy to reason about**.

This document describes the **exact syntax supported in Vyom v0.6**.

---

## ✨ Core Rules

- Indentation defines blocks (spaces recommended)
- Variables are **case-sensitive**
- Assignment uses `=` (no `set`)
- Numbers and strings are dynamically typed
- Execution is top-down and single-pass
- No hidden coercions or implicit behavior
- No virtual machine, no bytecode, no multi-pass execution

---

## 📝 Variables & Assignment

```vy
x = 10
name = "Vyom"
pi = 3.14
```

Rules:
- Variable names must start with a letter or `_`
- May contain letters, digits, `_`
- `A` and `a` are different variables
- Invalid identifiers raise errors

---

## 🧮 Expressions

### Arithmetic Operators

Supported operators:

```
+   -   *   /
```

Examples:

```vy
a = 10 + 5
b = a * 2
c = (a + b) / 3
```

Notes:
- Only **numeric expressions** are supported
- Strings are **not allowed** inside expressions
- Division by zero raises a runtime error

---

## 🔍 Comparison Operators

Supported operators:

```
>   <   >=   <=   ==   !=
```

Rules:
- Comparisons operate **only on numbers**
- Result is always a **number**
  - `1` → true
  - `0` → false

---

## 🔗 Logical Operators

Supported keywords:

```
and   or   not
```

Truth model:
- `0` → false
- non-zero → true

---

## 🔀 Control Flow

### if / elif / else

```vy
if x > 10:
    print 100
elif x > 5:
    print 50
else:
    print 10
```

---

## 🔄 Loops (NEW in v0.6)

### while loop

```vy
i = 0
while (i < 5):
    print i
    i = i + 1
```

Rules:
- Condition must be in **parentheses**
- Colon required after closing parenthesis
- Evaluates condition each iteration
- Loop body must be indented

### C-style for loop

```vy
for (i = 0; i < 5; i = i + 1):
    print i
```

Rules:
- Three expressions separated by **semicolons**: `init; condition; step`
- All three parts must be present (but can be empty)
- Condition evaluated at start of each iteration
- Step executed after each iteration body
- Mandatory parentheses and colon

### for-in-range loop

```vy
for i in range(5):
    print i
```

Rules:
- Syntactic sugar for counting from 0 to n-1
- Variable starts at 0, increments by 1
- `range(n)` always produces: 0, 1, 2, ..., n-1
- Equivalent to: `for (i = 0; i < n; i = i + 1):`

---

### Loop Constraints

- No `break` or `continue` statements
- Loops can contain `if/elif/else`
- Loops can be nested
- `return` statements exit function (including from inside loops)
- Loop variables do not shadow globals (Vyom naming rule)

---

## 🖨 print

```vy
print x
print 10 + 5
print "Hello Vyom"
```

---

## 🧠 Functions

```vy
def add(a, b):
    return a + b

print add(3, 4)
```

Rules:
- Functions return numeric values only
- Functions used in expressions must return

---

## 🧩 Scope Rules

- Each function call creates a new local scope
- No closures
- No implicit shadowing

---

## ⚠️ Errors

Errors are line-numbered and fatal.

---

## ❌ Not Supported in v0.6

- strings in expressions
- boolean type
- break / continue statements
- arrays / lists
- imports / modules
- nested function definitions
- variable shadowing

---

Vyom grows slowly and deliberately.
