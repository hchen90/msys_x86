#include "x86.h"
#include "sys.h"

uint32_t KNR(time)(uint32_t* tm)
{
  /*
  uint32_t stamp;
  uint8_t tmp;

  outb(0x70, 0);
  tmp = inb(0x71);
  stamp = tmp & 0xffff + (tmp >> 4) * 10;
  
  outb(0x70, 2);
  tmp = inb(0x71);
  stamp += (tmp & 0xffff + (tmp >> 4) * 10) * 60;

  outb(0x70, 4);
  tmp = inb(0x71);
  stamp += (tmp & 0xffff + (tmp >> 4) * 10) * 60 * 60;

  outb(0x70, 7);
  tmp = inb(0x71);
  stamp += (tmp & 0xffff + (tmp >> 4) * 10) * 60 * 60 * 24;
  */
}

char* KNR(ctime)(uint32_t tm)
{
  
}
