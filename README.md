<p align="center">
  <img src="assets/VYOM-Banner.png" alt="Vyom banner" />
</p>

<h1 align="center">Vyom</h1>
<p align="center"><i>Minimal, explicit scripting language</i></p>

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

---

## 🎯 What is Vyom?

Vyom is a **small, indentation-based scripting language** written in C.

It is designed with a strong focus on **clarity, predictability, and control** —
without virtual machines, bytecode layers, or hidden execution models.

Every line of code executes exactly as written. The interpreter is transparent, readable, and direct.

### Core Principles
- ✅ **Clarity over cleverness** — readable code beats terse code
- ✅ **Explicit behavior** — no implicit type conversions or magic
- ✅ **Visible execution** — follow the code line-by-line
- ✅ **No hidden layers** — no VM, no bytecode, just direct execution

> If the code can be followed line by line, the design is correct.

---

## 🚀 Current Version

**Vyom v0.5 — Control Flow & Boolean Logic** ⭐

This release introduces **structured decision-making** with full boolean logic support:

### New in v0.5
- ✅ **Control flow**: `if / elif / else` with nested support
- ✅ **Comparisons**: `==` `!=` `<` `>` `<=` `>=`
- ✅ **Logical operators**: `and` `or` `not` with short-circuit evaluation
- ✅ **Parenthesized expressions**: `(a == b) and (c > d)`
- ✅ **Numeric truth model**: `0` is false, non-zero is true

### What Vyom v0.5 Can Do
```vy
# Variables and functions
x = 10
y = 20

def classify(n):
    if n > 0:
        return 1
    elif n < 0:
        return -1
    else:
        return 0

# Boolean logic
if x < y and y > 15:
    print "Condition met"
else:
    print "Condition not met"

# Function calls
result = classify(x)
print result
```

---

## ⚡ Quick Start

### Installation

#### **Windows**

