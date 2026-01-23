# 🛣 Vyom Roadmap

Vyom evolves **intentionally and conservatively**.  
Stability comes first. Features earn their place.

This roadmap reflects **actual shipped versions**, not hypothetical ideas.

---

## ✅ v0.2 — Stability Core (Released)

**Focus:** syntax cleanup, predictability, and reliability

- `=` assignment (removed `set`)
- case-sensitive variables
- string concatenation
- arithmetic expressions (`+ - * /`)
- `type(x)`
- `exit`
- human-friendly error messages
- indentation-based blocks

---

## ✅ v0.3 — Types & Structure (Released)

**Focus:** structure without execution complexity

- optional explicit typed variables
  - `int`
  - `double`
  - `string`
- type locking for explicit variables
- dynamic (untyped) variables still supported
- function definitions using `def`
- functions parsed & stored (not executed)

> v0.3 introduced structure, not behavior.

---

## ⭐ v0.4 — Functions Complete (Released)

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
- error-message improvements
- edge-case handling
- documentation refinement
- NO new language features

> v0.4.x defines Vyom’s **core identity**.

---

## 🎯 v0.5 — Control Flow (Planned)

**Focus:** decision-making without complexity

- comparison operators  
  `> < >= <= == !=`
- numeric truth values (`0` = false, non-zero = true)
- `if / else` blocks (indentation-based)
- logical operators: `and`, `or`, `not`
- short-circuit evaluation

❌ No `elif`  
❌ No boolean type  
❌ No ternary operator  

---

## 🔁 v0.6 — Iteration (Planned)

**Focus:** controlled repetition

- looping construct (design TBD)
- clear execution rules
- no hidden iterator magic

---

## 📦 v0.7 — Data Structures (Planned)

**Focus:** minimal but useful containers

- lists / arrays
- indexing (`x[0]`)
- length / basic helpers

---

## 💻 v0.8 — REPL Mode (Optional)

- interactive shell
- step-by-step execution
- debugging-friendly output

---

## 🔬 Long-Term (Exploratory)

- modules / imports
- minimal standard library
- performance profiling & optimization
- formatter / linter
- language test suite

---

## 🧭 Design Rule (Non-Negotiable)

Vyom will **never** add:
- hidden execution passes
- implicit coercions
- magical scope behavior
- features that cannot be explained line-by-line

If a feature cannot be taught cleanly,  
it does not belong in Vyom.