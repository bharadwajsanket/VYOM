# 📜 Vyom — Changelog

All notable changes to the **Vyom Programming Language** will be documented in this file.

Vyom follows a **learning-first, explicit execution philosophy**.  
Each version adds concepts gradually, without hiding behavior.

---

## 🚀 v0.5 — Control Flow & Boolean Logic (Current)

**Release focus:** Decision making, comparisons, and logical execution

### ✨ Added
- `if / elif / else` control flow
- Nested conditional blocks
- Comparison operators:
  - `==`, `!=`, `<`, `>`, `<=`, `>=`
- Logical operators with short-circuit evaluation:
  - `and`, `or`, `not`
- Parenthesized expressions in conditions  
  `(a > 0) and (b < 10)`
- Numeric truth model:
  - `0` → false
  - non-zero → true

### 🔒 Rules & Guarantees
- Strict indentation-based blocks
- `else if` is **not allowed** (must use `elif`)
- `elif` after `else` is an error
- Chained comparisons (`a < b < c`) are not allowed
- Clear runtime errors with exact line numbers

### ❌ Not Included
- Loops (`for`, `while`)
- Boolean literals (`true`, `false`)
- Multiple arguments in `print`
- `print()` function syntax

---

## 🧠 v0.4 — Execution Safety & Errors

**Release focus:** Making execution predictable and safe

### ✨ Added
- Division-by-zero detection
- Undefined variable detection
- Invalid assignment detection (`a = b = c`)
- Function argument count validation
- Error on missing `return` in functions
- Error on `return` outside function
- Unmatched parentheses detection

### 🔒 Guarantees
- No silent failures
- No undefined behavior
- Program stops immediately on error

---

## 🧩 v0.3 — Functions & Scope

**Release focus:** Program structure and execution boundaries

### ✨ Added
- Function definitions using `def`
- Function calls with parameters
- Numeric return values
- Call stack with isolated local scopes
- Global vs local variable separation

### 🔒 Rules
- Each function call gets a fresh local scope
- Local variables cannot shadow global variables
- Missing `return` in a function is an error

### ❌ Not Included
- Closures
- Recursion optimizations
- Accessing globals without passing parameters

---

## 🧮 v0.2 — Variables & Expressions

**Release focus:** Data and computation

### ✨ Added
- Dynamic variables
- Numeric and string literals
- Variable reassignment
- Arithmetic operators:
  - `+`, `-`, `*`, `/`
- Operator precedence
- Parentheses in arithmetic expressions

### ❌ Not Included
- Modulus (`%`)
- Floor division (`//`)
- Exponentiation (`**`)
- String arithmetic

---

## 🧱 v0.1 — Core Interpreter

**Release focus:** Foundation and visibility

### ✨ Added
- Line-by-line interpreter
- Indentation-based block detection
- Comments using `#`
- `print` statement
- String and numeric output
- Clear error messages with line numbers

### 🎯 Philosophy Established
- No bytecode
- No virtual machine
- No hidden optimizations
- What you write is what executes

---

## 🚫 Intentional Limitations (as of v0.5)

These are **by design**, not missing features:

- No loops
- No arrays / lists
- No objects or classes
- No file I/O
- No modules or imports
- No garbage collection
- No type declarations (`int a = 10`)
- No boolean literals

---

## 🛣️ Forward Direction (Not Implemented Yet)

Future versions may include:
- Loops (`while`, `for`)
- Collections
- Compound assignment (`+=`, `-=`)
- Extended arithmetic
- Improved `print` syntax

These are **not part of v0.5**.

---

**Vyom is designed to be understood, not rushed.**