**[Download Latest Release](https://github.com/Sanket-Bharadwaj/VYOM/releases/latest)**

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

### Your First Program

Create `hello.vy`:
```vy
# Hello World in Vyom
name = "World"
print name
```

Run it:
```bash
vyom hello.vy
```

---

## ✨ Features

### ✅ Currently Supported (v0.5)

| Feature | Syntax | Example |
|---------|--------|---------|
| **Variables** | `x = value` | `x = 10` |
| **Numbers** | Integer/decimal | `42` `3.14` |
| **Strings** | Double quotes | `"Hello"` |
| **Arithmetic** | `+ - * /` | `x + y * 2` |
| **Comparisons** | `== != < > <= >=` | `x > 10` |
| **Logical Ops** | `and or not` | `a and b` |
| **Functions** | `def name(params):` | `def add(a, b):` |
| **Return** | `return value` | `return a + b` |
| **Conditionals** | `if / elif / else` | `if x > 0:` |
| **Print** | `print expression` | `print x` |
| **Comments** | `#` | `# comment` |

### 🔐 Design Guarantees

- **No variable shadowing** — local variables cannot hide globals
- **Variables hold values** — numeric or string values decided at runtime
- **No implicit conversions** — what you see is what you get
- **Short-circuit evaluation** — `and`/`or` evaluate lazily
- **Clear error messages** — line numbers and context included

---

## 📚 Examples

### Functions with Logic
```vy
def max(a, b):
    if a > b:
        return a
    else:
        return b

print max(10, 20)  # 20
```

### Nested Conditions
```vy
score = 85

if score >= 90:
    print "Grade: A"
elif score >= 80:
    print "Grade: B"
elif score >= 70:
    print "Grade: C"
else:
    print "Grade: F"
```

### Boolean Logic
```vy
x = 10
y = 20

# Short-circuit AND
if x > 0 and y > 15:
    print "Both conditions true"

# Negation
if not (x == y):
    print "x and y are different"

# Parentheses for grouping
if (x < 5 or x > 8) and y > 10:
    print "Complex condition"
```

### Recursive Functions
```vy
def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

print factorial(5)  # 120
```

More examples in [`examples/`](examples/) directory.

### Print Semantics (v0.5)

Print outputs a value followed by a newline:
- `print x` — prints variable value
- `print "text"` — prints string literal
- `print expr` — prints evaluated expression
- `print` — prints blank line

Parentheses are not required.

### String Concatenation Warning

String concatenation works only between strings.
Numeric values are not automatically converted to strings.

### Recursion Limitations

Deep recursion may cause stack overflow due to limited call stack.

---

## 🧠 Philosophy

### Why Vyom Exists

Most scripting languages operate as black boxes. Code goes in, results come out, but the execution model remains hidden behind layers of abstraction, optimization, and implicit behavior.

**Vyom is different:**
- Every line executes exactly as written
- No hidden optimizations or rewrites
- No virtual machine translating your code
- The interpreter is readable, direct C code

### Design Foundation

Vyom prioritizes:
- **Explicit execution** — the code you read is the code that runs
- **Predictable behavior** — no surprises, no magic
- **Direct interpretation** — no intermediate layers
- **Clear semantics** — every construct has an obvious meaning

The language evolves incrementally. Capabilities expand version by version. Correctness comes first.

---

## 🛣️ Evolution & Roadmap

Vyom grows incrementally. Future versions will expand capabilities while maintaining clarity and predictability.

### v0.6 — Loops & Iteration
- `while` loops
- `for` loops with range iteration
- `break` and `continue`
- Loop control flow

### v0.7 — Collections
- Lists/arrays
- Indexing `list[0]`
- Basic list operations
- String indexing

### v0.8 — String Methods
- String manipulation
- Built-in string functions
- String formatting

### Beyond
- File I/O
- Module system
- Standard library
- Error handling (`try/catch`)

---

## 📊 Status

| Feature | v0.5 | Planned |
|---------|------|----------|
| Variables | ✅ | ✅ |
| Functions | ✅ | ✅ |
| Arithmetic | ✅ | ✅ |
| Comparisons | ✅ | ✅ |
| Logic (and/or/not) | ✅ | ✅ |
| if/elif/else | ✅ | ✅ |
| Loops | ❌ | ✅ |
| Lists | ❌ | ❌ |

---

## 🏗️ Project Structure

Example project layout (may vary by platform).

```
VYOM/
├── src/
│   └── vy.c           # Main interpreter (single file)
├── examples/
│   ├── main.vy        # Progressive demo
│   ├── test.vy        # Test examples
├── docs/
│   ├── SYNTAX.md      # Language reference
│   ├── DESIGN_NOTES.md
│   └── JOURNEY.md
├── assets/
│   └── colors.md
├── windows/
│   ├── vyom.exe
│   ├── install.bat
│   └── uninstall.bat
├── CHANGELOG.md       # Version history
├── LICENSE            # MIT License
└── README.md          # This file
```

---

## 🤝 Contributing

Vyom is authored and maintained with a clear vision. Contributions should align with core principles: clarity, explicit behavior, and direct execution.

If you find a bug or have an idea:
1. **Open an issue** to discuss it
2. **Keep changes focused** — small, purposeful contributions
3. **Preserve core principles** — clarity and explicit behavior
4. **Include examples** — demonstrate the change

Breaking changes require discussion before implementation.

---

## 📖 Documentation

- **[Syntax Reference](docs/syntax.md)** — Complete language specification
- **[User Guide](docs/guide.md)** — Tutorial and examples
- **[Changelog](CHANGELOG.md)** — Version history and roadmap
- **[Examples](examples/)** — Sample programs

---

## 🐛 Bug Reports

Found a bug? Please report it!

1. Check [existing issues](https://github.com/Sanket-Bharadwaj/VYOM/issues)
2. Open a new issue with:
   - Vyom version (`vyom --version`)
   - Operating system
   - Code that reproduces the bug
   - Expected vs actual behavior

---



---

## 🎓 Learning Resources

### Understanding Vyom
- Start with [`examples/main.vy`](examples/main.vy) — progressive demo
- Read the [syntax reference](docs/syntax.md)
- Explore the [C source code](src/vy.c) — it's only ~700 lines!

### Understanding Interpreters
Vyom's source is deliberately simple. Study it to learn:
- How to tokenize code
- How to evaluate expressions
- How function calls work
- How control flow is implemented
- How to report errors clearly

---

## 📜 License

MIT License — See [LICENSE](LICENSE) file for details.

**Build • Explore • Understand**

---

## 💬 Community

- **GitHub Issues** — Bug reports and feature requests
- **Discussions** — Questions and ideas
- **Email** — sanketbharadwaj15@gmail.com 

---

<p align="center">
  <i>A language that does what it looks like it does.</i>
</p>

<p align="center">
  Authored by <b>Sanket Bharadwaj</b>
</p>
