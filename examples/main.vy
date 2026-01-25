# =========================================
# Vyom v0.5 — main.vy
# Official Progressive Demo
# Control Flow + Comparisons
# NO SHADOWING VERSION
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
print classify(-5)
print classify(0)


# 1️⃣2️⃣ End marker
print 999
