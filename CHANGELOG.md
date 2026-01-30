
# 📜 Vyom — Changelog

All notable changes to the **Vyom Programming Language** are documented here.
This file reflects the complete evolution from **v0.1 → v0.7**.

---

# 🚀 v0.7 — Strict Core & Developer Quality

## Summary

Vyom v0.7 is a **strict core** release focused on developer quality, consistency, and safety.
This is an incremental evolution, not a rewrite.

All v0.6 features are preserved **except where explicitly changed**.

---

## 🔥 Breaking Changes

### 1. `print()` Now Requires Parentheses

**OLD (≤ v0.6):**
```vy
print "hello"
print x
```

**NEW (v0.7):**
```vy
print("hello")
print(x)
print("x =", x)
print()
```

---

## ✨ New Features

- Multi-argument `print()`
- `break` and `continue`
- Loop-local scope
- `const` variables
- Strict type comparisons
- `__version__`, `__file__` introspection

---

## 📦 v0.6 — Loops & Iteration
- while loops
- for loops
- range loops

---

## 🎯 v0.5 — Control Flow
- if / elif / else
- logic & comparisons

---

## 🧠 v0.4 — Safety
- runtime validation
- call stack

---

## 🧩 v0.3 — Functions

---

## 🧮 v0.2 — Expressions

---

## 🌱 v0.1 — Core Interpreter

---

**Vyom is designed to be understood, not rushed.**
