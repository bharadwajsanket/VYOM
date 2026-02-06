# 🌌 Vyom — Core vs Std (Clear & Honest Explanation)

Vyom ko **do clean layers** me design kiya gaya hai:

1. **Vyom Core** — language itself  
2. **Vyom Std** — convenience & productivity layer  

Ye separation **accidental nahi**, balki intentional hai.

---

## 🧱 Vyom Core

### Vyom Core kya hai?

**Vyom Core = minimal, raw, predictable language runtime**

Core wahi cheez hai jo:
- language ke rules define karti hai
- execution semantics fix karti hai
- performance aur memory behavior decide karti hai

Core ka ek hi principle hai:

> **No magic. No sugar. Only what you write runs.**

---

### ✅ Vyom Core me kya hota hai

Sirf **language-level features**:

- Variables
- Functions
- `if / elif / else`
- `while`, `for`, `for-in range`
- `return`, `break`, `continue`
- Fixed-size arrays (1D / 2D)
- Strings (immutable)
- Arithmetic & logical expressions
- Strict scoping rules
- `print(...)`
- `exit(code)`

Example (Core):

```vy
int a[3][4]

a[1][2] = 42
print(a[1][2])
```

Output:
```
42
```

---

### ❌ Vyom Core me kya **nahi** hota

- File I/O
- String helpers
- Math / stats helpers
- Lists, dictionaries
- Implicit imports
- Hidden helpers

---

## 🌿 Vyom Std (Standard Library)

**Vyom Std = productivity layer on top of Core**

High-level helpers jaise:

```vy
data = read("a.txt")
print(upper(reverse(data)))
```

---

## 🔁 Relationship

```
Vyom Std
   ▲
   │
Vyom Core
```

---

## 📦 Distribution

### Core only
```vy
import io
data = io.read("a.txt")
```

### Core + Std
```vy
data = read("a.txt")
```

---

## 🔒 Final Promise

- Core freezes at v1.0
- Std evolves independently
- Sugar is optional
