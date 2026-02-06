# ======================================================
# 🧪 Vyom v0.9 — Comprehensive Stress Test
# Real-world algorithms using 2D arrays
# ======================================================

print("==============================================")
print("  Vyom v0.9 — Algorithm Stress Test")
print("==============================================")
print()

# ======================================================
# TEST 1: Matrix Addition
# ======================================================
print("▶ TEST 1: Matrix Addition")

int mat_A[3][3] = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
int mat_B[3][3] = [[9, 8, 7], [6, 5, 4], [3, 2, 1]]
int mat_C[3][3]

for i in range(3):
    for j in range(3):
        mat_C[i][j] = mat_A[i][j] + mat_B[i][j]

# All elements should be 10
int result1 = 1
for i in range(3):
    for j in range(3):
        if mat_C[i][j] != 10:
            result1 = 0

if result1 == 1:
    print("✓ Matrix Addition PASSED")

if result1 == 0:
    print("✗ Matrix Addition FAILED")

print()

# ======================================================
# TEST 2: Matrix Transpose
# ======================================================
print("▶ TEST 2: Matrix Transpose")

int mat_M[2][3] = [[1, 2, 3], [4, 5, 6]]
int mat_T[3][2]

for i in range(2):
    for j in range(3):
        mat_T[j][i] = mat_M[i][j]

# Verify transpose
int result2 = 1
if mat_T[0][0] != 1:
    result2 = 0
if mat_T[0][1] != 4:
    result2 = 0
if mat_T[1][0] != 2:
    result2 = 0
if mat_T[1][1] != 5:
    result2 = 0
if mat_T[2][0] != 3:
    result2 = 0
if mat_T[2][1] != 6:
    result2 = 0

if result2 == 1:
    print("✓ Matrix Transpose PASSED")

if result2 == 0:
    print("✗ Matrix Transpose FAILED")

print()

# ======================================================
# TEST 3: Matrix Multiplication (2x3 * 3x2 = 2x2)
# ======================================================
print("▶ TEST 3: Matrix Multiplication")

int mat_X[2][3] = [[1, 2, 3], [4, 5, 6]]
int mat_Y[3][2] = [[7, 8], [9, 10], [11, 12]]
int mat_Z[2][2]

# mat_Z[i][j] = sum of mat_X[i][k] * mat_Y[k][j]
for i in range(2):
    for j in range(2):
        mat_Z[i][j] = 0
        for k in range(3):
            mat_Z[i][j] = mat_Z[i][j] + mat_X[i][k] * mat_Y[k][j]

# Expected: mat_Z = [[58, 64], [139, 154]]
int result3 = 1
if mat_Z[0][0] != 58:
    result3 = 0
if mat_Z[0][1] != 64:
    result3 = 0
if mat_Z[1][0] != 139:
    result3 = 0
if mat_Z[1][1] != 154:
    result3 = 0

if result3 == 1:
    print("✓ Matrix Multiplication PASSED")

if result3 == 0:
    print("✗ Matrix Multiplication FAILED")

print()

# ======================================================
# TEST 4: Identity Matrix Check
# ======================================================
print("▶ TEST 4: Identity Matrix Creation")

int identity[4][4]

# Create identity matrix
for i in range(4):
    for j in range(4):
        if i == j:
            identity[i][j] = 1
        if i != j:
            identity[i][j] = 0

# Verify diagonal
int result4 = 1
if identity[0][0] != 1:
    result4 = 0
if identity[1][1] != 1:
    result4 = 0
if identity[2][2] != 1:
    result4 = 0
if identity[3][3] != 1:
    result4 = 0

# Verify off-diagonal
if identity[0][1] != 0:
    result4 = 0
if identity[1][0] != 0:
    result4 = 0

if result4 == 1:
    print("✓ Identity Matrix PASSED")

if result4 == 0:
    print("✗ Identity Matrix FAILED")

print()

# ======================================================
# TEST 5: Diagonal Sum
# ======================================================
print("▶ TEST 5: Diagonal Sum")

