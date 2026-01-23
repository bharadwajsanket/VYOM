# 🌌 Vyom — Language Syntax Guide (v0.4)

Vyom is a small, indentation-based scripting language written in C.
It is designed to be **explicit, predictable, and easy to reason about**.

This document describes the **exact syntax supported in Vyom v0.4**.

---

## ✨ Core Rules

- Indentation defines blocks (spaces recommended)
- Variables are **case-sensitive**
- Assignment uses `=` (no `set`)
- Numbers and strings are dynamically typed
- Execution is top-down and single-pass
- No hidden coercions or implicit behavior

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
- Only numeric expressions are supported
- Strings are **not allowed** inside expressions in v0.4
- Division by zero raises a runtime error

---

## 🖨 print

### Print values

```vy
print x
print 100
print 10 + 5
```

### Print strings

```vy
print "Hello Vyom"
```

Notes:
- `print` is a statement, not a function
- Parentheses are not required

---

## 🧠 Functions

### Define a function

```vy
def greet():
    print 100
```

### Call a function

```vy
greet()
```

### Parameters

```vy
def show(v):
    print v

show(42)
```

### Return values

```vy
def add(a, b):
    return a + b

result = add(3, 4)
print result
```

Rules:
- Functions may take parameters
- Functions may return a numeric value
- A function used inside an expression **must return**
- Functions called as statements may omit `return`

---

## 🧩 Scope Rules

```vy
x = 10

def demo():
    x = 5
    print x

demo()
print x
```

Output:
```
5
10
```

Rules:
- Global variables live at top-level
- Each function call creates a new local scope
- Locals do **not** leak outside functions
- No closures
- Shadowing behavior is explicitly defined by the interpreter

---

## 🔁 Execution Model

- Source file is read line by line
- Function definitions are collected first
- Top-level statements execute sequentially
- Function calls create a new call frame
- On return, the call frame is destroyed

---

## 💬 Comments

```vy
# this is a comment
```

- Comments start with `#`
- Comments are ignored by the interpreter

---

## ⚠️ Errors

Vyom prioritizes **clear, line-numbered errors**:

```
Line 12: Error: name not defined 'x'
Line 23: Error: division by zero
Line 40: Error: function missing return statement
```

Errors stop execution immediately.

---

## ❌ Not Supported in v0.4

These are **intentional limitations**:

- `if / else`
- loops
- strings in expressions
- arrays / lists
- boolean type
- imports / modules

---

## ▶️ Example Program (v0.4)

```vy
x = 10
y = 5

print x
print y

def add(a, b):
    return a + b

print add(3, 4)
```

---

Vyom grows slowly and deliberately.
If a feature cannot be explained cleanly,
it does not belong.
