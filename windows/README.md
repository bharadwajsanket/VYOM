# 🌌 Vyom — Minimal, Learning‑First Programming Language

This document describes **Vyom v0.7** behavior (Windows release).

Created and maintained by **Sanket Bharadwaj**

Vyom is a small, indentation‑based scripting language written in C.

Vyom is designed for clarity: every line executes exactly as written, with no hidden behavior, implicit conversions, or magic layers.

> “If the code can be followed line by line, the design is correct.”

------------------------------------------------------------

## 🚀 Current Version

**Vyom v0.7 — Strict Core & Developer Quality ⭐**

### New in v0.7
- Mandatory `print(...)` syntax (parentheses required)
- Multi‑argument `print()` support
- `break` and `continue` statements
- Block‑local loop scope
- `const` variables (immutable)
- Optional explicit typing: `int`, `double`, `string`
- Strict type‑safe comparisons
- Introspection constants: `__version__`, `__file__`
- Improved error messages with exact line numbers
- All features from v0.6 (loops, functions, control flow)

### Truth Model
```
0        → false
non‑zero → true
```

------------------------------------------------------------

## 📁 Package Contents (Windows)

```
Vyom-0.7-Windows/
├── vyom.exe          # Vyom interpreter
├── install.bat      # Installer (adds Vyom to PATH)
├── uninstall.bat    # Uninstaller (clean removal)
├── README.md        # This file
└── examples/
    └── main.vy      # Progressive demo program
```

------------------------------------------------------------

## ⚡ Quick Start

### Install
1. Right‑click **install.bat**
2. Select **Run as administrator**
3. Close the terminal
4. Open a **new** terminal window

### Verify
```bash
vyom --version
```

Expected output:
```
Vyom v0.7
```

### Run Example
```bash
vyom C:\Vyom\examples\main.vy
```

------------------------------------------------------------

## ✨ Example

```vy
name = "Vyom"
print("Hello")
print(name)
```

Output:
```
Hello
Vyom
```

------------------------------------------------------------

## 📜 License

MIT License — see **LICENSE** file for details.

------------------------------------------------------------

Made with ❤️ by **Sanket Bharadwaj**

Vyom is designed to be understood, not rushed.
