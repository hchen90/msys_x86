#include "x86.h"

// count - 扇区(sector)数

int hread(int lba, void* ptr, int count)
{
  int i = 0;

  if (count > 0) {
    outb(0x1f2, count); // 扇区数目
    outb(0x1f3, lba & 0xff); // LBA地址 0~7
    outb(0x1f4, (lba >> 8) & 0xff); // LBA地址 8~15
    outb(0x1f5, (lba >> 16) & 0xff); // LBA地址 16~23
    outb(0x1f6, (lba >> 24) & 0xff | 0xe0); // LBA地址 24~27
    outb(0x1f7, 0x20); // 读取命令
    
    for ( ; inb(0x1f7) & 0x88 != 0x08; ) {
      // 等待磁盘准备好
    }
    for ( ; i < count * 512; i += 2) {
      *(unsigned short*) (ptr + i) = inw(0x1f0); // 读取两个字节
    }
  }

  return i;
}

int hwrite(int lba, void* ptr, int count)
{
  int i = 0;

  if (count > 0) {
    outb(0x1f2, count); // 扇区数目
    outb(0x1f3, lba & 0xff); // LBA地址 0~7
    outb(0x1f4, (lba >> 8) & 0xff); // LBA地址 8~15
    outb(0x1f5, (lba >> 16) & 0xff); // LBA地址 16~23
    outb(0x1f6, (lba >> 24) & 0xff | 0xe0); // LBA地址 24~27
    outb(0x1f7, 0x30); // 写命令
    
    for ( ; inb(0x1f7) & 0x88 != 0x08; ) {
      // 等待磁盘准备好
    }
    for ( ; i < count * 512; i += 2) {
      outw(0x1f0, *(unsigned short*) (ptr + i)); //写入两个字节
    }
  }

  return i;
}
