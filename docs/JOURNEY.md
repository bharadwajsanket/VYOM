# 🌌 Vyom — The Journey (0.1 → 0.6)

This is not documentation.  
This is memory.

Vyom did not start as a programming language.  
It started as a thought:

> “Easy likhne me ho,  
> par andar se real ho.”

No buzzwords.  
No hype.  
No comparison games.

Bas ek file.  
Ek idea.  
Aur bahut saari galtiyaan.

---

## 🌱 v0.1 — The First Line

Everything started with one line:

```
a = 10
```

No types.  
No rules.  
No future plan.

Sirf ek sawaal tha:

**“Kya main kuch store kar sakta hoon?”**

Aur jab us value ko print kiya…  
wo feeling fake nahi thi.

Us moment pe Vyom language nahi tha.  
Wo **proof** tha — ki kuch ban sakta hai.

---

## 🧱 v0.2 — Things Started Breaking (And That Was Good)

Phir aaya:

```
print a
```

Indentation matter karne laga.  
Whitespace matter karne laga.  
Errors aane lage.

Kabhi code chala.  
Kabhi crash hua.

Aur ye acha tha.

Kyuki ab Vyom **jhoot nahi bol raha tha**.  
Agar kuch galat hai, to seedha bol raha tha.

No silent failure.  
No guessing.

---

## 🧩 v0.3 — Structure Without Confidence

Is phase me Vyom thoda serious banna chahta tha.

```
int a = 10
a = 1
```

Types optional the.  
Type locking exist karta tha.

Par execution…  
abhi khud pe bharosa nahi karta tha.

Functions likhe ja sakte the,  
par chal nahi sakte the.

Jaise plans — jo paper pe perfect ho,  
par real life me test nahi hue ho.

Yahin ek baat clear hui:

> Structure banana easy hai.  
> Correct behavior banana hard.

---

## 🔁 v0.4 — Functions Finally Breathed

Yahin se Vyom zinda hua.

Functions sirf syntax nahi rahe.  
Wo **run** hue.  
Wo **return** hue.  
Unka **scope** bana.

```
def add(a, b):
    return a + b
```

Local aur global ka farq clear hua.  
Shadowing ko jaan-bujh kar ban kiya gaya.

Is version me bugs aaye:

- stack related issues  
- return value confusion  
- scope leaks  

Par har bug ke baad Vyom aur **strong** hota gaya.

---

## ⚖️ v0.5 — Logic, Truth, and Control

Phir ek simple par dangerous sawaal aaya:

> “Truth kya hota hai?”

Answer simple rakha gaya:

```
0 = false  
non-zero = true
```

No `true`.  
No `false`.  
No magic keywords.

Comparisons aaye:
```
a < b
a == b
```

Logic aaya:
```
and
or
not
```

Aur finally control flow:

```
if
elif
else
```

Is phase me Vyom **bahut toota**.

- `elif` bina `if` ke  
- empty expressions  
- blocks do baar execute hona  
- return inside conditionals  

Par ek bhi issue ignore nahi kiya gaya.

Har problem ko:
**samjha → reproduce kiya → fix kiya**

Yahin Vyom ne seekha:
decision kaise lena hai.

---

## 🔄 v0.6 — Loops & Repetition (The Big One)

Ab tak Vyom ne seekha tha:

- kaise **store** kare (v0.1)
- kaise **print** kare (v0.2)
- kaise **define** kare (v0.3-0.4)
- kaise **decide** kare (v0.5)

Ab ek aur zaroori sawaal tha:

> "Kaise **repeat** kare?"

Kyuki jab repeat nahi kar sakte, to ek interpreter bas ek calculator ban jata hai.

Is phase me kaam tha:

**While loops** — simplest idea
```
while (i < 5):
    print i
    i = i + 1
```

Condition parentheses me.  
Colon zaroori.  
Har iteration pe condition check.

**C-style for loops** — familiar terrain
```
for (i = 0; i < 5; i = i + 1):
    print i
```

