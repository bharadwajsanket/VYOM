# 📜 Vyom — Changelog

All notable changes to **Vyom** are documented in this file.

Vyom follows a **clarity-first** philosophy:  
features are introduced only when they can be explained cleanly and implemented predictably.

---

## v0.5 — Control Flow & Boolean Logic ⭐ (Latest)

**Release focus:** structured decision-making with strict expression evaluation

### Added
- **Control flow statements**
  - `if / elif / else` with indentation-based blocks
  - Nested conditionals fully supported
  - Returns inside conditionals
  ```vy
  if score >= 90:
      print "A"
  elif score >= 60:
      print "B"
  else:
      print "C"
  ```

- **Comparison operators**
  - `==` `!=` `<` `>` `<=` `>=`
  - All comparisons return numeric values (1 or 0)
  - Chained comparisons explicitly disallowed (`a < b < c` is an error)

- **Logical operators**
  - `and` `or` `not`
  - **Short-circuit evaluation** implemented correctly
  - Numeric truth model: `0` is false, non-zero is true
  - All logical operators return 1 or 0
  ```vy
  print a < b and b > 15
  print not (a == b)
  ```

- **Expression evaluation pipeline**
  - Strict precedence: `or` < `and` < comparison < arithmetic < term
  - Parentheses for grouping: `(a == b) and (c == d)`
  - Operators cannot be used in assignment expressions

### Fixed
- Empty expression handling (blank lines no longer crash interpreter)
- Parenthesized expression unwrapping `(a == b)` now evaluates correctly
- Main loop block consumption for `if/elif/else` compounds
- Assignment vs comparison safety (`==` cannot be used as `=`)
- Whitespace-only lines properly skipped
- Empty `print` statements handled gracefully

### Changed
- `return` statements now evaluate through full condition pipeline
- Expression evaluation is strictly typed (no implicit conversions)
- Error messages enhanced with context-specific details

### Design Notes
- **NO boolean type** — numeric truth model only
- **NO ternary operator** — use `if/else` explicitly
- **NO `else if`** — must use `elif` keyword
- **NO loops yet** — coming in v0.6
- Control flow follows Python-like syntax with C-like semantics

### Error Handling Improvements
- `elif without preceding if`
- `else without preceding if`
- `use 'elif' instead of 'else if'`
- `chained comparisons not allowed`
- `empty right-hand side in assignment`
- `comparison is not assignment`

### Technical Improvements
- Defense-in-depth validation at pipeline entry points
- Fail-safe defaults for edge cases
- Robust empty string handling across all evaluation contexts
- Production-grade error recovery

> v0.5 completes the **procedural foundation** of Vyom. Control flow, comparisons, and logical operations work exactly as specified with no hidden behavior.

---

## v0.4.3 — Stable Core Finalization

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

## Roadmap

### v0.6 (Planned) — Loops & Iteration
- `while` loops
- `for` loops with range iteration
- `break` and `continue`
- Loop control flow

### v0.7 (Planned) — Collections
- Lists/arrays
- Basic collection operations
- Indexing and slicing

### Future Considerations
- String methods
- File I/O
- Module system
- Standard library

---

**Philosophy:** Vyom grows only when features can be explained simply and behave predictably.
