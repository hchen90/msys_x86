/*$@kill.c*/

#include "unistd.h"

long from_dec(char* str, int len);

int start(int argc, char* argv[])
{
  if (argc >= 2) {
    pid_t pid = (pid_t) from_dec(argv[1], FUN(strlen)(argv[1]));
    FUN(kill)(pid, 0);
  }
  return 0;
}
