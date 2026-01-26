🌌 Vyom — Minimal Learning-First Programming Language

This document describes Vyom v0.6 behavior.

<p align="center">
  <strong>Created and maintained by Sanket Bharadwaj</strong>
</p>



⸻

Vyom is a small, indentation-based scripting language written in C.

Vyom is designed for clarity: every line executes exactly as written,
with no hidden behavior or magic.

“If the code can be followed line by line, the design is correct.”

⸻

🚀 Current Version

Vyom v0.6 — Loops & Iteration ⭐

New in v0.6
	•	✅ While loops: while (condition):
	•	✅ C-style for loops: for (init; condition; step):
	•	✅ Range-based loops: for i in range(n):
	•	✅ Loop nesting and proper return handling
	•	✅ All previous features from v0.5 (control flow, logic)

⚠️ Vyom has NO boolean type.
Use numbers explicitly:
	•	0 → false
	•	non-zero → true

⸻

✨ Key Features

Feature	Example
Variables	x = 10
Functions	def add(a, b):
Arithmetic (numbers only)	x + y * 2
Comparisons	x > 10
Logic	a and b or not c
Conditionals	if x > 0:
While Loop	while (i < 5):
For Loop (C-style)	for (i = 0; i < 5; i = i + 1):
For Loop (Range)	for i in range(5):
Return values	return a + b
Local scope	Function-local variables
Nested calls	add(square(2), 5)

Design Guarantees
	•	✅ No variable shadowing — locals cannot hide globals
	•	✅ Explicit execution — every line visible, no magic
	•	✅ Clear errors — line numbers and context included
	•	✅ Predictable behavior — what you write is what runs

⸻

📁 Package Contents (Windows)

Vyom-0.5-Windows/
├── vyom.exe
├── install.bat
├── uninstall.bat
├── README.md
├── CHANGELOG.md
├── LICENSE
└── examples/
    └── main.vy


⸻

⚡ Quick Start (Windows)

1️⃣ Install

Automatic (recommended):
	1.	Extract ZIP
	2.	Right-click install.bat → Run as administrator
	3.	Confirm installation
	4.	Close terminal
	5.	Open a NEW terminal

Verify:

vyom --version

Expected:

Vyom v0.6 (loops, iteration, while, for)


⸻

🧩 Installer & Uninstaller (v0.6)

install.bat
	•	Installs Vyom to C:\Vyom
	•	Copies vyom.exe and examples/
	•	Safely updates User PATH (PowerShell-based)
	•	Avoids Windows PATH truncation bug
	•	Requires terminal restart

Run:

install.bat


⸻

uninstall.bat
	•	Removes C:\Vyom directory
	•	Deletes executable and examples
	•	Safely removes PATH entry only

Run:

C:\Vyom\uninstall.bat


⸻

▶️ Run Example

vyom C:\Vyom\examples\main.vy

Expected output:

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
1
0
1
1
1
1
2
1
-1
0
999


⸻

✍️ First Program (v0.5-valid)

name = "World"
print "Hello"
print name

def greet(person):
    print "Welcome"
    print person

greet("Vyom User")

score = 85

if score >= 90:
    print "A"
elif score >= 80:
    print "B"
else:
    print "C"


⸻

🧠 Execution Model
	1.	Read file + indentation
	2.	Collect def blocks
	3.	Execute top-level code
	4.	Each function call gets its own local scope

Scoping
	•	Globals: top-level
	•	Locals: function-only
	•	❌ No shadowing allowed

⸻

❌ Intentional Limitations (v0.5)
	•	❌ No loops
	•	❌ No arrays
	•	❌ No string concatenation
	•	❌ No string comparison
	•	❌ No file I/O

⸻

🛣️ Roadmap

v0.6
	•	while loops
	•	for loops
	•	break / continue

⸻

📜 License

MIT License

⸻


<p align="center">
<i>Vyom is not the most powerful language.<br>
But it might be the most understandable one.</i>
</p>


<p align="center">
<strong>Made with ❤️ by Sanket Bharadwaj</strong>
</p>
