# 🌌 Vyom — Language Syntax Guide (v0.8)

Vyom is a small, indentation-based scripting language written in C.  
It is designed to be **explicit, predictable, and easy to reason about**.

This document describes the **exact syntax supported in Vyom v0.8**.

---

## ✨ Core Rules

- Indentation defines blocks (spaces recommended)
- Variables are **case-sensitive**
- Assignment uses `=` (no `set`, no `:=`)
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
- Invalid identifiers raise runtime errors

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
- Arithmetic expressions operate on **numbers only**
- Division by zero raises a runtime error
- Operator precedence is respected
- Parentheses are supported

---

## 🔤 Strings

```vy
s = "hello"
name = "Vyom"
```

Rules:
- Strings are immutable
- Strings are **not** implicitly converted to numbers
- String literals must use double quotes `"`

---

## 🔎 String Indexing (v0.8)

```vy
s = "hello"
print(s[0])   # h
print(s[4])   # o
```

Rules:
- Index must be an integer
- Indexing is **read-only**
- Out-of-bounds access raises a runtime error
- Result is a **string of length 1**

---

## 📦 Arrays (v0.8)

### Declaration

```vy
int a[3]
string names[2]
```

### Initialization

```vy
int nums[3] = [1, 2, 3]
string words[2] = ["hi", "bye"]
```

Rules:
- Arrays are **fixed-size**
- Size must be a positive integer
- Element type is fixed at declaration
- No resizing, no dynamic growth

---

### Array Access & Assignment

```vy
a[0] = 10
print(a[0])
```

Rules:
- Index must be integer
- Bounds checking is mandatory
- Out-of-bounds → runtime error
- `const` arrays cannot be modified

---

## 📏 len() Built-in (v0.8)

```vy
len(a)
len(s)
```

Rules:
- Works only on arrays and strings
- Returns an integer
- Using `len()` on numbers raises an error

---

## 🔍 Comparison Operators

```
>   <   >=   <=   ==   !=
```

Rules:
- Type-safe comparisons
- Result is numeric: `1` true, `0` false

---

## 🔗 Logical Operators

```
and   or   not
```

Short-circuit evaluation.

---

## 🔀 Control Flow

### if / elif / else

```vy
if x > 10:
    print(100)
elif x > 5:
    print(50)
else:
    print(10)
```

---

## 🔄 Loops

### while loop

```vy
i = 0
while (i < 5):
    print(i)
    i = i + 1
```

---

### C-style for loop

```vy
for (i = 0; i < 5; i = i + 1):
    print(i)
```

---

### for-in-range loop

```vy
for i in range(5):
    print(i)

for i in range(1, 10):
    print(i)

for i in range(10, 0, -1):
    print(i)
```

---

## 🖨 print()

```vy
print("Hello")
print("x =", x)
print()
```

---

## 🧠 Functions

```vy
def add(a, b):
    return a + b

print(add(3, 4))
```

---

## 🚪 exit()

```vy
exit()
exit(1)
```

---

## ❌ Not Supported in v0.8

- Dynamic arrays
- Dictionaries
- Garbage collection
- Exceptions
- Classes / objects
