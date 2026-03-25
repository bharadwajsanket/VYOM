<p align="center">
  <img src="assets/VYOM-Banner.png" alt="Vyom" />
</p>

<p align="center">
  <b>A small, explicit scripting language implemented in C.</b>
</p>

<p align="center">
  <a href="https://getvyom.vercel.app">Website</a> •
  <a href="https://github.com/Sanket-Bharadwaj/VYOM">GitHub</a> •
  <a href="https://github.com/Sanket-Bharadwaj/VYOM/releases/latest">Download</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/version-1.0-blue?style=flat-square" alt="Version" />
  <img src="https://img.shields.io/badge/language-C-555?style=flat-square&logo=c" alt="Language" />
  <img src="https://img.shields.io/badge/platform-POSIX%20%7C%20Windows-lightgrey?style=flat-square" alt="Platform" />
  <img src="https://img.shields.io/badge/license-MIT-green?style=flat-square" alt="License" />
  <img src="https://img.shields.io/badge/dependencies-none-orange?style=flat-square" alt="Dependencies" />
</p>

---

# Vyom

Vyom is a scripting language that executes source code directly.  
No bytecode, no virtual machine, no runtime optimizer.  
The interpreter is a single C file — around 2700 lines.

**Design rule:** if you cannot trace execution line by line through the source, the design is wrong.

---

## Features

**Types**
- `int`, `double`, `string`
- Optional explicit type declarations
- `const` variables

**Variables and Expressions**
- Assignment and arithmetic: `+ - * / // %`
- Comparisons: `== != < > <= >=`
- Logic: `and`, `or`, `not` (short-circuit)
- Operator precedence, parenthesised expressions

**Control Flow**
- `if` / `elif` / `else`
- `while` loop
- C-style `for (init; cond; step):`
- `for i in range(start, stop, step):`
- `break`, `continue`

**Functions**
- Parameters, return values, recursion
- Block-local scope
- No closures

**Arrays**
- Fixed-size 1D arrays: `int a[n]`
- Fixed-size 2D arrays: `int m[rows][cols]`
- Numeric and string element types
- Mandatory bounds checking
- Partial initialisation with zero-fill

**Strings**
- String literals, indexing (read-only)
- String comparisons

**Builtins**

| Builtin | Description |
|---|---|
| `print(...)` | Print one or more values |
| `input()` | Read a line from stdin |
| `len(x)` | Length of array or string |
| `ascii(c)` | ASCII value of a single character |
| `char(n)` | Character from ASCII code (0–255) |
| `concat(a, b)` | Join two strings |
| `exit(code)` | Exit with status code |

**Safety**
- Division by zero detected at runtime
- Array index out of bounds detected at runtime
- All errors include the source line number
- Recursion depth limit enforced (prevents C stack overflow)

---

## Quick Start

### Windows

Download the latest release:  
**[Download](https://github.com/bharadwajsanket/VYOM/releases/latest)**

The ZIP includes `vyom.exe`, `install.bat`, and `examples/`.

1. Extract the ZIP
2. Run `install.bat` (right-click → Run as Administrator if prompted)
3. Restart your terminal

```
vyom --version
```

> If Windows blocks `install.bat`: right-click → Properties → check **Unblock** → run again.

---

### macOS / Linux

Build from source:

```bash
git clone https://github.com/Sanket-Bharadwaj/VYOM.git
cd VYOM
gcc -O2 src/vy.c -o vyom -lm
sudo mv vyom /usr/local/bin/
```

Verify:

```bash
vyom --version
```

---

## Error Messages

Errors always include the source line number:

```
Error (line 7): division by zero
Error (line 12): array index 10 out of bounds (size 5)
Error (line 3): undefined variable 'x'
Error (line 9): function 'add' expects 2 args, got 3
```

---

## Philosophy

Vyom is built around one idea: **what you write is exactly what runs.**

- No implicit conversions
- No hidden allocations
- No magic behaviour
- No silent failures

The language stays small on purpose. Features are added only when they make programs clearer, not to expand what the language can do in the abstract.

The interpreter is a single readable C file. If something behaves unexpectedly, the source is the documentation.

---

<details>
<summary>Example — Fibonacci</summary>

```vyom
def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

for i in range(8):
    print(fib(i))
```

Output:

```
0
1
1
2
3
5
8
13
```

</details>

---

<p align="center">
  Created and maintained by <b>Sanket Bharadwaj</b><br>
  <a href="https://getvyom.vercel.app">getvyom.vercel.app</a>
</p>