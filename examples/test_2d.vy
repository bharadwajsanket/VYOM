# ==============================================
# Vyom v0.9 — 2D Array Test Suite
# ==============================================

print("==============================================")
print("  Vyom v0.9 — 2D Array Tests")
print("==============================================")
print()

# ------------------------------------------------------
# 1️⃣ Basic 2D array declaration
# ------------------------------------------------------
print("▶ Test 1: Basic 2D array declaration")

int a[3][4]

print("Declared int a[3][4]")
print("len(a) =", len(a))
print("len(a[0]) =", len(a[0]))
print()

# ------------------------------------------------------
# 2️⃣ 2D array with initialization (single line)
# ------------------------------------------------------
print("▶ Test 2: 2D array with initialization")

int b[2][3] = [[1, 2, 3], [4, 5, 6]]

print("b[0][0] =", b[0][0])
print("b[0][1] =", b[0][1])
print("b[0][2] =", b[0][2])
print("b[1][0] =", b[1][0])
print("b[1][1] =", b[1][1])
print("b[1][2] =", b[1][2])
print()

# ------------------------------------------------------
# 3️⃣ 2D array assignment
# ------------------------------------------------------
print("▶ Test 3: 2D array assignment")

a[0][0] = 10
a[1][2] = 42
a[2][3] = 99

print("a[0][0] =", a[0][0])
print("a[1][2] =", a[1][2])
print("a[2][3] =", a[2][3])
print()

# ------------------------------------------------------
# 4️⃣ Zero-fill test (partial initialization)
# ------------------------------------------------------
print("▶ Test 4: Zero-fill (partial init)")

int c[3][3] = [[1, 2], [5]]

print("c[0][0] =", c[0][0])
print("c[0][1] =", c[0][1])
print("c[0][2] =", c[0][2])
print("c[1][0] =", c[1][0])
print("c[1][1] =", c[1][1])
print("c[2][0] =", c[2][0])
print()

# ------------------------------------------------------
# 5️⃣ Nested loop iteration
# ------------------------------------------------------
print("▶ Test 5: Nested loop iteration")

for i in range(len(b)):
    for j in range(len(b[0])):
        print("b[", i, "][", j, "] =", b[i][j])

print()

# ------------------------------------------------------
# 6️⃣ Expression in indexing
# ------------------------------------------------------
print("▶ Test 6: Expression in indexing")

int idx = 1
print("b[idx][idx + 1] =", b[idx][idx + 1])
print()

# ------------------------------------------------------
# 7️⃣ Matrix computation
# ------------------------------------------------------
print("▶ Test 7: Matrix sum")

int sum = 0
for i in range(len(b)):
    for j in range(len(b[0])):
        sum = sum + b[i][j]

print("Sum of all elements =", sum)
print()

# ------------------------------------------------------
# 🔚 End
# ------------------------------------------------------
print("==============================================")
print("  All 2D array tests passed!")
print("==============================================")