int diagmat[4][4] = [[1, 0, 0, 0], [0, 2, 0, 0], [0, 0, 3, 0], [0, 0, 0, 4]]

int dsum = 0
for i in range(4):
    dsum = dsum + diagmat[i][i]

int result5 = 0
if dsum == 10:
    result5 = 1

if result5 == 1:
    print("✓ Diagonal Sum PASSED (sum =", dsum, ")")

if result5 == 0:
    print("✗ Diagonal Sum FAILED")

print()

# ======================================================
# TEST 6: Row Sum and Column Sum
# ======================================================
print("▶ TEST 6: Row & Column Sums")

int rcmat[3][4] = [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]

# Row sums
int rsum0 = 0
int rsum1 = 0
int rsum2 = 0

for j in range(4):
    rsum0 = rsum0 + rcmat[0][j]
    rsum1 = rsum1 + rcmat[1][j]
    rsum2 = rsum2 + rcmat[2][j]

# Column sums
int csum0 = 0
int csum1 = 0
int csum2 = 0
int csum3 = 0

for i in range(3):
    csum0 = csum0 + rcmat[i][0]
    csum1 = csum1 + rcmat[i][1]
    csum2 = csum2 + rcmat[i][2]
    csum3 = csum3 + rcmat[i][3]

int result6 = 1
if rsum0 != 10:
    result6 = 0
if rsum1 != 26:
    result6 = 0
if rsum2 != 42:
    result6 = 0
if csum0 != 15:
    result6 = 0
if csum1 != 18:
    result6 = 0
if csum2 != 21:
    result6 = 0
if csum3 != 24:
    result6 = 0

if result6 == 1:
    print("✓ Row & Column Sums PASSED")

if result6 == 0:
    print("✗ Row & Column Sums FAILED")

print()

# ======================================================
# TEST 7: Find Max Element
# ======================================================
print("▶ TEST 7: Find Maximum Element")

int maxarr[3][3] = [[5, 12, 3], [8, 99, 7], [1, 4, 15]]

maxval = maxarr[0][0]
maxrow = 0
maxcol = 0

for i in range(3):
    for j in range(3):
        if maxarr[i][j] > maxval:
            maxval = maxarr[i][j]
            maxrow = i
            maxcol = j

int result7 = 0
if maxval == 99:
    if maxrow == 1:
        if maxcol == 1:
            result7 = 1

if result7 == 1:
    print("✓ Find Max PASSED (max =", maxval, ")")

if result7 == 0:
    print("✗ Find Max FAILED")

print()

# ======================================================
# TEST 8: Find Min Element
# ======================================================
print("▶ TEST 8: Find Minimum Element")

int minarr[3][3] = [[5, 12, 3], [8, 1, 7], [2, 4, 15]]

minval = minarr[0][0]
minrow = 0
mincol = 0

for i in range(3):
    for j in range(3):
        if minarr[i][j] < minval:
            minval = minarr[i][j]
            minrow = i
            mincol = j

int result8 = 0
if minval == 1:
    if minrow == 1:
        if mincol == 1:
            result8 = 1

if result8 == 1:
    print("✓ Find Min PASSED (min =", minval, ")")

if result8 == 0:
    print("✗ Find Min FAILED")

print()

# ======================================================
# TEST 9: Matrix Scalar Multiplication
# ======================================================
print("▶ TEST 9: Scalar Multiplication")

int scal[2][2] = [[1, 2], [3, 4]]
int mult = 5

for i in range(2):
    for j in range(2):
        scal[i][j] = scal[i][j] * mult

int result9 = 1
if scal[0][0] != 5:
    result9 = 0
if scal[0][1] != 10:
    result9 = 0
if scal[1][0] != 15:
    result9 = 0
if scal[1][1] != 20:
    result9 = 0

if result9 == 1:
    print("✓ Scalar Multiplication PASSED")

if result9 == 0:
    print("✗ Scalar Multiplication FAILED")

print()

# ======================================================
# TEST 10: Count Elements Meeting Condition
# ======================================================
print("▶ TEST 10: Count Elements > Threshold")

