#ifndef _X86_H_
#define _X86_H_

#include "types.h"
#include "proc.h"

static inline uint8_t
inb(uint16_t port)
{
  uint8_t data;
  asm volatile("inb %1, %0" : "=a" (data) : "d" (port));
  return data;
}

static inline void
outb(uint16_t port, uint8_t data)
{
  asm volatile("outb %0, %1" : : "a" (data), "d" (port));
}

static inline uint16_t
inw(uint16_t port)
{
  uint16_t data;
  asm volatile("inw %1, %0" : "=a" (data) : "d" (port));
  return data;
}

static inline void
outw(uint16_t port, uint16_t data)
{
  asm volatile("outw %0, %1" : : "a" (data), "d" (port));
}

struct gdtdesc {
  unsigned int limit_low : 16;
  unsigned int base_low : 24;
  unsigned int accessed : 1;
  unsigned int read_write : 1;
  unsigned int conforming_expand_down : 1;
  unsigned int code : 1;
  unsigned int always : 1;
  unsigned int DPL : 2;
  unsigned int present : 1;
  unsigned int limit_high : 4;
  unsigned int available : 1;
  unsigned int always_0 : 1;
  unsigned int big : 1;
  unsigned int gran : 1;
  unsigned int base_high : 8;
} __attribute__((packed));

static inline void
lgdt(uint16_t pd[3])
{
  asm volatile("lgdt (%0)" : : "r" (pd));
}

static inline void
sgdt(uint16_t pd[3])
{
  asm volatile("sgdt (%0)" : "=a" (pd));
}

struct ingdesc {
  unsigned int offset_low : 16;
  unsigned int seg_selector : 16;
  unsigned int always : 13;
  unsigned int DPL : 2;
  unsigned int present : 1;
  unsigned int offset_high : 16;
} __attribute__((packed));

static inline void
lidt(uint16_t pd[3])
{
  asm volatile("lidt (%0)" : : "r" (pd));
}

static inline void
sidt(uint16_t pd[3])
{
  asm volatile("sidt (%0)" : "=a" (pd));
}

/*
struct taddesc {
  unsigned int limit_low : 16;
  unsigned int base_low : 24;
  unsigned int always : 5;
  unsigned int DPL : 2;
  unsigned int present : 1;
  unsigned int limit_high : 4;
  unsigned int available : 1;
  unsigned int always_0 : 2;
  unsigned int gran : 1;
  unsigned int base_high : 8;
} __attribute__((packed));

struct tagdesc {
  unsigned int always : 16;
  unsigned int seg_selector : 16;
  unsigned int always_0 : 13;
  unsigned int DPL : 2;
  unsigned int present : 1;
  unsigned int always_1 : 16;
} __attribute__((packed));

static inline void
ltr(uint16_t tr)
{
  asm volatile("ltr %0" : : "a" (tr));
}

static inline void
str(uint16_t* tr)
{
  asm volatile("str (%0)" : "=a" (tr));
}
*/

static inline void
cli(void)
{
  asm volatile("cli");
}

static inline void
sti(void)
{
  asm volatile("sti");
}

static inline int
btcsr(int csr, void* ptr, int off)
{
  uint8_t a = off / 8;
  uint8_t b = *(uint8_t*) ((uint8_t*) ptr + a);
  uint8_t c = off % 8;
  uint8_t* d = (uint8_t*) ((uint8_t*) ptr + a);

  int rev = (b >> c) & 0x1;

  if (csr == 1) { /*not*/
    if (rev) {
      *d = b & ~(0x1 << c);
    } else {
      *d = b | (0x1 << c);
    }
  } else if (csr == 2) { /*set*/
    *d = b | (0x1 << c);
  } else if (csr == 3) { /*clear*/
    *d = b & ~(0x1 << c);
  }

  return rev;
}

static inline uint32_t
get_cr0(void)
{
  uint32_t rev;
  asm volatile("movl %%cr0, %0" : "=a" (rev));
  return rev;
}

static inline void
set_cr0(uint32_t r)
{
  asm volatile("movl %0, %%cr0" : : "a" (r));
}

static inline uint32_t
get_cr3(void)
{
  uint32_t rev;
  asm volatile("movl %%cr3, %0" : "=a" (rev));
  return rev;
}

static inline void
set_cr3(uint32_t r)
{
  asm volatile("movl %0, %%cr3" : : "a" (r));
}

#endif
