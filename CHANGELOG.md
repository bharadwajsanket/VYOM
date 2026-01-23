# 📜 Vyom — Changelog

All notable changes to **Vyom** are documented in this file.

Vyom follows a **learning-first** release philosophy:  
each version introduces concepts only when they can be explained cleanly.

---

## v0.4 — Functions Complete ⭐ (Latest)

**Release focus:** completing the function model with clarity and correctness

### Added
- Function calls  
  ```vy
  greet()
  add(3, 4)
  ```
- Function parameters
- Return values using `return`
- Local scope inside functions
- Proper call stack implementation
- Nested function calls  
  ```vy
  sum_of_squares(3, 4)
  ```
- Numeric-safe execution model
- Line-number aware runtime errors

### Fixed
- Argument evaluation order (arguments are evaluated **before** call frame creation)
- Variable shadowing bugs between global and local scope
- Incorrect name resolution inside nested calls

### Design Notes
- Functions are collected first, then executed
- Each function call creates a fresh local frame
- Locals shadow globals (no closures by design)
- Functions used as expressions **must** return a value

> v0.4 marks the first version where Vyom can express real computation cleanly.

---

## v0.3 — Typed Variables & Structure

**Release focus:** introducing structure without breaking simplicity

### Added
- Optional explicit typed variables:
  - `int`
  - `double`
  - `string`
- Type locking for explicit variables
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
- Functions are parsed & stored only

> v0.3 was about *structure*, not execution.

---

## v0.2 — Stability & String Concatenation

**Release focus:** cleaner syntax, reliability, and predictable behavior

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
- Variables are now **case-sensitive**
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
- Comments `#`
- Indentation-based blocks
- `--version` and `--help`

---