int cntarr[4][4] = [[1, 5, 2, 8], [3, 9, 4, 7], [6, 1, 10, 2], [8, 3, 5, 11]]
int thresh = 5
int cnt = 0

for i in range(4):
    for j in range(4):
        if cntarr[i][j] > thresh:
            cnt = cnt + 1

int result10 = 0
if cnt == 7:
    result10 = 1

if result10 == 1:
    print("✓ Count Elements PASSED (count =", cnt, ")")

if result10 == 0:
    print("✗ Count Elements FAILED")

print()

# ======================================================
# TEST 11: Pascal's Triangle (first 5 rows)
# ======================================================
print("▶ TEST 11: Pascal's Triangle")

int pascal[5][5]

# Initialize first column and diagonal
for i in range(5):
    pascal[i][0] = 1
    pascal[i][i] = 1

# Fill rest - need to do carefully
pascal[2][1] = pascal[1][0] + pascal[1][1]
pascal[3][1] = pascal[2][0] + pascal[2][1]
pascal[3][2] = pascal[2][1] + pascal[2][2]
pascal[4][1] = pascal[3][0] + pascal[3][1]
pascal[4][2] = pascal[3][1] + pascal[3][2]
pascal[4][3] = pascal[3][2] + pascal[3][3]

# Verify known values: row 4 should be 1, 4, 6, 4, 1
int result11 = 1
if pascal[4][0] != 1:
    result11 = 0
if pascal[4][1] != 4:
    result11 = 0
if pascal[4][2] != 6:
    result11 = 0
if pascal[4][3] != 4:
    result11 = 0
if pascal[4][4] != 1:
    result11 = 0

if result11 == 1:
    print("✓ Pascal's Triangle PASSED")
    print("  Row 4:", pascal[4][0], pascal[4][1], pascal[4][2], pascal[4][3], pascal[4][4])

if result11 == 0:
    print("✗ Pascal's Triangle FAILED")

print()

# ======================================================
# TEST 12: Checkerboard Pattern
# ======================================================
print("▶ TEST 12: Checkerboard Pattern")

int checker[4][4]

# Manually set checkerboard
checker[0][0] = 0
checker[0][1] = 1
checker[0][2] = 0
checker[0][3] = 1
checker[1][0] = 1
checker[1][1] = 0
checker[1][2] = 1
checker[1][3] = 0
checker[2][0] = 0
checker[2][1] = 1
checker[2][2] = 0
checker[2][3] = 1
checker[3][0] = 1
checker[3][1] = 0
checker[3][2] = 1
checker[3][3] = 0

# Verify pattern
int result12 = 1
if checker[0][0] != 0:
    result12 = 0
if checker[0][1] != 1:
    result12 = 0
if checker[1][0] != 1:
    result12 = 0
if checker[1][1] != 0:
    result12 = 0

if result12 == 1:
    print("✓ Checkerboard Pattern PASSED")

if result12 == 0:
    print("✗ Checkerboard Pattern FAILED")

print()

# ======================================================
# TEST 13: Border Sum (perimeter elements)
# ======================================================
print("▶ TEST 13: Border Sum")

int bordermat[4][4] = [[1, 2, 3, 4], [5, 0, 0, 8], [9, 0, 0, 12], [13, 14, 15, 16]]

int bsum = 0

# Top row
for j in range(4):
    bsum = bsum + bordermat[0][j]

# Bottom row
for j in range(4):
    bsum = bsum + bordermat[3][j]

# Left column (excluding corners already counted)
bsum = bsum + bordermat[1][0]
bsum = bsum + bordermat[2][0]

# Right column (excluding corners already counted)
bsum = bsum + bordermat[1][3]
bsum = bsum + bordermat[2][3]

# Expected: 1+2+3+4 + 13+14+15+16 + 5+9 + 8+12 = 102
int result13 = 0
if bsum == 102:
    result13 = 1

if result13 == 1:
    print("✓ Border Sum PASSED (sum =", bsum, ")")

if result13 == 0:
    print("✗ Border Sum FAILED (sum =", bsum, ")")

