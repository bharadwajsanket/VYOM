# 🌌 Vyom — Language Syntax Guide (v0.5)

Vyom is a small, indentation-based scripting language written in C.
It is designed to be **explicit, predictable, and easy to reason about**.

This document describes the **exact syntax supported in Vyom v0.5**.

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

```vy
if x > 10:
    print 100
elif x > 5:
    print 50
else:
    print 10
```

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

## ❌ Not Supported in v0.5

- strings in expressions
- boolean type
- loops
- arrays / lists
- imports / modules

---

Vyom grows slowly and deliberately.
