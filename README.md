<p align="center">
  <img src="assets/VYOM-Banner.png" alt="Vyom banner" />
</p>

<h1 align="center">Vyom</h1>
<p align="center"><i>Minimal, learning-first scripting language</i></p>

<p align="center">
  Created and maintained by <b>Sanket Bharadwaj</b>
</p>

---

Vyom is a small, indentation-based scripting language written in C, designed with a strong focus on  
<b>simplicity, clarity, and predictable behavior</b>.

Vyom is a <b>learning-first language</b> — built to help you understand how interpreters work,  
how syntax is processed, and how programming languages evolve from a minimal core.

Vyom is intentionally small, explicit, and honest.

---

## 🚀 Current Version

<b>Vyom v0.4 — Stable (Functions Complete)</b>

This release completes the function system and turns Vyom into a fully usable procedural language,
while preserving its learning-first philosophy.

---

## 🧠 Philosophy

- Clarity over cleverness  
- Learning-first, not feature-first  
- Predictable execution  
- No hidden magic  

> If it teaches something, feels good to use, and doesn’t overcomplicate — it’s worth shipping.

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

## ✨ Features (v0.4)

### Variables
```vy
x = 10
y = 5
```

### Functions
```vy
def greet():
    print 100

greet()
```

### Parameters
```vy
def show(v):
    print v

show(42)
```

### Return values
```vy
def add(a, b):
    return a + b

print add(3, 4)
```

### Local Scope
```vy
temp = 999

def local_demo():
    temp = 42
    print temp

local_demo()
print temp
```

### Nested Calls
```vy
def square(n):
    return n * n

def sum_of_squares(a, b):
    return square(a) + square(b)

print sum_of_squares(3, 4)
```

---

## 🧪 Canonical Test (v0.4)

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

## ⬇️ Download

### Windows

- <b>Vyom v0.4 (Stable)</b>  
  https://github.com/Sanket-Bharadwaj/VYOM/releases

Includes:
- vyom.exe
- install.bat
- uninstall.bat
- README.md
- examples/main.vy

---

## 🛠 Build from Source

### macOS / Linux
```bash
gcc src/vy.c -o vyom
chmod +x vyom
sudo mv vyom /usr/local/bin/
```

---

## ▶️ Run

```bash
vyom examples/main.vy
```

---

## ❌ Intentional Limitations (v0.4)

- No strings in expressions
- No conditionals yet (`if` planned in v0.5)
- No loops
- No arrays / lists
- No closures

These are design choices — not bugs.

---

## 🛣 Roadmap

### v0.5 (planned)
- `if / else`
- Comparison operators
- Boolean logic

### v0.6
- Loops
- Iteration

---

## 📜 License

MIT License  
Learn • Build • Explore 🚀
