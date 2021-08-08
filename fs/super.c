#include "fs.h"
#include "sys.h"
#include "unistd.h"

struct minix_super_block msb = {0};
extern struct minix_super_block msb;

void super_init(void)
{
  if (! msb.s_state) {
    char buf[512];

    if (hread(2, buf, 1)) {
      FUN(memcpy)(&msb, buf, sizeof(msb));
      msb.s_state = 1;
    }
  }
}

