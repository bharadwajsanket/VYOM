# =========================================
# Vyom v0.4 — main.vy
# Official Progressive Demo (NUMERIC SAFE)
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


# 5️⃣ Local scope demo
temp = 999

def local_demo():
    temp = 42
    print temp

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


# 8️⃣ End
print 999