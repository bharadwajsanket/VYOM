# ======================================================
# 🌌 Vyom v0.9 — Official Progressive Demo
# Covers: core language + fixed arrays + string indexing + 2D arrays
# ======================================================

print("==============================================")
print("  Vyom v0.9 — Progressive Language Demo")
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

def show(val):
    print("Value:", val)

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

def add_nums(p, q):
    return p + q

print("add_nums(3, 4) =", add_nums(3, 4))
print()

# ------------------------------------------------------
# 7️⃣ Nested function calls
# ------------------------------------------------------
print("▶ Nested function calls")

def square(n):
    return n * n

def sum_of_squares(p, q):
    return square(p) + square(q)

print("sum_of_squares(3, 4) =", sum_of_squares(3, 4))
print()

# ------------------------------------------------------
# 8️⃣ Comparisons
# ------------------------------------------------------
print("▶ Comparisons")

num1 = 10
num2 = 20

print("num1 < num2  =", num1 < num2)
print("num1 == num2 =", num1 == num2)
print("num1 != num2 =", num1 != num2)
print()

# ------------------------------------------------------
# 9️⃣ Logical operators
# ------------------------------------------------------
print("▶ Logical operators")

print("num1 < num2 and num2 > 15 =", num1 < num2 and num2 > 15)
print("num1 > num2 or num2 == 20 =", num1 > num2 or num2 == 20)
print("not (num1 == num2)        =", not (num1 == num2))
print()

# ------------------------------------------------------
# 🔟 if / elif / else control flow
# ------------------------------------------------------
print("▶ Conditional control flow")

score = 72
print("Score =", score)

if score >= 90:
    print("Grade: A")

if score >= 60:
    if score < 90:
        print("Grade: B")

if score < 60:
    print("Grade: C")

print()

# ------------------------------------------------------
# 1️⃣1️⃣ Return inside condition
# ------------------------------------------------------
print("▶ Return inside condition")

def classify(n):
    if n > 0:
        return 1
    if n < 0:
        return -1
    return 0

print("classify(10) =", classify(10))
print("classify(-5) =", classify(-5))
print("classify(0)  =", classify(0))
print()

# ------------------------------------------------------
# 1️⃣2️⃣ While loop
# ------------------------------------------------------
print("▶ While loop countdown")

counter = 5
while (counter > 0):
    print(counter)
    counter = counter - 1
print()

# ------------------------------------------------------
# 1️⃣3️⃣ C-style for loop
# ------------------------------------------------------
print("▶ C-style for loop (sum 1 to 5)")

sum_total = 0
for (j = 1; j <= 5; j = j + 1):
    sum_total = sum_total + j

print("Sum =", sum_total)
print()

# ------------------------------------------------------
# 1️⃣4️⃣ Range-based for loop (v0.8 supports step)
# ------------------------------------------------------
print("▶ Range-based loop with step")

for k in range(0, 6, 2):
    print(k)
print()

# ------------------------------------------------------
# 1️⃣5️⃣ Fixed-size numeric arrays (v0.8)
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

int sqrs[5]

for m in range(len(sqrs)):
    sqrs[m] = m * m

for m in range(len(sqrs)):
    print("sqrs[", m, "] =", sqrs[m])
print()

# ------------------------------------------------------
# 1️⃣9️⃣ 2D Arrays - Declaration (NEW in v0.9)
# ------------------------------------------------------
print("▶ 2D array declaration")

int matrix[3][4]

print("Declared int matrix[3][4]")
print("len(matrix) =", len(matrix))
print("len(matrix[0]) =", len(matrix[0]))
print()

# ------------------------------------------------------
# 2️⃣0️⃣ 2D Arrays - Initialization (NEW in v0.9)
# ------------------------------------------------------
print("▶ 2D array with initialization")

int grid[2][3] = [[1, 2, 3], [4, 5, 6]]

print("grid[0][0] =", grid[0][0])
print("grid[0][1] =", grid[0][1])
print("grid[0][2] =", grid[0][2])
print("grid[1][0] =", grid[1][0])
print("grid[1][1] =", grid[1][1])
print("grid[1][2] =", grid[1][2])
print()

# ------------------------------------------------------
# 2️⃣1️⃣ 2D Arrays - Assignment (NEW in v0.9)
# ------------------------------------------------------
print("▶ 2D array element assignment")

matrix[0][0] = 100
matrix[1][2] = 42
matrix[2][3] = 999

print("matrix[0][0] =", matrix[0][0])
print("matrix[1][2] =", matrix[1][2])
print("matrix[2][3] =", matrix[2][3])
print()

# ------------------------------------------------------
# 2️⃣2️⃣ 2D Arrays - Nested loop iteration (NEW in v0.9)
# ------------------------------------------------------
print("▶ Nested loop over 2D array")

for row in range(len(grid)):
    for col in range(len(grid[0])):
        print("grid[", row, "][", col, "] =", grid[row][col])
print()

# ------------------------------------------------------
# 2️⃣3️⃣ 2D Arrays - Matrix sum (NEW in v0.9)
# ------------------------------------------------------
print("▶ 2D array matrix sum")

int sum_val = 0
for row in range(len(grid)):
    for col in range(len(grid[0])):
        sum_val = sum_val + grid[row][col]

print("Sum of all elements =", sum_val)
print()

# ------------------------------------------------------
# 🔚 End
# ------------------------------------------------------
print("==============================================")
print("  End of Vyom v0.9 Demo")
print("==============================================")