print()

# ======================================================
# TEST 14: Upper Triangular Matrix Check
# ======================================================
print("▶ TEST 14: Upper Triangular Matrix")

int uppermat[3][3] = [[1, 2, 3], [0, 4, 5], [0, 0, 6]]

int result14 = 1
# Check below diagonal is zero
if uppermat[1][0] != 0:
    result14 = 0
if uppermat[2][0] != 0:
    result14 = 0
if uppermat[2][1] != 0:
    result14 = 0

if result14 == 1:
    print("✓ Upper Triangular PASSED")

if result14 == 0:
    print("✗ Upper Triangular FAILED")

print()

# ======================================================
# TEST 15: Symmetric Matrix Check
# ======================================================
print("▶ TEST 15: Symmetric Matrix Check")

int symmmat[3][3] = [[1, 2, 3], [2, 4, 5], [3, 5, 6]]

int result15 = 1
# Check symmetry manually
if symmmat[0][1] != symmmat[1][0]:
    result15 = 0
if symmmat[0][2] != symmmat[2][0]:
    result15 = 0
if symmmat[1][2] != symmmat[2][1]:
    result15 = 0

if result15 == 1:
    print("✓ Symmetric Matrix PASSED")

if result15 == 0:
    print("✗ Symmetric Matrix FAILED")

print()

# ======================================================
# TEST 16: Large Matrix Operations (10x10)
# ======================================================
print("▶ TEST 16: Large Matrix (10x10) Sum")

int largemat[10][10]

# Fill with sequential values manually using formula
for i in range(10):
    for j in range(10):
        largemat[i][j] = i * 10 + j + 1

# Sum all elements (should be 1+2+...+100 = 5050)
int lsum = 0
for i in range(10):
    for j in range(10):
        lsum = lsum + largemat[i][j]

int result16 = 0
if lsum == 5050:
    result16 = 1

if result16 == 1:
    print("✓ Large Matrix Sum PASSED (sum =", lsum, ")")

if result16 == 0:
    print("✗ Large Matrix Sum FAILED (sum =", lsum, ")")

print()

# ======================================================
# TEST 17: Matrix Copy
# ======================================================
print("▶ TEST 17: Matrix Copy")

int srcmat[3][3] = [[7, 8, 9], [4, 5, 6], [1, 2, 3]]
int dstmat[3][3]

for i in range(3):
    for j in range(3):
        dstmat[i][j] = srcmat[i][j]

int result17 = 1
if dstmat[0][0] != srcmat[0][0]:
    result17 = 0
if dstmat[1][1] != srcmat[1][1]:
    result17 = 0
if dstmat[2][2] != srcmat[2][2]:
    result17 = 0

if result17 == 1:
    print("✓ Matrix Copy PASSED")

if result17 == 0:
    print("✗ Matrix Copy FAILED")

print()

# ======================================================
# TEST 18: Complex Index Expressions
# ======================================================
print("▶ TEST 18: Complex Index Expressions")

int exprmat[5][5]
for i in range(5):
    for j in range(5):
        exprmat[i][j] = i * 10 + j

# Test complex index expressions
int idx = 2
int result18 = 1

if exprmat[idx][idx] != 22:
    result18 = 0

if exprmat[idx + 1][idx - 1] != 31:
    result18 = 0

if result18 == 1:
    print("✓ Complex Index Expressions PASSED")

if result18 == 0:
    print("✗ Complex Index Expressions FAILED")

print()

# ======================================================
# FINAL SUMMARY
# ======================================================
print("==============================================")
print("  STRESS TEST SUMMARY")
print("==============================================")

int total = result1 + result2 + result3 + result4 + result5 + result6 + result7 + result8 + result9 + result10 + result11 + result12 + result13 + result14 + result15 + result16 + result17 + result18

print("Tests Passed:", total, "/ 18")

if total == 18:
    print()
    print("ALL TESTS PASSED! Vyom v0.9 is STABLE!")

if total != 18:
    print()
    print("Some tests failed. Review needed.")

print("==============================================")
