#include "types.h"
#include "fs.h"

int file_read(struct inode* inop, struct file* filp, char* buf, int len)
{
  int rev = 0, ix = 0;

  if (inop != NULL && filp != NULL) {
    unsigned int ino = iget(inop->i_ino), skip = filp->f_off;

    if (ino != -1) {
      iput(ino);
    }
  }

  return rev;
}

int file_write(struct inode* inop, struct file* filp, char* buf, int len)
{
  return 0;
}

void file_seek(struct file* filp, unsigned int off)
{
}
