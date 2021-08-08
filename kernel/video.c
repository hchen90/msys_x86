/*$@ video.c*/
#include "types.h"
#include "x86.h"
#include "video.h"
#include "unistd.h"

uint32_t vid_vptr = 0xb8000;

extern uint32_t vid_vptr;

int pointtooffset(int y, int x)
{
  return y * 80 + x;
}

void offsettopoint(int off, int* y, int* x)
{
  int rev = 0;

  if (y != NULL) {
    *y = off / 80;
  }
  if (x != NULL) {
    *x = off % 80;
  }
}

int getcursor(int* y, int* x)
{
  int rev = 0;

  outb(0x3d4, 0x0e);
  rev |= (inb(0x3d5) << 8) & 0xff00;

  outb(0x3d4, 0x0f);
  rev |= inb(0x3d5) & 0xff;

  offsettopoint(rev, y, x);

  return rev;
}

int setcursor(int y, int x)
{
  int idx = pointtooffset(y, x), rev = 0;

  while (idx >= 2000) {
    FUN(memcpy)((uint8_t*) vid_vptr, (uint8_t*) (vid_vptr + 80 * 2), idx * 2);
    idx -= 80;
    rev = 1;
  }

  outb(0x3d4, 0x0e);
  outb(0x3d5, (idx >> 8) & 0xff);
  outb(0x3d4, 0x0f);
  outb(0x3d5, idx & 0xff);

  return rev;
}

void putchar(char ch, int color)
{
  int y, x, off;

  off = getcursor(&y, &x);

  if (ch == '\n') {
    setcursor(y + 1, 0);
  } else if (ch == '\t') {
    if (off + 4 < 2000) {
      putchar(' ', 0x07);
      putchar(' ', 0x07);
      putchar(' ', 0x07);
      putchar(' ', 0x07);
      setcursor(y, x + 4);
    } else {
      putchar(' ', 0x07);
      setcursor(y, x + 1);
    }
  } else {
    *(uint8_t*)(vid_vptr + off * 2) = ch;
    *(uint8_t*)(vid_vptr + off * 2 + 1) = color & 0xff;
    setcursor(y, x + 1);
  }
}

void puts(const char* text, int color)
{
  int i;

  if (text == NULL) { /*清空屏幕*/
    for (i = 0; i < 2000; i += 2) {
      *(uint8_t*)(vid_vptr + i) = 0;
      *(uint8_t*)(vid_vptr + i + 1) = 0x07;
      setcursor(0, 0);
    }
  } else { /*显示文字*/
    for (i = 0; text[i] != '\0'; i++)  {
      putchar(text[i], color);
    }
  }
}

struct {
  uint8_t ch;
  uint8_t down;
  uint8_t up;
} chars_table[] = { /*键盘扫描码*/
  {'a', 0x1e, 0x9e} ,
  {'b', 0x30, 0xb0} ,
  {'c', 0x2e, 0xae} ,
  {'d', 0x20, 0xa0} ,
  {'e', 0x12, 0x92} ,
  {'f', 0x21, 0xa1} ,
  {'g', 0x22, 0xa2} ,
  {'h', 0x23, 0xa3} ,
  {'i', 0x17, 0x97} ,
  {'j', 0x24, 0xa4} ,
  {'k', 0x25, 0xa5} ,
  {'l', 0x26, 0xa6} ,
  {'m', 0x32, 0xb2} ,
  {'n', 0x31, 0xb1} ,
  {'o', 0x18, 0x98} ,
  {'p', 0x19, 0x99} ,
  {'q', 0x10, 0x90} ,
  {'r', 0x13, 0x93} ,
  {'s', 0x1f, 0x9f} ,
  {'t', 0x14, 0x94} ,
  {'u', 0x16, 0x96} ,
  {'v', 0x2f, 0xaf} ,
  {'w', 0x11, 0x91} ,
  {'x', 0x2d, 0xad} ,
  {'y', 0x15, 0x95} ,
  {'z', 0x2c, 0xac} ,
  {'0', 0x0b, 0x8b} ,
  {'1', 0x02, 0x82} ,
  {'2', 0x03, 0x83} ,
  {'3', 0x04, 0x84} ,
  {'4', 0x05, 0x85} ,
  {'5', 0x06, 0x86} ,
  {'6', 0x07, 0x87} ,
  {'7', 0x08, 0x88} ,
  {'8', 0x09, 0x89} ,
  {'9', 0x0a, 0x8a} ,
  {'\n', 0x1c, 0x9c} ,
  {'\b', 0x0e, 0x8e} ,
  {'\t', 0x0f, 0x8f} ,
  {' ', 0x39, 0xb9} ,
  {'-', 0x0c, 0x8c} ,
  {'+', 0x0d, 0x8d} ,
  {'[', 0x1a, 0x9a} ,
  {']', 0x1b, 0x9b} ,
  {'\\', 0x2b, 0xab} ,
  {';', 0x27, 0xa7} ,
  {'\'', 0x28, 0xa8} ,
  {',', 0x33, 0xb3} ,
  {'.', 0x34, 0xb4} ,
  {'/', 0x35, 0xb5} ,
  {0, 0x00, 0x00}
};

static uint8_t cbix = 0;
static uint8_t cb[2] = {0};
static uint8_t shft_on = 0;
static uint8_t cap_on = 0;

int getchar(void)
{
  if (inb(0x64) & 0x1) { /*读取键盘状态*/
    /*读取字符*/
    uint8_t c = inb(0x60);

    if (c == 0x2a) { /*左SHIFT按下*/
      shft_on |= 0x01;
      return 0;
    }
    
    if (c == 0x36) { /*右SHIFT按下*/
      shft_on |= 0x02;
      return 0;
    }

    if (c == 0xaa || c == 0xb6) { /*SHIFT弹上*/
      shft_on = 0;
      return 0;
    }

    cb[cbix] = c;
    cbix = (cbix + 1) % 2;

    if (! cbix) {
      if (cb[0] == 0x3a && cb[1] == 0xba) { /*CAPSLOCK*/
        cap_on = ! cap_on;
        return 0;
      }
    }

    if (! cbix) {
      int i;

      for (i = 0; chars_table[i].ch; i++) {
        if (chars_table[i].down == cb[0] && chars_table[i].up == cb[1]) {
          return (shft_on || cap_on) && i >= 0 && i <= 25 ? chars_table[i].ch & 0xdf : chars_table[i].ch;
        }
      }
    }
  }

  return 0;
}

int gets(char* buf, int len)
{
  int rev = 0, c;
  
  while ((c = getchar()) != '\n' && c != 0) {
    if (rev < len) {
      buf[rev++] = c & 0xff;
    }
  }

  return rev;
}

/*end*/
