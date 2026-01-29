<p align="center">
  <img src="assets/VYOM-Banner.png" alt="Vyom banner" />
</p>


<p align="center">
  Created and maintained by <b>Sanket Bharadwaj</b>
</p>

<p align="center">
  <a href="#-current-version">Version</a> •
  <a href="#-quick-start">Quick Start</a> •
  <a href="#-features">Features</a> •
  <a href="#-examples">Examples</a> •
  <a href="#-philosophy">Philosophy</a>
</p>

# Vyom

**A minimal, explicit scripting language with transparent execution semantics.**

Vyom is a statically-scoped, dynamically-typed scripting language implemented in ISO C (C99-compatible). It executes code directly without virtual machines, bytecode compilation, or runtime optimization layers. Every construct maps to straightforward interpretation logic.

**Design Principle:** If you cannot trace execution line-by-line through the source, the design is wrong.

---

## Status

**Current Release:** v0.6.0 (Loops & Iteration)

```
Language:     Vyom
Version:      0.6.0
License:      MIT
Interpreter:  ~1100 lines of C
Dependencies: None (libc only)
Platform:     POSIX-compliant systems, Windows
```

---

## Features

**Implemented in v0.6:**
- Variables with lexical scoping
- First-class functions with lexical scoping (closures not supported)
- Numeric types (integer, floating-point)
- String literals (immutable)
- Arithmetic and comparison operators
- Boolean logic (`and`, `or`, `not`) with short-circuit evaluation
- Control flow: `if`/`elif`/`else`
- Loops: `while`, `for` (C-style), `for-in-range`
- Recursive function calls
- Explicit return semantics

**Not Yet Implemented:**
- Closures
- Aggregate data types (arrays, dictionaries)
- String manipulation methods
- File I/O primitives
- Exception handling
- Module system

---

## ⚡ Quick Start

### Installation

#### **Windows** - **[Download Latest Release](https://github.com/Sanket-Bharadwaj/VYOM/releases/latest)**

Includes:
- `vyom.exe` (standalone executable)
- `install.bat` (adds to PATH)
- `uninstall.bat`
- Example programs

Run `install.bat` once and restart your terminal.

#### **macOS / Linux**
Build from source:
```bash
git clone https://github.com/Sanket-Bharadwaj/VYOM.git
cd VYOM
gcc src/vy.c -o vyom
chmod +x vyom
sudo mv vyom /usr/local/bin/
```

### Hello World

```vy
# hello.vy
name = "Vyom"
print "Hello"
print name
```

```bash
$ vyom hello.vy
Hello
Vyom
```

---

## Language Overview

### Variable Binding

Variables are dynamically typed and lexically scoped. No implicit type coercion occurs.

```vy
x = 42          # numeric
y = 3.14159     # numeric
name = "Alice"  # string

# Type mismatch errors are runtime errors
z = x + name    # Error: cannot add number and string
```

### Functions

Functions are first-class values. Parameters are passed by value.

```vy
def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)

print fibonacci(10)  # 55
```

No variable shadowing is permitted. Local variables cannot hide outer scope bindings.

### Control Flow

Conditional execution follows standard semantics:

```vy
def classify(x):
    if x < 0:
        return "negative"
    elif x == 0:
        return "zero"
    else:
        return "positive"
```

### Loops

Three loop constructs are provided:

**While loop:**
```vy
i = 0
while (i < 5):
    print i
    i = i + 1
```

**C-style for loop:**
```vy
for (i = 0; i < 10; i = i + 1):
    print i
```

**Range-based iteration:**
```vy
for i in range(10):
    print i
```

Loops may be nested. Early return from within a loop terminates the enclosing function.

### Operators

**Arithmetic:** `+`, `-`, `*`, `/`  
**Comparison:** `==`, `!=`, `<`, `>`, `<=`, `>=`  
**Logical:** `and`, `or`, `not`

Logical operators short-circuit. Division by zero is a runtime error.

---

## Design Rationale

### No Hidden Complexity

Vyom prioritizes transparent execution over convenience features. No implicit type conversions. No hidden allocations. No optimizer rewriting your logic. Every line of code maps directly to interpreter actions.

### Explicit Semantics

```vy
# This fails at runtime - no silent conversion
x = 5 + "10"  # Error: type mismatch

# This is explicit and works
x = 5
y = 10
result = x + y
```

If the behavior isn't obvious from reading the code, it's a language bug.

### Transparent Implementation

