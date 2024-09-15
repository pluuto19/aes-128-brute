def xor_hex_strings(hex_str1, hex_str2):
    return '{:x}'.format(int(hex_str1, 16) ^ int(hex_str2, 16)).zfill(len(hex_str1))

# Bob's ciphertext and IV
bob_ciphertext = "54601f27c6605da997865f62765117ce"
bob_iv = "d27d724f59a84d9b61c0f2883efa7bbc"

# Get the next IV from user input
next_iv = input("Enter the next IV: ").strip()

# Prepare payloads for "Yes" and "No"
yes_payload = xor_hex_strings("596573", next_iv[:6])  # "Yes" in hex
no_payload = xor_hex_strings("4e6f", next_iv[:4])     # "No" in hex

print(f"To check if the message is 'Yes', send this plaintext: {yes_payload}")
print(f"To check if the message is 'No', send this plaintext: {no_payload}")

print("\nAfter sending each plaintext, compare the first 16 bytes (32 hex characters) of the resulting ciphertext with Bob's ciphertext:")
print(f"Bob's ciphertext (first 16 bytes): {bob_ciphertext[:32]}")
print("If either matches, you've determined Bob's message.")