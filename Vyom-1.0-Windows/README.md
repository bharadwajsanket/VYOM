# 🌌 Vyom v1.0 — Windows Release

Created and maintained by **Sanket Bharadwaj**

A minimal, explicit scripting language with transparent execution semantics.  
Written in ~2700 lines of C. No dependencies. No magic.

> "If the code can be followed line by line, the design is correct."

---

## 📁 Package Contents

```
Vyom-1.0-Windows/
├── vyom.exe          # Vyom interpreter
├── install.bat       # Installer (adds Vyom to PATH)
├── uninstall.bat     # Uninstaller (clean removal)
├── README.md         # This file
└── examples/
    └── main.vy       # Demo program
```

---

## ⚡ Installation

### Install
1. Right-click **install.bat**
2. Select **Run as administrator**
3. Restart your terminal

### Verify
```bash
vyom --version
```

Expected output:
```
Vyom v1.0
```

### Uninstall
1. Right-click **uninstall.bat**
2. Select **Run as administrator**

---

## ✨ Language Features

### Core
- Variables with optional typing (`int`, `double`, `string`)
- `const` immutable variables
- Lexical scoping (no shadowing)

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `//` (integer division)
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Logical: `and`, `or`, `not`

### Control Flow
- `if` / `elif` / `else`
- `while`, `for`, `for-in-range`
- `break`, `continue`

### Data Structures
- 1D Arrays: `int a[5]`
- 2D Arrays: `int grid[3][4]`
- Bounds checking always on

### Functions
- `def` with parameters and return
- Recursion supported
- `print()`, `len()`, `exit()`

---

## 🚀 Example

```vy
# Hello World
name = "Vyom"
print("Hello,", name)

# Integer Division
print(7 // 3)   # 2

# Arrays
int nums[3] = [1, 2, 3]
print(len(nums))
```

---

## ⚠️ Windows Security Notice

Windows may show "Smart App Control blocked this file" for unsigned scripts.

**To proceed:**
1. Right-click `install.bat`
2. Properties → Check **Unblock**
3. Run again as Administrator

---

## 📚 Documentation

- [Full README](https://github.com/Sanket-Bharadwaj/VYOM)
- [Syntax Guide](https://github.com/Sanket-Bharadwaj/VYOM/blob/main/docs/SYNTAX.md)
- [Changelog](https://github.com/Sanket-Bharadwaj/VYOM/blob/main/CHANGELOG.md)

---

## 📜 License

MIT License

---

Made with ❤️ by **Sanket Bharadwaj**

**Build with clarity. Execute with transparency.** 🌌
