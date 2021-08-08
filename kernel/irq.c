#include "types.h"
#include "x86.h"
#include "sys.h"

uint16_t irq_mask = 0xffff;

void init8259(void)
{
  // ICW1
  outb(M_PIC, 0x11);
  outb(S_PIC, 0x11);

  // ICW2
  outb(M_PIC + 1, 0x20);
  outb(S_PIC + 1, 0x28);

  // ICW3
  outb(M_PIC + 1, 0x04);
  outb(S_PIC + 1, 0x02);

  // ICW4
  outb(M_PIC + 1, 0x03);
  outb(S_PIC + 1, 0x03);
}

void enable_irq(uint16_t irq_no)
{
  irq_mask &= ~ (1 << irq_no);

  if (irq_no >= 8) {
    irq_mask &= ~ (1 << 2);
  }

  outb(M_IMR, irq_mask & 0xff);
  outb(S_IMR, (irq_mask >> 8) & 0xff);
}

void disable_irq(uint16_t irq_no)
{
  irq_mask |= (1 << irq_no);

  if ((irq_mask & 0xff) == 0xff00) {
    irq_mask |= (1 << 2);
  }

  outb(M_IMR, irq_mask & 0xff);
  outb(S_IMR, (irq_mask >> 8) & 0xff);
}

void enable_all_irq(void)
{
  outb(M_IMR, 0x00);
  outb(S_IMR, 0x00);
}

void disable_all_irq(void)
{
  outb(M_IMR, 0xff);
  outb(S_IMR, 0xff);
}

//end
