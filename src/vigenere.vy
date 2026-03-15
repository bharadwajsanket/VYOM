# ================================================
# Vigenere Cipher — Vyom
# ================================================

print("Enter message:")
string raw_msg = input()

print("Enter key:")
string raw_key = input()

# ---- Normalize: keep only A-Z, auto-uppercase ----
# Build cleaned message (letters only, uppercased)
string message[100]
int mlen = 0
for i in range(len(raw_msg)):
    int c = ascii(raw_msg[i])
    # lowercase a-z → uppercase
    if c >= 97 and c <= 122:
        c = c - 32
    # keep only A-Z
    if c >= 65 and c <= 90:
        message[mlen] = char(c)
        mlen = mlen + 1

# Build cleaned key (letters only, uppercased)
string key[100]
int klen = 0
for i in range(len(raw_key)):
    int c = ascii(raw_key[i])
    if c >= 97 and c <= 122:
        c = c - 32
    if c >= 65 and c <= 90:
        key[klen] = char(c)
        klen = klen + 1

if mlen == 0:
    print("Error: message has no letters")
    exit(1)

if klen == 0:
    print("Error: key has no letters")
    exit(1)

# ---- Encrypt ----
string enc_chars[100]
string enc_out = ""
for i in range(mlen):
    int ki = i % klen
    int m  = ascii(message[i])  - 65
    int k  = ascii(key[ki])     - 65
    enc_chars[i] = char((m + k) % 26 + 65)
    enc_out = concat(enc_out, enc_chars[i])

# ---- Decrypt ----
string dec_out = ""
for i in range(mlen):
    int ki = i % klen
    int e  = ascii(enc_chars[i]) - 65
    int k  = ascii(key[ki])      - 65
    dec_out = concat(dec_out, char((e - k + 26) % 26 + 65))

# ---- Build original for display ----
string msg_out = ""
for i in range(mlen):
    msg_out = concat(msg_out, message[i])

string key_out = ""
for i in range(klen):
    key_out = concat(key_out, key[i])

# ---- Output ----
print("Message  :", msg_out)
print("Key      :", key_out)

print("Encrypted:")
for i in range(mlen):
    print(enc_chars[i], "")

print("Encrypted string:", enc_out)
print("Decrypted string:", dec_out)

if dec_out == msg_out:
    print("Round-trip: PASS")
else:
    print("Round-trip: FAIL")

