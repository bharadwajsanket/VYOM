# 📜 Vyom — Changelog

All notable changes to **Vyom** are documented in this file.

Vyom follows a **clarity-first** philosophy:  
features are introduced only when they can be explained cleanly and implemented predictably.

---

## v0.4.3 — Stable Core Finalization ⭐ (Latest)

**Release focus:** correctness, strict scoping, and execution safety

### Fixed
- Enforced **no-shadowing rule**
  - Local variables cannot shadow global variables
  - Function parameters cannot shadow globals
- Corrected function return enforcement
  - Functions used in expressions **must** return a value
  - Functions used as statements may omit `return`
- Stable argument evaluation order  
  *(arguments are evaluated before call frame creation)*
- Clear runtime errors with accurate line numbers

### Notes
- v0.4.x is now considered a **frozen core**
- No semantic or scoping changes will be made in this series
- All future features will land in **v0.5+**

> v0.4.3 represents a stable, minimal procedural language with strict rules and predictable behavior.

---

## v0.4 — Functions Complete

**Release focus:** completing the function execution model

### Added
- Function calls  
  ```vy
  greet()
  add(3, 4)
  ```
- Function parameters
- Return values using `return`
- Local scope inside functions
- Call stack–based execution
- Nested function calls  
  ```vy
  sum_of_squares(3, 4)
  ```
- Numeric-safe execution model
- Line-number aware runtime errors

### Fixed
- Argument evaluation order
- Name resolution inside nested calls
- Early function execution bugs

### Design Notes
- Functions are collected first, then executed
- Each function call creates a fresh local frame
- **Shadowing is explicitly disallowed**
- No closures (by design)

> v0.4 is the first version where Vyom expresses real computation cleanly.

---

## v0.3 — Typed Variables & Structure

**Release focus:** introducing structure without breaking simplicity

### Added
- Optional explicit typed variables:
  - `int`
  - `double`
  - `string`
- Type locking for explicitly typed variables
- Function definitions using `def`
  ```vy
  def greet():
      print "Hello"
  ```

### Changed
- Typed variables cannot change type after declaration
- Clear type-error messages for invalid assignments

### Limitations (Intentional)
- Function calls not supported
- `return` not supported
- Functions are parsed and stored only

> v0.3 was about **structure**, not execution.

---

## v0.2 — Stability & Cleaner Syntax

**Release focus:** reliability and predictable behavior

### Added
- String concatenation
  ```vy
  print "a = " + x
  print name + " Lang"
  ```
- `type(x)` → prints `number` or `string`
- `exit` — graceful execution stop
- Whitespace-tolerant parsing

### Changed
- Switched from `set x = 10` → `x = 10`
- Variables became **case-sensitive**
- Improved error messages
- Better handling of invalid identifiers & expressions

---

## v0.1 — Core Interpreter (Initial Release)

### Added
- Variables (`set` syntax)
- Numbers & strings
- `print`
- Math expressions (`+  -  *  /`)
- `if / elif / else`
- `repeat N:`
- Comments using `#`
- Indentation-based blocks
- `--version` and `--help`

---
