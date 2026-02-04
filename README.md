<p align="center">
  <img src="assets/VYOM-Banner.png" alt="Vyom banner" />
</p>

<p align="center">
  Created and maintained by <b>Sanket Bharadwaj</b>
</p>

<p align="center">
  <a href="#status">Version</a> •
  <a href="#quick-start">Quick Start</a> •
  <a href="#features">Features</a> •
  <a href="#language-overview">Language Overview</a> •
  <a href="#philosophy">Philosophy</a>
</p>

# Vyom

**A minimal, explicit scripting language with transparent execution semantics.**

Vyom is a statically-scoped, dynamically-typed scripting language implemented in ISO C (C99-compatible).  
It executes code directly without virtual machines, bytecode compilation, or runtime optimization layers.

**Design Principle:**  
If you cannot trace execution line-by-line through the source, the design is wrong.

---

## Status

**Current Release:** v0.8.0 — *Strict Core + Fixed Arrays*

```
Language:     Vyom
Version:      0.8.0
License:      MIT
Interpreter:  ~2000 lines of C
Dependencies: None (libc only)
Platform:     POSIX-compliant systems, Windows
```

---

## Features

### Implemented in v0.8
- Lexically scoped variables
- Optional explicit typing: `int`, `double`, `string`
- `const` variables (immutable)
- **Fixed-size typed arrays** (numeric & string)
- Array element assignment with mandatory bounds checking
- **String indexing (read-only)**
- **`len()` builtin** for arrays and strings
- First-class functions (no closures)
- Numeric and string literals
- Arithmetic operators: `+ - * /`
- Strict type-safe comparisons
- Boolean logic: `and`, `or`, `not` (short-circuit)
- Control flow: `if / elif / else`
- Loops:
  - `while`
  - C-style `for`
  - `for-in range(start, stop, step)`
- `break` and `continue`
- Block-local loop scope
- **`exit(code)` builtin** with cleanup
- Introspection constants: `__version__`, `__file__`
- Mandatory `print(...)` with multi-argument support
- Explicit return semantics

### Explicit Non-Goals
- No dynamic arrays or lists
- No garbage collection
- No exceptions
- No classes / objects
- No implicit behavior

---

## ⚡ Quick Start

## Installation

### ⊞ Windows

[![Download Vyom](https://img.shields.io/badge/Download-Vyom%20Latest-blue?style=for-the-badge)](https://github.com/Sanket-Bharadwaj/VYOM/releases/latest)

**Included in the Windows release:**
- `vyom.exe` — standalone Vyom interpreter
- `install.bat` — adds Vyom to PATH
- `uninstall.bat` — removes Vyom from the system
- `examples/` — sample Vyom programs

**Steps:**
1. Download and extract the ZIP
2. Run `install.bat`
3. Restart your terminal

Verify:
```bash
vyom --version
```

⚠️ **Windows Security Notice**

When running `install.bat`, Windows may show:  
"Smart App Control blocked this file"

This is expected for unsigned scripts.

To proceed:
- Right click `install.bat`
- Properties → Check "Unblock"
- Run again (preferably as Administrator)

---

###  macOS / 🐧 Linux

Build from source:
```bash
git clone https://github.com/Sanket-Bharadwaj/VYOM.git
cd VYOM
gcc src/vy.c -o vyom
chmod +x vyom
sudo mv vyom /usr/local/bin/
```

Verify:
```bash
vyom --version
```

---

## 👋 Hello World

Create `hello.vy`:
```vy
name = "Vyom"
print("Hello")
print(name)
```

Run:
```bash
vyom hello.vy
```

Output:
```text
Hello
Vyom
```

---

## Philosophy

Vyom is designed to be **readable**, **predictable**, and **honest** in execution.

- No magic
- No hidden layers
- No silent behavior

The code you write is the code that runs.

**Build with clarity. Execute with transparency.**
<<<<<<< Updated upstream

=======
<<<<<<< HEAD
=======

>>>>>>> 85c8d09 (docs(v0.8): update README, roadmap, syntax guide and story)
>>>>>>> Stashed changes
