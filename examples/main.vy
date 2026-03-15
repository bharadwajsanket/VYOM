# ================================================================
#
#  ██╗   ██╗██╗   ██╗ ██████╗ ███╗   ███╗
#  ██║   ██║╚██╗ ██╔╝██╔═══██╗████╗ ████║
#  ██║   ██║ ╚████╔╝ ██║   ██║██╔████╔██║
#  ╚██╗ ██╔╝  ╚██╔╝  ██║   ██║██║╚██╔╝██║
#   ╚████╔╝    ██║   ╚██████╔╝██║ ╚═╝ ██║
#    ╚═══╝     ╚═╝    ╚═════╝ ╚═╝     ╚═╝
#
#  A minimal, explicit scripting language.
#  https://getvyom.vercel.app
#
#  Run this file:
#    vyom examples/main.vy
#
# ================================================================


# ----------------------------------------------------------------
#  1. VARIABLES
# ----------------------------------------------------------------

int    age    = 21
double height = 5.11
string lang   = "Vyom"
const int MAX = 100

print("=== Variables ===")
print("Language :", lang)
print("Age      :", age)
print("Height   :", height)
print("Max      :", MAX)


# ----------------------------------------------------------------
#  2. ARITHMETIC
# ----------------------------------------------------------------

print("")
print("=== Arithmetic ===")

int a = 17
int b = 5

print("a + b  =", a + b)
print("a - b  =", a - b)
print("a * b  =", a * b)
print("a / b  =", a / b)
print("a // b =", a // b)
print("a % b  =", a % b)


# ----------------------------------------------------------------
#  3. STRINGS
# ----------------------------------------------------------------

print("")
print("=== Strings ===")

string word = "VYOM"
print("Word   :", word)
print("Length :", len(word))
print("First  :", word[0])
print("Last   :", word[3])

string rev = ""
for si in range(len(word) - 1, -1, -1):
    rev = concat(rev, word[si])
print("Reverse:", rev)


# ----------------------------------------------------------------
#  4. IF / ELIF / ELSE
# ----------------------------------------------------------------

print("")
print("=== Conditionals ===")

int score = 85

if score >= 90:
    print("Grade: A")
elif score >= 80:
    print("Grade: B")
elif score >= 70:
    print("Grade: C")
else:
    print("Grade: F")

int x = 42
if x % 2 == 0:
    print(x, "is even")
else:
    print(x, "is odd")


# ----------------------------------------------------------------
#  5. LOOPS
# ----------------------------------------------------------------

print("")
print("=== Loops ===")

print("Countdown:")
int n = 5
while (n > 0):
    print(n)
    n = n - 1

print("Range 1 to 5:")
for i in range(1, 6):
    print(i)

print("Step by 2:")
for i in range(0, 10, 2):
    print(i)

print("C-style for:")
for (int ci = 0; ci < 4; ci = ci + 1):
    print(ci)

print("Skip 3, stop at 6:")
for i in range(10):
    if i == 3:
        continue
    if i == 6:
        break
    print(i)


# ----------------------------------------------------------------
#  6. FUNCTIONS
# ----------------------------------------------------------------

print("")
print("=== Functions ===")

def add(a, b):
    return a + b

def power(base, exp):
    int result = 1
    for pi in range(exp):
        result = result * base
    return result

def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

def is_prime(n):
    if n < 2:
        return 0
    for di in range(2, n):
        if n % di == 0:
            return 0
    return 1

print("add(7, 3)    =", add(7, 3))
print("power(2, 8)  =", power(2, 8))
print("factorial(6) =", factorial(6))
print("is_prime(17) =", is_prime(17))
print("is_prime(18) =", is_prime(18))


# ----------------------------------------------------------------
#  7. ARRAYS
# ----------------------------------------------------------------

print("")
print("=== Arrays ===")

int scores[5] = [92, 78, 85, 90, 76]
print("Scores:", scores[0], scores[1], scores[2], scores[3], scores[4])

int best = scores[0]
for ai in range(1, len(scores)):
    if scores[ai] > best:
        best = scores[ai]
print("Best  :", best)

int total = 0
for ai in range(len(scores)):
    total = total + scores[ai]
print("Avg   :", total / len(scores))

string days[7] = ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]
print("Days  :", days[0], days[1], days[2], days[3], days[4], days[5], days[6])


# ----------------------------------------------------------------
#  8. 2D ARRAYS
# ----------------------------------------------------------------

print("")
print("=== 2D Arrays ===")

int identity[3][3]
for row in range(3):
    for col in range(3):
        if row == col:
            identity[row][col] = 1
        else:
            identity[row][col] = 0

print("Identity matrix 3x3:")
for row in range(3):
    print(identity[row][0], identity[row][1], identity[row][2])

int mul[4][4]
for row in range(4):
    for col in range(4):
        mul[row][col] = (row + 1) * (col + 1)

print("Multiplication table 4x4:")
for row in range(4):
    print(mul[row][0], mul[row][1], mul[row][2], mul[row][3])


# ----------------------------------------------------------------
#  9. BUILTINS
# ----------------------------------------------------------------

print("")
print("=== Builtins ===")

print("ascii(A) =", ascii("A"))
print("char(65) =", char(65))

string alphabet = ""
for ai in range(26):
    alphabet = concat(alphabet, char(65 + ai))
print("Alphabet:", alphabet)

# ROT13 using ascii / char / concat
def rot13(c):
    int code = ascii(c)
    if code >= 65 and code <= 90:
        return char((code - 65 + 13) % 26 + 65)
    return c

string msg    = "VYOM"
string cipher = ""
for ri in range(len(msg)):
    cipher = concat(cipher, rot13(msg[ri]))

string back = ""
for ri in range(len(cipher)):
    back = concat(back, rot13(cipher[ri]))

print("ROT13:", msg, "->", cipher, "->", back)


# ----------------------------------------------------------------
#  10. FIBONACCI
# ----------------------------------------------------------------

print("")
print("=== Fibonacci ===")

def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

int fibs[10]
for fi in range(10):
    fibs[fi] = fib(fi)

print("First 10 terms:")
for fi in range(10):
    print(fi, "->", fibs[fi])


# ----------------------------------------------------------------

print("")
print("================================================")
print("  Vyom v1.0  --  getvyom.vercel.app")
print("================================================")
