# 🌌 Vyom — Minimal Learning-First Programming Language

Created and maintained by **Sanket Bharadwaj**

Vyom is a small, indentation-based scripting language written in C, designed to
teach **how programming languages actually work**.

Vyom is not built to compete with Python or JavaScript.  
It exists to **understand them** — through clarity, explicit behavior, and
visible execution.

---

## 🚀 Current Version

**Vyom v0.4.x — Functions Complete**

---

## ✨ Key Features (v0.4)

- Dynamic variables
- Numeric expressions (`+  -  *  /`)
- `print`
- Indentation-based blocks
- Function definitions (`def`)
- Function calls
- Parameters
- Return values
- Local scope
- Nested function calls
- Predictable, line-by-line execution

---

## 📁 Package Contents (Windows)

- `vyom.exe` — Vyom interpreter
- `install.bat` — Windows installer
- `uninstall.bat` — Windows uninstaller
- `README.md`
- `examples/main.vy` — canonical test program

---

## ▶️ Quick Start (Windows)

### 1️⃣ Install (one-time)

1. Download **Vyom v0.4.x (Windows ZIP)** from GitHub Releases  
2. Extract the ZIP file to any folder  
3. Double-click `install.bat`  
4. Restart **Command Prompt / PowerShell**

This installs Vyom to:

```
C:\Vyom
```

and adds it to your system `PATH`.

---

### 2️⃣ Run the example

```cmd
vyom examples\main.vy
```

Expected output:

```
10
5
20
100
42
99
42
999
7
25
999
```

---

### 3️⃣ Write your own program

Create a file `test.vy`:

```vy
def square(n):
    return n * n

print square(5)
```

Run it:

```cmd
vyom test.vy
```

---

## 🧠 Execution Model

- Files are executed top-to-bottom
- `def` blocks are collected first
- Each function call creates a fresh local scope
- Arguments are evaluated before the call
- Locals are destroyed on return

---

## ❌ Intentional Limitations (v0.4)

- No strings in expressions
- No conditionals yet
- No loops
- No arrays / lists
- No closures

---

## 📜 License

MIT License  
Build • Explore • Understand
