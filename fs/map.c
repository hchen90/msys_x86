#include "types.h"
#include "fs.h"

static char* mapptr = 0;
static uint32_t maplen = 0;


int bread(int nblk)
{
}

void brelse(int nblk)
{
}



int bmap_get(int nblk)
{
}

void bmap_setstate(int nblk, int st)
{
}

void bmap_set(int nblk)
{
  bmap_setstate(nblk, 1);
}

void bmap_clear(int nblk)
{
  bmap_setstate(nblk, 0);
}



int imap_get(int ino)
{
}

void imap_setstate(int ino, int st)
{
}

void imap_set(int ino)
{
  imap_setstate(ino, 1);
}

void imap_clear(int ino)
{
  imap_setstate(ino, 0);
}
