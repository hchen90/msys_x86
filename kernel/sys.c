/*$@ sys.c*/
#include "types.h"
#include "video.h"
#include "x86.h"
#include "sys.h"
#include "proc.h"
#include "unistd.h"


extern void jmp_user(uint32_t eax, uint32_t eip, uint32_t cs, uint32_t eflags, uint32_t esp, uint32_t ss) __attribute__((noreturn));
extern void switch_to(uint32_t eip, uint32_t ebp, uint32_t esp) __attribute__((noreturn));
extern uint32_t get_eip(void);


void fake(void)
{
  char buf[256], i = 0;
  
  {
    puts(NULL, 7);
    puts("# cat README.txt\nthis file is for test. if you opened it, it means filesystem is working as expect\n# ", 7);
    puts("pwd\n/\n# ls /proc\nmeminfo\ncpuinfo\nmisc\n1\n2\n3\n# ", 7);
    while(1){}
  }
  puts("welcome to miniOS\n", 0x07);

  while (1) {
    int ch = getchar();
    if (ch) {
    if (ch == '\n') {
      if (i > 0) {
        buf[i] = 0;
      }
      i = 0;
    } else
    if (i < 256) {
      if (! i) {
        puts("# ", 7);
      }
      buf[i] = ch;
      putchar(ch, 7);
      i++;
    }

    if (ch == '\n') {
    if (! FUN(strcmp)(buf, "ls")) {
      puts("\nbin\ndev\nproc\ntmp\n", 0x07);
    } else if (! FUN(strcmp)(buf, "ps")) {
      puts("\ninit - 0\nsh - 1\nps - 2\n", 0x07);
    }
    }

    }
  }
}

void kmain(void)
{
  fake();

  puts(NULL, 0); /*清空屏幕*/
  puts("hello, world", 0x05);
  puts("@end\n", 0x07);

  mem_setup();
  //FUN(memcpy)((char*) 0x180000, test_txt, 64);
  //map_page((void*) 0x180000, (void*) 0x300000, 0x03);
  //puts(test_txt, 0x06);
  //puts((char*) 0x300000, 0x07);

/*
  { char buf[256];
    void* a = new_memory(NULL, 128);
    void* b = new_memory(NULL, 512);
    void* c;
    uint32_t s = 0;
    
    KNR(snprintf)(buf, sizeof(buf), "a:%x - b:%x\n", a, b);
    puts(buf, 0x06);

    if (hread(0x2, b, 1)) {
      KNR(snprintf)(buf, sizeof(buf), "disk_data: %x %x\n", *(uint32_t*) b, *(uint32_t*) (b + 4));
      puts(buf, 0x02);
    }

    free_memory(NULL, b);
    free_memory(NULL, a);

    c = new_memory(NULL, 24);

    KNR(snprintf)(buf, sizeof(buf), "c:%x\n", c);
    puts(buf, 0x06);

    FUN(memcpy)(c, "/usr/share/bin/ls", 24);
    puts(c, 0x03);

    s += next_filename(c) + 1;
    KNR(snprintf)(buf, sizeof(buf), "%s", c + s - 1);
    puts(buf, 0x02);

    s += next_filename(c + s) + 1;
    KNR(snprintf)(buf, sizeof(buf), "%s", c + s - 1);
    puts(buf, 0x02);
  
    s += next_filename(c + s) + 1;
    KNR(snprintf)(buf, sizeof(buf), "%s", c + s - 1);
    puts(buf, 0x02);

    KNR(snprintf)(buf, sizeof(buf), "sizeof(gdtdesc):%d", sizeof(struct gdtdesc));
    puts(buf, 0x03);
  } */
  
  { int i;
    struct ingdesc id;

    id.seg_selector = 8 * 1;
    id.always = 0xe00;
    id.DPL = 0;
    id.present = 1;
    
    setup_interupts();
  }

  {
    struct gdtdesc gd;

    gd.limit_low = 0xffff;
    gd.base_low = 0;
    gd.accessed = 1;
    gd.read_write = 1;
    gd.conforming_expand_down = 0;
    gd.code = 1;
    gd.always = 1;
    gd.DPL = 3;
    gd.present = 1;
    gd.limit_high = 0xf;
    gd.available = 1;
    gd.always_0 = 0;
    gd.big = 1;
    gd.gran = 1;
    gd.base_high = 0;

    new_segment(&gd);

    gd.code = 0;

    new_segment(&gd);
  }

  {
    init8259();
    disable_all_irq();
    enable_irq(0);
    init_timer(100);
    sti();
  }

  { //char buf[64];

    while (1) {
      int ch = getchar();
      
      if (ch) {
        //KNR(snprintf)(buf, sizeof(buf), "%c", ch);
        putchar(ch, 0x07);
      }
    }
  }

}

void switch_task(struct context* oldctx, struct context* newctx)
{
}

void logsc(int lev, char* text)
{
  int cr = 0x07;
  char buf[256],* p;
  size_t l = FUN(strlen)(text) + 16;

  if (l > 256) l = 256;

  if (lev == 1) { /*warning text*/
    cr = 0x06;
    FUN(memcpy)(buf, " *** warning - ", 15);
    p = buf + 15;  
  } else if (lev == 2) { /*error text*/
    cr = 0x04;
    FUN(memcpy)(buf, " *** error - ", 13);
    p = buf + 13;
  } else { /*generic text*/
    cr = 0x07;
    FUN(memcpy)(buf, " *** info - ", 12);
    p = buf + 12;
  }

  FUN(memcpy)(p, text, l - 16);
  puts(buf, cr);
}
/*end*/
