def xor_bytes(a, b):
    return bytes([x ^ y for x, y in zip(a, b)])

def hex_to_bytes(hex_str):
    return bytes.fromhex(hex_str)

def bytes_to_hex(byte_str):
    return byte_str.hex()

def oracle_encrypt(plaintext_hex):
    return input(f"Simulated ciphertext for plaintext {plaintext_hex}: ")

def crack_ciphertext(bob_ciphertext, bob_iv, next_iv):
    bob_ciphertext_bytes = hex_to_bytes(bob_ciphertext)
    bob_iv_bytes = hex_to_bytes(bob_iv)
    next_iv_bytes = hex_to_bytes(next_iv)

    plaintext_yes = "Yes".encode().ljust(16, b'\x00')
    plaintext_no = "No".encode().ljust(16, b'\x00')

    crafted_plaintext_yes = xor_bytes(plaintext_yes, xor_bytes(bob_iv_bytes, next_iv_bytes))
    crafted_plaintext_no = xor_bytes(plaintext_no, xor_bytes(bob_iv_bytes, next_iv_bytes))

    crafted_plaintext_yes_hex = bytes_to_hex(crafted_plaintext_yes)
    crafted_plaintext_no_hex = bytes_to_hex(crafted_plaintext_no)

    ciphertext_for_yes = oracle_encrypt(crafted_plaintext_yes_hex)
    ciphertext_for_no = oracle_encrypt(crafted_plaintext_no_hex)

    if bob_ciphertext == ciphertext_for_yes:
        print("Bob's secret message is: Yes")
    elif bob_ciphertext == ciphertext_for_no:
        print("Bob's secret message is: No")
    else:
        print("Unable to determine Bob's message.")

bob_ciphertext = "54601f27c6605da997865f62765117ce"
bob_iv = "d27d724f59a84d9b61c0f2883efa7bbc"
next_iv = "d34c739f59a84d9b61c0f2883efa7bbc"

crack_ciphertext(bob_ciphertext, bob_iv, next_iv)
