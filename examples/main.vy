# ======================================================
# 🌌 Vyom v0.8 — Official Progressive Demo
# Covers: core language + fixed arrays + string indexing
# ======================================================

print("==============================================")
print("  Vyom v0.8 — Progressive Language Demo")
print("==============================================")
print()

# ------------------------------------------------------
# 1️⃣ Dynamic variables
# ------------------------------------------------------
print("▶ Dynamic variables")

x = 10
y = 5

print("x =", x)
print("y =", y)
print()

# ------------------------------------------------------
# 2️⃣ Re-assignment
# ------------------------------------------------------
print("▶ Re-assignment")

x = 20
print("x after reassignment =", x)
print()

# ------------------------------------------------------
# 3️⃣ Basic function
# ------------------------------------------------------
print("▶ Basic function")

def greet():
    print("Hello from greet()")

greet()
print()

# ------------------------------------------------------
# 4️⃣ Function with parameters
# ------------------------------------------------------
print("▶ Function with parameters")

def show(v):
    print("Value:", v)

show(42)
show(99)
print()

# ------------------------------------------------------
# 5️⃣ Local scope demo (NO SHADOWING)
# ------------------------------------------------------
print("▶ Local vs global scope")

temp = 999

def local_demo():
    local_temp = 42
    print("Inside function, local_temp =", local_temp)

local_demo()
print("Outside function, temp =", temp)
print()

# ------------------------------------------------------
# 6️⃣ Function with return value
# ------------------------------------------------------
print("▶ Function return value")

def add(a, b):
    return a + b

print("add(3, 4) =", add(3, 4))
print()

# ------------------------------------------------------
# 7️⃣ Nested function calls
# ------------------------------------------------------
print("▶ Nested function calls")

def square(n):
    return n * n

def sum_of_squares(a, b):
    return square(a) + square(b)

print("sum_of_squares(3, 4) =", sum_of_squares(3, 4))
print()

# ------------------------------------------------------
# 8️⃣ Comparisons
# ------------------------------------------------------
print("▶ Comparisons")

a = 10
b = 20

print("a < b  =", a < b)
print("a == b =", a == b)
print("a != b =", a != b)
print()

# ------------------------------------------------------
# 9️⃣ Logical operators
# ------------------------------------------------------
print("▶ Logical operators")

print("a < b and b > 15 =", a < b and b > 15)
print("a > b or b == 20 =", a > b or b == 20)
print("not (a == b)     =", not (a == b))
print()

# ------------------------------------------------------
# 🔟 if / elif / else control flow
# ------------------------------------------------------
print("▶ Conditional control flow")

score = 72
print("Score =", score)

if score >= 90:
    print("Grade: A")
elif score >= 60:
    print("Grade: B")
else:
    print("Grade: C")
print()

# ------------------------------------------------------
# 1️⃣1️⃣ Return inside condition
# ------------------------------------------------------
print("▶ Return inside condition")

def classify(n):
    if n > 0:
        return 1
    elif n < 0:
        return -1
    else:
        return 0

print("classify(10) =", classify(10))
print("classify(-5) =", classify(-5))
print("classify(0)  =", classify(0))
print()

# ------------------------------------------------------
# 1️⃣2️⃣ While loop
# ------------------------------------------------------
print("▶ While loop countdown")

i = 5
while (i > 0):
    print(i)
    i = i - 1
print()

# ------------------------------------------------------
# 1️⃣3️⃣ C-style for loop
# ------------------------------------------------------
print("▶ C-style for loop (sum 1 to 5)")

total = 0
for (j = 1; j <= 5; j = j + 1):
    total = total + j

print("Sum =", total)
print()

# ------------------------------------------------------
# 1️⃣4️⃣ Range-based for loop (v0.8 supports step)
# ------------------------------------------------------
print("▶ Range-based loop with step")

for k in range(0, 6, 2):
    print(k)
print()

# ------------------------------------------------------
# 1️⃣5️⃣ Fixed-size numeric arrays (NEW in v0.8)
# ------------------------------------------------------
print("▶ Fixed-size numeric arrays")

int nums[5] = [1, 2, 3, 4, 5]

print("nums[0] =", nums[0])
print("nums[4] =", nums[4])
print("len(nums) =", len(nums))

nums[2] = 99
print("nums[2] after update =", nums[2])
print()

# ------------------------------------------------------
# 1️⃣6️⃣ String arrays
# ------------------------------------------------------
print("▶ String arrays")

string names[3] = ["Sanket", "Vyom", "Lang"]

print("names[0] =", names[0])
print("names[1] =", names[1])
print("len(names) =", len(names))
print()

# ------------------------------------------------------
# 1️⃣7️⃣ String indexing (read-only)
# ------------------------------------------------------
print("▶ String indexing")

word = "Vyom"
print("word =", word)
print("word[0] =", word[0])
print("word[1] =", word[1])
print("len(word) =", len(word))
print()

# ------------------------------------------------------
# 1️⃣8️⃣ Array + loop together
# ------------------------------------------------------
print("▶ Looping over array indices")

int squares[5]

for idx in range(len(squares)):
    squares[idx] = idx * idx

for idx in range(len(squares)):
    print("squares[", idx, "] =", squares[idx])
print()

# ------------------------------------------------------
# 🔚 End
# ------------------------------------------------------
print("==============================================")
print("  End of Vyom v0.8 Demo")
print("==============================================")