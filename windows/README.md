🌌 Vyom — Minimal Learning-First Programming Language

This document describes Vyom v0.6 behavior.

Created and maintained by Sanket Bharadwaj

Vyom is a small, indentation-based scripting language written in C.

Vyom is designed for clarity: every line executes exactly as written,
with no hidden behavior or magic.

“If the code can be followed line by line, the design is correct.”

------------------------------------------------------------

🚀 Current Version

Vyom v0.6 — Loops & Iteration ⭐

New in v0.6
- While loops: while (condition):
- C-style for loops: for (init; condition; step):
- Range-based loops: for i in range(n):
- Loop nesting with correct scope handling
- Proper return behavior inside loops
- All features from v0.5 (functions, logic, control flow)

Truth model:
0 → false  
non-zero → true  

------------------------------------------------------------

📁 Package Contents (Windows)

Vyom-0.6-Windows/
├── vyom.exe
├── install.bat
├── uninstall.bat
├── README.md
├── CHANGELOG.md
├── LICENSE
└── examples/
    └── main.vy

------------------------------------------------------------

⚡ Quick Start

Install:
- Right click install.bat → Run as administrator
- Restart terminal

Verify:
vyom --version

------------------------------------------------------------

📜 License

MIT License

Made with ❤️ by Sanket Bharadwaj
