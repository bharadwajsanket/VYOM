# 🛣 Vyom Roadmap

Vyom evolves **intentionally and conservatively**.  
Stability comes first. Features earn their place.

This roadmap reflects **actual shipped versions and committed direction**,  
not aspirational promises.

---

## ✅ v0.1 — Core Language (Released)

**Focus:** the absolute minimum required to execute code

- Variable assignment
- Print statement
- Indentation-based blocks
- Sequential execution

---

## ✅ v0.2 — Expressions (Released)

**Focus:** data and computation

- Numeric and string literals
- Variable reassignment
- Arithmetic operators: `+ - * /`
- Operator precedence
- Parentheses in expressions

---

## ✅ v0.3 — Structure (Released)

**Focus:** function definitions and parsing

- Function definitions using `def`
- Parameters and return values
- Optional type annotations
- Static (lexical) scoping model

---

## ✅ v0.4 — Execution Model (Released)

**Focus:** completing the procedural execution engine

- Function calls
- Local scope
- Call stack
- Nested function execution
- Runtime error reporting with line numbers
- Strict scope rules (no accidental shadowing)

> v0.4 is the **first fully usable Vyom language**.

---

## 🧱 v0.4.x — Stability & Hardening (Completed Phase)

**Focus:** freezing core semantics

- Bug fixes only
- Edge-case handling
- Interpreter safety
- Documentation cleanup
- Packaging improvements

> No new language features were introduced in this phase.

---

## ✅ v0.5 — Control Flow (Released)

**Focus:** readable decision-making

- Comparison operators: `== != < > <= >=`
- Logical operators: `and`, `or`, `not`
- Short-circuit evaluation
- `if / elif / else`
- Nested conditionals
- Numeric truth values (`0 = false`, non-zero = true)

---

## ✅ v0.6 — Loops & Iteration (Released)

**Focus:** controlled repetition

- `while (condition):`
- `for (init; condition; step):` (C-style)
- `for i in range(n):`
- Loop nesting
- Correct `break`, `continue`, and `return` behavior

---

## ✅ v0.7 — Strict Core & Developer Quality (Released)

**Focus:** correctness, explicitness, and discipline

- Mandatory `print(...)` syntax
- Multi-argument `print`
- `const` variables (immutable)
- Strict type-safe comparisons
- Loop-local scope isolation
- `break` and `continue`
- Introspection constants: `__version__`, `__file__`

> v0.7 intentionally adds **no data structures**.

---

## ✅ v0.8 — Fixed Arrays & Indexing (Released)

**Focus:** minimal data structures without abstraction creep

- Fixed-size typed arrays (numeric & string)
- Array element assignment with bounds checking
- Read-only string indexing (`s[i]`)
- `len()` for arrays and strings
- Enhanced `range(start, stop, step)` with strict validation
- `exit(code)` with full memory cleanup

> No dynamic resizing. No collections magic.

---

## 🔮 v0.9 — File I/O & Environment (Planned)

**Focus:** controlled interaction with the outside world

- File read operations
- File write operations
- Explicit error handling
- No implicit resource management

---

## 🚀 v1.0 — Stable Core (Target)

**Definition of v1.0:**

- Stable syntax (no breaking changes)
- Stable semantics
- Fully documented behavior
- Predictable execution model
- Production-ready procedural scripting language

---

## 🧭 Design Rule (Non-Negotiable)

Vyom will **never** add:

- Hidden execution passes
- Implicit type coercions
- Magical scope behavior
- Features that cannot be explained line-by-line

If a feature cannot be taught cleanly,  
**it does not belong in Vyom.**
