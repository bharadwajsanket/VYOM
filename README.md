<p align="center">
  <img src="assets/VYOM-Banner.png" alt="Vyom banner" />
</p>

<h1 align="center">Vyom</h1>
<p align="center"><i>Minimal, explicit scripting language</i></p>

<p align="center">
  Created and maintained by <b>Sanket Bharadwaj</b>
</p>

---

Vyom is a small, indentation-based scripting language written in C.

It is designed with a strong focus on **clarity, predictability, and control** —
without virtual machines, bytecode layers, or hidden execution models.

Vyom is not positioned as a replacement for existing languages.  
It exists as a **clear, honest execution model** you can fully understand.

---

## 🚀 Current Version

**Vyom v0.4 — Stable (Functions Complete)**

This release completes the function system and establishes Vyom as a usable,
explicit procedural language with local scope and predictable behavior.

---

## 🧠 Philosophy

- Clarity over cleverness  
- Explicit behavior over convenience  
- Simple syntax with visible execution  
- No VM, no bytecode, no hidden layers  

> If the code can be followed line by line, the design is correct.

---

## ✨ Feature Timeline

### v0.1 — Foundation
- Variables
- Numbers & strings
- `print`
- Indentation-based blocks
- Comments using `#`

### v0.2 — Stability Core
- Assignment using `=`
- Arithmetic expressions (`+ - * /`)
- String concatenation
- `if / else`
- `repeat`
- Human-friendly errors

### v0.3 — Types & Structure
- Optional typed variables (`int`, `double`, `string`)
- Type-locking
- Function definitions (`def`) — parsed only

### v0.4 — Functions Complete
- Function calls
- Parameters
- Return values
- Local scope
- Nested function calls
- Call stack
- Numeric-safe execution model

---

## ⚙️ Setup

### Windows
Download the latest release from GitHub:

https://github.com/Sanket-Bharadwaj/VYOM/releases

Includes:
- `vyom.exe`
- `install.bat`
- `uninstall.bat`
- `README.md`
- `examples/main.vy`

Run `install.bat` once and restart the terminal.

---

### macOS / Linux
Build from source:

```bash
gcc src/vy.c -o vyom
chmod +x vyom
sudo mv vyom /usr/local/bin/
```

---

## ▶️ Quick Example

```vy
def add(a, b):
    return a + b

print add(3, 4)
```

---

## ❌ Intentional Limitations (v0.4)

- No strings in expressions
- No conditionals yet (`if` planned next)
- No loops
- No arrays / lists
- No closures

These are deliberate design decisions.

---

## 🛣 Roadmap

### v0.5
- `if / else`
- Comparison operators
- Boolean logic

### v0.6
- Loops
- Iteration

---

## 📜 License

MIT License  
Build • Explore • Understand
