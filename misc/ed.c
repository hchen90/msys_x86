/* command:
 * N - goto line N
 * . - current line
 * $ - last line
 * e - edit file
 * w - write file
 * a - append text after current line
 * d - delete current line
 * s - subsititute text in current line
 * */

#include "unistd.h"

int start(int argc, char* argv[])
{
  const char s[] = "hello, world";

  FUN(write)(stdout, s, sizeof(s) - 1);
}
