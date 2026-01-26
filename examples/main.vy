# =========================================
# Vyom v0.6 — main.vy
# Official Progressive Demo
# Loops & Iteration
# =========================================

# 1️⃣ Dynamic variables
x = 10
y = 5

print x
print y


# 2️⃣ Re-assignment
x = 20
print x


# 3️⃣ Basic function
def greet():
    print 100

greet()


# 4️⃣ Function with parameters
def show(v):
    print v

show(42)
show(99)


# 5️⃣ Local scope demo (NO SHADOWING)
temp = 999

def local_demo():
    local_temp = 42
    print local_temp

local_demo()
print temp


# 6️⃣ Function with return
def add(a, b):
    return a + b

result = add(3, 4)
print result


# 7️⃣ Nested function calls
def square(n):
    return n * n

def sum_of_squares(a, b):
    return square(a) + square(b)

print sum_of_squares(3, 4)


# 8️⃣ Comparisons
a = 10
b = 20

print a < b
print a == b
print a != b


# 9️⃣ Logical operators
print a < b and b > 15
print a > b or b == 20
print not (a == b)


# 🔟 if / elif / else control flow
score = 72

if score >= 90:
    print 1
elif score >= 60:
    print 2
else:
    print 3


# 1️⃣1️⃣ Return inside condition
def classify(n):
    if n > 0:
        return 1
    elif n < 0:
        return -1
    else:
        return 0

print classify(10)


# 1️⃣2️⃣ While loop — count down
i = 5
while (i > 0):
    print i
    i = i - 1


# 1️⃣3️⃣ C-style for loop — sum
total = 0
for (j = 1; j <= 5; j = j + 1):
    total = total + j
print total


# 1️⃣4️⃣ Range-based for loop — simple iteration
for k in range(3):
    print k


# 1️⃣5️⃣ Nested loops
for outer in range(2):
    for inner in range(3):
        print outer * 3 + inner


# 1️⃣6️⃣ Conditional inside loop
for num in range(10):
    if num == 5:
        print num


# 1️⃣7️⃣ Loop inside function
def sum_range(n):
    result = 0
    for (idx = 1; idx <= n; idx = idx + 1):
        result = result + idx
    return result

print sum_range(10)


# 1️⃣8️⃣ Factorial using loop
def factorial(n):
    product = 1
    for (i = 1; i <= n; i = i + 1):
        product = product * i
    return product

print factorial(5)


# 1️⃣9️⃣ Fibonacci using while loop
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

print fibonacci(8)

print classify(-5)
print classify(0)


# 1️⃣2️⃣ End marker
print 999
