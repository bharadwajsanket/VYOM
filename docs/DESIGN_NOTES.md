# 🌌 Vyom — Design Notes

Vyom is a small, human-friendly scripting language written in C.  
Its design philosophy centers on **clarity, predictability, and deliberate evolution**.

Vyom is not built by chasing features.  
It is built by **making careful decisions and living with them**.

---

## 🎯 Core Principles

### ✔ Simplicity First
Vyom avoids complexity where a simple rule is sufficient.  
Language behavior should feel *obvious*, not surprising.

If something needs a long explanation, it probably does not belong.

---

### ✔ Human-Readable, Not Parser-Centric
Vyom syntax is shaped for people who read code — not for clever parsing tricks.

The goal is:
- code that explains itself
- errors that tell *what went wrong and why*
- behavior that matches intuition

---

### ✔ Stability Before Features
Features are added **slowly and deliberately**.

Vyom prefers:
- a small, stable core
- consistent semantics
- incremental growth

Over:
- rushed feature sets
- half-finished abstractions
- breaking changes

---

### ✔ Performance With Restraint
Vyom is implemented in C for efficiency and control.  
However, performance is never pursued at the cost of clarity.

Predictability > micro-optimizations  
Correctness > clever tricks

---

### ✔ Learning-Friendly by Design
Vyom is also a learning vehicle for:
- interpreter architecture
- expression evaluation
- control flow implementation
- scope and call stacks

Every feature should **teach something meaningful** — not hide complexity.

---

## 🧩 Key Design Decisions

### 🔹 Explicit Assignment (`=`)
Vyom uses a single, explicit assignment operator:

```vy
x = 10
```

No implicit declarations.  
No hidden bindings.

Values determine behavior — clearly and directly.

---

### 🔹 Lexical Scoping, No Shadowing
Variables follow lexical scoping rules.

Local variables **cannot shadow** outer scope variables.  
This is a conscious restriction to avoid:
- accidental bugs
- confusing scope resolution
- hidden name conflicts

Clarity beats flexibility here.

---

### 🔹 Minimal Type System (by choice)
Currently supported:
- Numbers (integer / floating-point)
- Strings (immutable)

No automatic type coercion is performed.

```vy
x = 5 + "10"   # runtime error
```

If types must interact, it should be **explicit**.

---

### 🔹 Numeric Truth Model
Vyom does not have a boolean type.

Truth is numeric:
- `0` → false
- non-zero → true

This keeps control flow simple, consistent, and easy to reason about.

---

### 🔹 Clear, Contextual Errors
Vyom errors aim to guide, not punish.

Instead of:
```
syntax error
```

Vyom prefers:
```
Error at line 5: variable "count" is not defined
```

Errors:
- include line numbers
- explain the reason
- stop execution immediately

No silent failures. No undefined behavior.

---

### 🔹 Indentation Defines Structure
Indentation determines blocks, similar to Python.

However, Vyom remains strict but predictable:
- indentation defines scope
- inconsistent structure results in clear errors
- no hidden block boundaries

---

## 🚧 Why Vyom Grows Slowly

Large feature jumps usually lead to:
- fragile implementations
- inconsistent semantics
- confusing rules for users

Vyom grows through **controlled milestones**:

- v0.1 → execution basics
- v0.2 → stability & ergonomics
- v0.3 → structure & functions
- v0.4 → real function execution
- v0.5 → logic & control flow
- v0.6 → loops & iteration

Each version answers **one core question** well.

---

## 🧭 Long-Term Vision

Vyom aims to feel like:
- Python-level readability
- Lua-level minimalism
- C-level transparency
- A distinct Vyom identity

The goal is not to replace existing languages —  
but to offer a **clean, honest, understandable execution model**.

---

## 📝 Notes for Contributors

When proposing a change, ask:

1. Does this improve clarity?
2. Can a beginner predict the behavior?
3. Does it simplify or complicate the interpreter?
4. Is the cost worth the capability?

Prefer:
- small changes
- explicit behavior
- documented intent

A language is a philosophy — not just syntax.

---

## 🌟 Closing Thought

Vyom will evolve.  
But it will never abandon its core values:

- simplicity
- honesty
- understandability

> “Do fewer things — but do them well.”
