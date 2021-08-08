/*$@cat.c*/
#include "fs.h"
#include "unistd.h"

int start(int argc, char* argv[])
{
  int i;

  static char buf[234];

  FUN(write)(stdout, "hello, world, this is a test", 24);

  buf[23] = 'E';

  for (i = 1; i < argc; i++) {
    int fd = FUN(open)(argv[i], O_RDONLY);
    if (fd != -1) {
      size_t len;
      char buf[256];
      while((len = FUN(read)(fd, buf, sizeof(buf))) > 0) {
        FUN(write)(stdout, buf, len);
      }
      FUN(close)(fd);
    }
  }
  return 0;
}
