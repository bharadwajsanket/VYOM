# 📜 Vyom — Changelog

All notable changes to the **Vyom Programming Language** are documented here.

---

## 🚀 v0.6 — Loops & Iteration (Current)

**Release focus:** Controlled repetition and iteration patterns

### ✨ Added
- **While loops**: `while (condition):` with mandatory parentheses
- **C-style for loops**: `for (init; condition; step):`
- **Range-based for loops**: `for i in range(n):` — syntactic sugar for common patterns
- **Loop nesting**: loops inside functions, conditionals, and other loops
- **Return from loops**: `return` inside a loop properly exits the function

### 🔒 Rules & Guarantees
- Mandatory parentheses in while conditions
- Mandatory colons after all loop headers
- Semicolon-separated clauses in C-style for loops
- Range iteration always starts at 0, increments by 1
- Loops respect indentation-based block structure
- Early returns from loops work correctly

### ❌ Not Included
- `break` and `continue` statements
- `do-while` loops
- `repeat` loops
- `range(start, end)` or `range(start, end, step)`
- Boolean type
- Implicit variables in range loops

---

## 🎯 v0.5 — Control Flow & Boolean Logic

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
- Loops
- Boolean literals (`true`, `false`)

---

## 🧠 v0.4 — Functions & Local Scope

**Release focus:** Making execution predictable and safe

### ✨ Added
- Division-by-zero detection
- Undefined variable detection
- Invalid assignment detection (`a = b = c`)
- Function argument count validation
- Error on missing `return` in functions
- Error on `return` outside function
- Unmatched parentheses detection
- Call stack implementation
- Local variable scope isolation
- Function parameter binding

### 🔒 Guarantees
- No silent failures
- No undefined behavior
- Program stops immediately on error
- Local variables cannot shadow global variables

---

## 🧩 v0.3 — Functions & Structure

**Release focus:** Program structure and execution boundaries

### ✨ Added
- Function definitions using `def`
- Function calls with parameters
- Numeric return values
- Call stack with isolated local scopes
- Global vs local variable separation
- Variable type checking for explicit types

### 🔒 Rules
- Each function call gets a fresh local scope
- Local variables cannot shadow global variables
- Missing `return` in a function is an error

### ❌ Not Included
- Closures
- Recursion optimizations

---

## 🧮 v0.2 — Expressions & Variables

**Release focus:** Data and computation

### ✨ Added
- Dynamic variables
- Numeric and string literals
- Variable reassignment
- Arithmetic operators:
  - `+`, `-`, `*`, `/`
- Operator precedence
- Parentheses in arithmetic expressions
- Basic error reporting

### ❌ Not Included
- Modulus (`%`)
- Floor division (`//`)
- Type system

---

## 🌱 v0.1 — Core Language

**Release focus:** The absolute minimum to execute code

### ✨ Added
- Variable assignment: `x = value`
- Print statement: `print x`
- Indentation-based structure
- Sequential execution
- String literals with double quotes
- Numeric literals (integers and decimals)
- Line-by-line interpretation
- Comment support with `#`

### 🔒 Guarantees
- Code executes line by line, top to bottom
- Variables persist across lines
- Errors stop execution immediately

---

## 📋 Legend

- **✨ Added** — New features introduced
- **🔒 Rules & Guarantees** — Design commitments and semantic rules
- **❌ Not Included** — Intentionally excluded features
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
