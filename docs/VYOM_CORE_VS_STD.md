# 🌌 Vyom — Core vs Std (Clear Explanation)

Vyom ko **do clearly separated layers** me design kiya gaya hai:

1. **Vyom Core** — language itself  
2. **Vyom Std** — convenience & productivity layer

Dono ka purpose, audience aur scope alag hai — intentionally.

---

## 🧱 Vyom Core

### Vyom Core kya hai?

**Vyom Core = minimal, raw, predictable language runtime**

Ye wahi cheez hai jo:
- language ke rules define karti hai
- execution semantics fix karti hai
- performance aur memory behavior decide karti hai

Core ka principle:
> **No magic. No sugar. Only what you write runs.**

---

### ✅ Vyom Core me kya hota hai

Sirf **language features**:

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
arr int a[3][4]
a[1][2] = 42
print(a[1][2])
```

Jo likha hai wahi execute hota hai.

---

### ❌ Vyom Core me kya **nahi** hota

Intentionally exclude kiya gaya hai:

- ❌ File I/O (`read`, `write`)
- ❌ String helpers (`upper`, `lower`, `reverse`)
- ❌ Math / statistics helpers
- ❌ Lists, dictionaries, tuples
- ❌ Import system
- ❌ Hidden helpers
- ❌ Runtime sugar

Rule:
> **Agar cheez C me “free” nahi hai,  
> to Vyom Core me bhi free nahi hogi.**

---

### 🎯 Vyom Core kis ke liye hai?

- C / C++ developers
- Performance-focused users
- Systems programmers
- Language learners
- Jinko memory, speed aur predictability chahiye

---

## 🌿 Vyom Std (Standard Library)

### Vyom Std kya hai?

**Vyom Std = productivity layer on top of Core**

Ye language ka part nahi hai.  
Ye **optional convenience package** hai.

Std ka goal:
> **Kam code, zyada intent**

---

### ✅ Vyom Std me kya hota hai

High-level helpers:

- File I/O
  ```vy
  read("file.txt")
  write("out.txt", data)
  ```

- String utilities
  ```vy
  upper(s)
  lower(s)
  reverse(s)
  ```

- Math / stats
  ```vy
  mean(arr)
  max(arr)
  min(arr)
  ```

- Safe defaults
- Developer-friendly APIs

Example (Std):

```vy
data = read("a.txt")
print(upper(reverse(data)))
```

---

### ❌ Vyom Std kya nahi karta

- Core language rules change ❌
- Hidden behavior introduce ❌
- Core performance guarantees break ❌

Std **sirf Core ke upar** chalta hai.

---

## 🔁 Relationship: Core vs Std

Non-negotiable rule:

> **Std depends on Core  
> Core never depends on Std**

Diagram:

```
Vyom Std   (optional, evolving)
   ▲
   │
Vyom Core  (stable, frozen at 1.0)
```

---

## 📦 Distribution Model

### Option 1 — Core only

```
vyom-core
```

- Minimal
- Fast
- No std
- Advanced users

---

### Option 2 — Core + Std

```
vyom
```

- Core included
- Std auto-available
- Beginner friendly

User decide karta hai:
> **Kitna control chahiye vs kitni convenience**

---

## 🧠 One-line Summary

- **Vyom Core** = C-style honesty  
- **Vyom Std** = Python-style comfort  

Same language.  
Different layers.

---

## 🔒 Final Design Promise

- Core freezes at **v1.0**
- Std evolves independently (2.0, 3.0…)
- Core semantics never break
- Convenience is optional, not forced

---

## 🔥 Final Thought

> **Vyom slow nahi hai.  
> Vyom honest hai.  
> Sugar optional hai.**
