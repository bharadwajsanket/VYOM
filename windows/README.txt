🌌 Vyom — Minimal Learning-First Programming Language

This document describes Vyom v0.5 behavior only.

<p align="center">
  <strong>Created and maintained by Sanket Bharadwaj</strong>
</p>



⸻

Vyom is a small, indentation-based scripting language written in C, designed to
teach how programming languages actually work.

Vyom is not built to compete with Python or JavaScript.
It exists to understand them — through clarity, explicit behavior, and visible execution.

“If the code can be followed line by line, the design is correct.”

⸻

🚀 Current Version

Vyom v0.5 — Control Flow & Boolean Logic ⭐

New in v0.5
	•	✅ Control flow: if / elif / else with nested support
	•	✅ Comparisons: == != < > <= >=
	•	✅ Logical operators: and or not with short-circuit evaluation
	•	✅ Parenthesized expressions: (a == b) and (c > d)
	•	✅ Numeric truth model: 0 is false, non-zero is true

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

Vyom v0.5 (control flow, comparisons, logical ops)


⸻

🧩 Installer & Uninstaller (v0.5)

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
