#include "types.h"
#include "sys.h"
#include "x86.h"
#include "video.h"

void int0x20(void)
{
  static int ticks;

  ticks = (ticks + 1) % 18;

  if (! ticks) {
    //puts("int0x20\n", 0x02);
    sched(); // 过完时间片调度一次
  }

  outb(EOI, EOI);
}

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr32(void);
extern void isr33(void);
extern void isr34(void);
extern void isr35(void);
extern void isr36(void);
extern void isr37(void);
extern void isr38(void);
extern void isr39(void);
extern void isr40(void);
extern void isr41(void);
extern void isr42(void);
extern void isr43(void);
extern void isr44(void);
extern void isr45(void);
extern void isr46(void);
extern void isr47(void);

struct {
  void (* isr)(void);
} isrt[48] = {
  isr0,
  isr1,
  isr2,
  isr3,
  isr4,
  isr5,
  isr6,
  isr7,
  isr8,
  isr9,
  isr10,
  isr11,
  isr12,
  isr13,
  isr14,
  isr15,
  isr16,
  isr17,
  isr18,
  isr19,
  isr20,
  isr21,
  isr22,
  isr23,
  isr24,
  isr25,
  isr26,
  isr27,
  isr28,
  isr29,
  isr30,
  isr31, // 32
  isr32,
  isr33,
  isr34,
  isr35,
  isr36,
  isr37,
  isr38,
  isr39,
  isr40,
  isr41,
  isr42,
  isr43,
  isr44,
  isr45,
  isr46,
  isr47
};

void isr_handler(uint32_t ino)
{
  switch (ino) {
    case 0:
      puts("devide error\n", 0x07);
      break;
    case 1:
      puts("debug exception\n", 0x07);
      break;
    case 2:
      puts("nmi exception\n", 0x07);
      break;
    case 3:
      puts("breakpoint exception\n", 0x07);
      break;
    case 4:
      puts("overflow exception\n", 0x07);
      break;
    case 5:
      puts("bounds exception\n", 0x07);
      break;
    case 6:
      puts("invalid opcode exception\n", 0x07);
      break;
    case 7:
      puts("no coprocessor exception\n", 0x07);
      break;
    case 8:
      puts("double fault exception\n", 0x07);
      break;
    case 9:
      puts("segment overrun exception\n", 0x07);
      break;
    case 10:
      puts("tss exception\n", 0x07);
      break;
    case 11:
      puts("segment not present exception\n", 0x07);
      break;
    case 12:
      puts("stack fault exception\n", 0x07);
      break;
    case 13:
      puts("general protection exception\n", 0x07);
      break;
    case 14:
      puts("page fault exception\n", 0x07);
      break;
    default:
      if (ino > 31) {
        if (ino == 32) {
          int0x20();
        }
      } else {
        puts("reserved exception\n", 0x07);
      }
  }
}

void setup_interupts(void)
{
  int i;
  struct ingdesc id;

  id.seg_selector = 8 * 1;
  id.always = 0xe00;
  id.DPL = 0;
  id.present = 1;

  for (i = 0; i < 48; i++) {
    id.offset_low = (uint16_t) ((uint32_t) isrt[i].isr & 0xffff);
    id.offset_high = (uint16_t) ((uint32_t) isrt[i].isr >> 16 & 0xffff);

    new_interupt(&id, i);
  }
}


uint32_t new_segment(struct gdtdesc* gd)
{
  uint32_t rev = 0;

  if (gd != NULL) {
    uint16_t pd[3];
    uint8_t* p;
    uint32_t off, low, high;

    sgdt(pd);

    if (pd[0] >= 12 * 8 - 1) {
      logsc(2, "failed to create segment");
      return 0;
    }

    off = *(uint32_t*) (pd + 1);

    low = gd->limit_low | (gd->base_low << 16 & 0xffff0000);
    high = (gd->base_low >> 16 & 0xff) | (gd->accessed << 8) | (gd->read_write << 9) | (gd->conforming_expand_down << 10) | (gd->code << 11) | (gd->always << 12) | (gd->DPL << 13) | (gd->present << 15) | (gd->limit_high << 16) | (gd->available << 20) | (gd->always_0 << 21) | (gd->big << 22) | (gd->gran << 23) | (gd->base_high << 24);
      
    *(uint32_t*) (off + pd[0] + 1) = low;
    *(uint32_t*) (off + pd[0] + 1 + 4) = high;
    
    rev = (pd[0] + 1) / 8;

    pd[0] += 8;

    lgdt(pd);
  }
  
  return rev;
}

void new_interupt(struct ingdesc* id, uint8_t inum)
{
  if (id != NULL) {
    uint16_t pd[3];
    uint32_t off, low, high;

    sidt(pd);
    
    off = (pd[1] & 0xffff) | (pd[2] << 16 & 0xffff0000);

    low = id->offset_low | (id->seg_selector << 16 & 0xffff0000);
    high = id->always | (id->DPL << 13) | (id->present << 15) | (id->offset_high << 16);

    *(uint32_t*) (off + inum * 8) = low;
    *(uint32_t*) (off + inum * 8 + 4) = high;

    lidt(pd);
  }
}

//end