The interpreter is deliberately kept simple:
- Single-pass tokenization
- Recursive descent parsing
- Direct AST interpretation
- No intermediate representation
- No bytecode emission

This makes the implementation understandable to anyone who wants to learn how interpreters work.

### Execution Model

Vyom prioritizes predictable execution over aggressive optimization. The interpreter performs exactly the operations expressed by the source code, without reordering, specialization, or speculative execution.

The execution model:
- Direct interpretation of source code
- No runtime optimization
- No garbage collection
- Memory grows until process exit
- Predictable resource usage

This design trades runtime performance for implementation clarity and execution transparency.

---

## Error Handling

Errors are detected at runtime and reported with context:

```
Error at line 15: Undefined variable 'count'
Error at line 23: Division by zero
Error at line 8: Type mismatch in operation
```

No exceptions. No stack unwinding. Errors terminate execution immediately.

---

## Platform Support

**Primary development platform:** Linux (GCC, Clang)  
**Tested on:** macOS, Windows (MinGW, MSVC), FreeBSD

The interpreter requires only standard C library functions. No platform-specific dependencies.

---

## Project Structure

```
VYOM/
├── src/
│   └── vy.c              # Complete interpreter implementation
├── examples/
│   ├── main.vy          # Basic syntax demonstration
├── docs/
│   ├── SYNTAX.md         # Complete language specification
│   ├── DESIGN.md         # Implementation notes
│   └── CHANGELOG.md      # Version history
└── README.md
```

---

## Versioning

Vyom uses semantic versioning. Minor version increments add language features. Patch versions fix bugs without changing semantics.

**Roadmap:**
- **v0.7:** Lists and indexing operations
- **v0.8:** String methods and manipulation
- **v0.9:** File I/O and standard library
- **v1.0:** Stable language specification

---

## Contributing

Contributions are welcome but must align with core design principles:

1. **Clarity over cleverness** – readable code beats terse code
2. **Explicit over implicit** – no magic behavior
3. **Simple over complex** – fewer features done right beats many features done poorly

Before submitting patches:
- Open an issue to discuss the change
- Ensure the interpreter remains under 2000 lines
- Add test cases demonstrating the feature
- Update documentation

Pull requests that add complexity without proportional value will be rejected.

---

## Testing

Run the test suite:

```bash
./vyom tests/test_suite.vy
```

Current test coverage focuses on:
- Arithmetic edge cases (division by zero, overflow)
- Scope resolution
- Function call semantics
- Loop termination conditions
- Type error detection

---

## Limitations

**Current limitations:**
- Call stack depth is limited (typically ~1000 frames)
- No tail call optimization
- No garbage collection (memory grows until program exit)
- No closures
- Limited string operations
- No Unicode support

These are design tradeoffs for implementation simplicity. Future versions may address some of these constraints.

---

## Design Trade-offs

Vyom makes deliberate design choices that prioritize execution transparency and implementation simplicity.  
The following table highlights these trade-offs relative to common scripting language designs — not as competition, but as context.

| Feature | Vyom | Python | Lua | JavaScript |
|---------|------|--------|-----|------------|
| Virtual machine / bytecode | No | Yes | Yes | Yes |
| Implicit type conversions | No | Yes | Yes | Yes |
| Variable shadowing | Disallowed | Allowed | Allowed | Allowed |
| Garbage collection | No | Yes | Yes | Yes |
| Interpreter size (approx.) | ~1100 LOC | ~400K LOC | ~30K LOC | ~1M+ LOC |


Vyom intentionally favors a small, readable interpreter and explicit execution semantics.  
These choices trade convenience and automation for predictability and clarity.

---

## Documentation

**Language Reference:** [docs/SYNTAX.md](docs/SYNTAX.md)  
**Design Notes:** [docs/DESIGN.md](docs/DESIGN.md)  
**Changelog:** [docs/CHANGELOG.md](docs/CHANGELOG.md)

---

## License

MIT License. See [LICENSE](LICENSE) for full text.

---

## Author

Created and maintained by **Sanket Bharadwaj**.

**Contact:** sanketbharadwaj15@gmail.com  
**Repository:** https://github.com/Sanket-Bharadwaj/VYOM

---

## Philosophy

> "Programs must be written for people to read, and only incidentally for machines to execute."  
> — Structure and Interpretation of Computer Programs

Vyom embodies this principle. The interpreter is short enough to read in one sitting. The execution model has no hidden layers. The language semantics are small enough to hold in your head.

The code you write is the code that runs.

**Build with clarity. Execute with transparency.**
