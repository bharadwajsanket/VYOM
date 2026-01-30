# ======================================================
# 🌌 Vyom v0.7 — Official Progressive Demo
# Covers: variables, functions, logic, control flow, loops
# ======================================================

print("==============================================")
print("  Vyom v0.7 — Progressive Language Demo")
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
# 1️⃣4️⃣ Range-based for loop
# ------------------------------------------------------
print("▶ Range-based loop (0 to 2)")

for k in range(3):
    print(k)
print()

# ------------------------------------------------------
# 1️⃣5️⃣ Nested loops
# ------------------------------------------------------
print("▶ Nested loops (2 x 3)")

for outer in range(2):
    for inner in range(3):
        print("value =", outer * 3 + inner)
print()

# ------------------------------------------------------
# 1️⃣6️⃣ Conditional inside loop
# ------------------------------------------------------
print("▶ Conditional inside loop")

for num in range(10):
    if num == 5:
        print("Found:", num)
print()

# ------------------------------------------------------
# 1️⃣7️⃣ Loop inside function
# ------------------------------------------------------
print("▶ Loop inside function")

def sum_range(n):
    result = 0
    for (idx = 1; idx <= n; idx = idx + 1):
        result = result + idx
    return result

print("sum_range(10) =", sum_range(10))
print()

# ------------------------------------------------------
# 1️⃣8️⃣ Factorial using loop
# ------------------------------------------------------
print("▶ Factorial using loop")

def factorial(n):
    product = 1
    for (i = 1; i <= n; i = i + 1):
        product = product * i
    return product

print("factorial(5) =", factorial(5))
print()

# ------------------------------------------------------
# 1️⃣9️⃣ Fibonacci using while loop
# ------------------------------------------------------
print("▶ Fibonacci using while loop")

def fibonacci(n):
    a = 0
    b = 1
    count = 0
    while (count < n):
        temp = a + b
        a = b
        b = temp
        count = count + 1
    return a

print("fibonacci(8) =", fibonacci(8))
print()

# ------------------------------------------------------
# 🔚 End
# ------------------------------------------------------
print("==============================================")
print("  End of Vyom v0.7 Demo")
print("==============================================")
print(999)