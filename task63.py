import socket
import binascii

def hex_to_bytes(hex_str):
    return binascii.unhexlify(hex_str)

def bytes_to_hex(byte_str):
    return binascii.hexlify(byte_str).decode('utf-8')

def interact_with_oracle(host, port, my_plaintext):
    # Create a TCP connection to the oracle
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        
        # Read initial response from the oracle
        response = s.recv(4096).decode('utf-8')
        print(response)  # To see Bob's ciphertext and the IVs
        
        # Send our crafted plaintext
        s.sendall(f"{my_plaintext}\n".encode('utf-8'))
        
        # Receive the ciphertext response
        response = s.recv(4096).decode('utf-8')
        print(response)
        
        return response

def main():
    # Oracle connection details
    host = '10.9.0.80'
    port = 3000
    
    # Initial crafted plaintext
    my_plaintext = "11223344aabbccdd"  # This is just an example, adjust based on the attack strategy
    
    # Interact with the oracle
    response = interact_with_oracle(host, port, my_plaintext)

    # Parse Bob's ciphertext and IVs from the response
    # Assuming the response format is known, extract the relevant values
    # Modify parsing logic as per actual response format
    lines = response.splitlines()
    bob_ciphertext = lines[0].split(": ")[1]
    bob_iv = lines[1].split(": ")[1]
    next_iv = lines[2].split(": ")[1]

    # Print out the captured values for debugging
    print(f"Bob's ciphertext: {bob_ciphertext}")
    print(f"Bob's IV: {bob_iv}")
    print(f"Next IV: {next_iv}")

    # Continue this loop with crafted plaintexts as necessary to deduce Bob's secret message
    # Implement a loop to try different plaintexts if needed

if __name__ == '__main__':
    main()
