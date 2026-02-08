# 🌌 Vyom — Language Syntax Guide (v1.0)

Vyom is a small, indentation-based scripting language written in C.  
It is designed to be **explicit, predictable, and easy to reason about**.

This document describes the **exact syntax supported in Vyom v1.0**.

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
+   -   *   /   //
```

- `/` — floating-point division
- `//` — integer division (truncates toward zero)

Examples:

```vy
a = 10 + 5
b = a * 2
c = (a + b) / 3
d = 7 // 3        # 2
e = -7 // 3       # -2
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

## 📦 1D Arrays (v0.8)

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

## 📐 2D Arrays (v0.9)

### Declaration

```vy
int matrix[3][4]
```

Rules:
- 2D arrays support **numeric types only** (`int`)
- Size is fixed at declaration: `[rows][cols]`
- Row-major contiguous memory layout
- No resizing, no dynamic growth

### Initialization

```vy
int grid[2][3] = [[1, 2, 3], [4, 5, 6]]
```

Rules:
- Outer array contains rows
- Each row is an inner array
- Missing elements are **zero-filled**
- Missing rows are **zero-filled**

### Partial Initialization

```vy
int c[3][3] = [[1, 2], [5]]
# Result:
# c[0] = [1, 2, 0]
# c[1] = [5, 0, 0]
# c[2] = [0, 0, 0]
```

### 2D Array Access & Assignment

```vy
grid[0][0] = 100
print(grid[1][2])
```

Rules:
- Access pattern: `array[row][col]`
- Both indices must be integers
- Bounds checking is mandatory
- Out-of-bounds → runtime error

### 2D Array Dimensions

```vy
int a[3][4]
len(a)      # 3 (number of rows)
len(a[0])   # 4 (number of columns)
```

### Nested Loop Iteration

```vy
for row in range(len(grid)):
    for col in range(len(grid[0])):
        print(grid[row][col])
```

---

## 📏 len() Built-in

```vy
len(a)      # array: returns size (1D) or rows (2D)
len(a[0])   # 2D array: returns columns
len(s)      # string: returns character count
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

## ❌ Not Supported in v1.0

- Dynamic arrays
- 2D string arrays (only numeric 2D supported)
- Dictionaries
- Garbage collection
- Exceptions
- Classes / objects