Init, condition, step — familiar formula.  
Semicolons zaroori.  
Exact expected.

**For-in-range loops** — sugar for counting
```
for i in range(5):
    print i
```

Iska matlab: 0 se 4 tak.  
Bas itna.  
Clear.

---

### The Hard Part (Jab Sab Toota)

Phir tha testing.

Aur **bugs mile**:

**Bug 1: The Step Corruption**

C-style for loop ka step buffer reuse ho raha tha.  
Second iteration me step ka value galat tha.

Symptoms:
```
for (i = 0; i < 5; i = i + 1):
    print i
```

Output nahi tha.  
Infinite loop tha.  
Ya crash.

Wajah: step pointer ko freed memory point kar raha tha.

**Solution**: Step ko separate buffer me copy karna pada.

---

**Bug 2: The If-Inside-Loop Mystery**

Ye zyada interesting bug tha.

```
i = 0
while (i < 5):
    if i == 2:
        print "TWO"
    i = i + 1
```

Condition evaluate ho raha tha correctly.  
Par block **always execute** ho raha tha.

Yaani:
- `i == 2` false ho raha tha  
- Lekin print ho raha tha

Debug output:
```
if condition evaluated to 0
found two
```

**What was happening**:

Main loop execution me jab `exec_statement("if i == 2:")` call hota tha,  
wo condition evaluate to 0 kar deta tha.  
Lekin phir block skip nahi hota tha.

**The Root Cause**:

Loop execution code me block end detection nahi tha.  
`exec_statement` ke baad `find_block_end()` call nahi ho raha tha.

**The Fix**:

Har nested control structure (if/while/for) ke liye explicit block skipping add kiya.

```c
if (!strncmp(s, "if", 2) && (s[2] == ' ' || s[2] == '\t')) {
    exec_statement(s);
    if (did_return) return;
    j = find_block_end(j);  // THIS LINE MATTERS
    continue;
}
```

---

### The Moment It Clicked

Tab tak **every loop** test kiya:

- While: ✓ Worked  
- C-style for: ✓ Worked  
- Range for: ✓ Worked  
- Nested loops: ✓ Worked  
- Conditionals in loops: ✓ Worked  
- Functions with loops: ✓ Worked  

Aur sab test files:

```
sum_1_to_5 = 15 ✓
factorial_5 = 120 ✓
fibonacci_sequence = correct ✓
nested_2x3 = perfect ✓
```

Ye moment tha jab realize hua:

> Vyom ab **complete** nahi hai,  
> par **sufficient** hai.

Three fundamental loops, nested support, proper scope handling.

---

## 📚 What We Actually Built

Ye v0.6 release nahi tha sirf.  
Ye ek **phase completion** tha.

**Code Phase**:
- 3 loop implementations (while, for, for-in-range)
- Nested loop support with proper block handling
- 2 critical bugs discovered aur fixed
- Full testing cycle

**Documentation Phase**:
- README.md updated (loop syntax + examples)
- CHANGELOG.md rewritten (v0.1 to v0.6 complete history)
- ROADMAP.md updated (v0.6 released, v0.7+ planned)
- windows/README.txt updated (feature table)
- SYNTAX.md updated (full loop reference)
- examples/main.vy updated (19 progressive examples)

**The Real Win**:

Har decision properly tested tha.  
Har edge case handle tha.  
Har error message clear tha.

---

## 🌟 Final Note

Vyom now has:

✓ Variables (dynamic typed)  
✓ Functions (with scope + return)  
✓ Comparisons (< > == != >= <=)  
✓ Logic operators (and, or, not)  
✓ Control flow (if, elif, else)  
✓ **Loops** (while, for, for-in-range) ← v0.6  

Kya complete hai?

Nahi.

Kya usable hai?

**Haan.**

Kya story interesting hai?

Ek aur version ke baad pata chalega.

---

Vyom **ban raha hai**.  
Aur ye sab milke **zaroori** tha.

— Sanket  
*25 January 2026*
