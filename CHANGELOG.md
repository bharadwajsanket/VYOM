# 📜 Vyom — Changelog

All notable changes to the **Vyom Programming Language** are documented here.
This file reflects the complete evolution from **v0.1 → v0.9**.

---

# 🚀 v0.9 — 2D Arrays & Multi-Dimensional Data

## Summary

Vyom v0.9 extends the array system with **fixed-size 2D arrays**, enabling grid-based data structures and matrix operations while maintaining the language's commitment to explicit, bounds-checked semantics.

No v0.8 behavior was removed. All existing programs continue to run unchanged.

---

## ✨ New Features

### 1. Fixed-Size 2D Arrays

- Supported element types: `int` (numeric only for 2D)
- Size decided at declaration time: `int a[rows][cols]`
- Row-major contiguous memory layout
- No resizing, no dynamic growth
- Mandatory bounds checking on every access

```vy
int matrix[3][4]
int grid[2][3] = [[1, 2, 3], [4, 5, 6]]
```

---

### 2. 2D Array Element Access

```vy
print(grid[0][0])   # 1
print(grid[1][2])   # 6
```

- Both indices must be integers
- Out-of-bounds → runtime error
- Access pattern: `array[row][col]`

---

### 3. 2D Array Element Assignment

```vy
matrix[0][0] = 100
matrix[1][2] = 42
```

- Index must be integer
- Out-of-bounds → runtime error
- `const` arrays are immutable

---

### 4. Enhanced `len()` for 2D Arrays

```vy
int a[3][4]
len(a)      # 3 (number of rows)
len(a[0])   # 4 (number of columns)
```

- `len(array)` returns row count
- `len(array[i])` returns column count
- Enables dynamic nested loop iteration

---

### 5. Partial Initialization with Zero-Fill

```vy
int c[3][3] = [[1, 2], [5]]
# c[0] = [1, 2, 0]
# c[1] = [5, 0, 0]
# c[2] = [0, 0, 0]
```

- Missing elements are zero-filled
- Missing rows are zero-filled
- Explicit over implicit

---

## 🔒 Stability Guarantee

- No v0.8 behavior was removed
- No syntax changes outside listed features
- No implicit behavior added
- No dynamic memory features exposed

---

# 🚀 v0.8 — Fixed Arrays & Explicit Semantics

## Summary

Vyom v0.8 is a **strict superset** of v0.7 focused on adding **carefully bounded data structures** while preserving the original execution model.

No architectural redesign was performed. All v0.7 programs continue to run unchanged.

This release deliberately adds *power without abstraction*.

---

## ✨ New Features

### 1. Fixed-Size Typed Arrays

- Supported element types:
  - `int`
  - `string`
- Size decided at declaration time
- No resizing, no dynamic growth
- Mandatory bounds checking on every access

```vy
int a[3]
string names[2]
int b[3] = [1, 2, 3]
```

---

### 2. Array Element Assignment

```vy
a[0] = 10
names[1] = "Vyom"
```

- Index must be integer
- Out-of-bounds → runtime error
- `const` arrays are immutable

---

### 3. String Indexing (Read-Only)

```vy
string s = "hello"
print(s[1])   # e
```

- Returns a **string of length 1**
- Index must be integer
- Strings remain immutable

---

### 4. `len()` Builtin

Works only on:
- arrays
- strings

```vy
len(a)
len("test")
```

- `len(number)` → runtime error

---

### 5. Enhanced `range()` Loop

```vy
for i in range(5):
    print(i)

for i in range(2, 10):
    print(i)

for i in range(0, 10, 2):
    print(i)
```

- Supports 1 / 2 / 3 arguments
- `step ≠ 0` enforced
- Loop variable must be numeric
- Loop variable cannot shadow typed / const variables

---

### 6. `exit(code)` Builtin

```vy
exit()
exit(1)
```

- Immediately terminates program
- Optional numeric exit code
- Frees all allocated array memory before exiting

---

## 🔒 Stability Guarantee

- No v0.7 behavior was removed
- No syntax changes outside listed features
- No implicit behavior added
- No dynamic memory features exposed

---

# 🚀 v0.7 — Strict Core & Developer Quality

## Summary

Vyom v0.7 focused on **semantic clarity, safety, and consistency**.

---

## 🔥 Breaking Changes

### `print()` Requires Parentheses

**OLD (≤ v0.6):**
```vy
print "hello"
```

**NEW (v0.7):**
```vy
print("hello")
print("x =", x)
print()
```

---

## ✨ v0.7 Features

- Multi-argument `print()`
- `break` and `continue`
- Loop-local scope
- `const` variables
- Strict type comparisons
- `__version__`, `__file__` introspection

---

## 📦 v0.6 — Loops & Iteration
- `while`
- C-style `for`
- `for-in range`

---

## 🎯 v0.5 — Control Flow
- `if / elif / else`
- Boolean logic

---

## 🧠 v0.4 — Safety
- Runtime validation
- Call stack enforcement

---

## 🧩 v0.3 — Functions

---

## 🧮 v0.2 — Expressions

---

## 🌱 v0.1 — Core Interpreter

---

**Vyom is designed to be understood, not rushed.**
