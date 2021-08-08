#include "types.h"
#include "x86.h"
#include "sys.h"

#define TIMER_FREQ 1193180

void init_timer(uint32_t hz)
{
  if (hz > 0) {
    uint32_t divisor = TIMER_FREQ / hz;

    outb(P_TIMER + 3, 0x34); // 速率发生器
    outb(P_TIMER, divisor & 0xff);
    outb(P_TIMER, (divisor >> 8) & 0xff);

    enable_irq(0);
  }
}

//end
