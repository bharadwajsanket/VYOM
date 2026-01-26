# 🛣 Vyom Roadmap

Vyom evolves **intentionally and conservatively**.  
Stability comes first. Features earn their place.

This roadmap reflects **actual shipped versions and committed direction**, not hype.

---

## ✅ v0.1 — Core Language (Released)

**Focus:** the absolute minimum to execute code

- Variable assignment
- Print statement
- Indentation-based blocks
- Sequential execution

---

## ✅ v0.2 — Expressions (Released)

**Focus:** Data and computation

- Numeric and string literals
- Variable reassignment
- Arithmetic operators: `+ - * /`
- Operator precedence
- Parentheses in expressions

---

## ✅ v0.3 — Structure (Released)

**Focus:** function definitions and parsing

- Function definitions using `def`
- Parsing parameters and returns
- Type annotation support (optional)

---

## ✅ v0.4 — Execution (Released)

**Focus:** completing the procedural execution model

- function calls
- parameters
- return values
- local scope
- call stack
- nested function calls
- numeric-safe execution
- clear runtime errors with line numbers
- strict scope rules (no accidental shadowing)

> v0.4 is the **first fully usable Vyom language**.

---

## 🔒 v0.4.x — Stability & Hardening (Current)

**Focus:** correctness, clarity, and freezing core semantics

- bug fixes only
- edge-case handling
- documentation refinement
- packaging & installer fixes
- **NO new language features**

> v0.4.x defines Vyom’s **core identity**.

---

## ✅ v0.5 — Control Flow (Released)

**Focus:** readable decision-making

- Comparison operators: `== != < > <= >=`
- Logical operators: `and`, `or`, `not`
- Short-circuit evaluation
- `if / elif / else` with nesting
- Numeric truth values: `0 = false`, non-zero = true
- Parenthesized expressions

---

## ✅ v0.6 — Loops & Iteration (Released)

**Focus:** controlled repetition

- `while (condition):` loops
- `for (init; condition; step):` C-style loops
- `for i in range(n):` range-based loops
- Loop nesting
- Proper return handling in loops

---

## 📦 v0.7 — Collections (Planned)

**Focus:** minimal but practical containers

- Lists / arrays
- Indexing (`x[0]`)
- Iteration with `for` over lists
- List length queries

---

## 📝 v0.8 — String Operations (Planned)

**Focus:** basic string manipulation

- String indexing (`s[0]`)
- String concatenation (refined)
- Built-in string functions

---

## 💾 v0.9 — File I/O (Planned)

**Focus:** reading and writing files

- File reading
- File writing
- Basic file operations

---

## 🚀 v1.0 — Stable Core (Goal)

**Definition of v1.0:**
- Stable syntax (no breaking changes)
- Stable semantics
- Complete procedural language
- Production-ready

---

## 🧭 Design Rule (Non-Negotiable)

Vyom will **never** add:
- hidden execution passes
- implicit coercions
- magical scope behavior
- features that cannot be explained line-by-line

If a feature cannot be taught cleanly,  
it does not belong in Vyom.
