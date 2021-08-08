/*$@ video.h*/
#ifndef _VIDEO_H_
#define _VIDEO_H_
int getcursor(int* y, int* x);
int setcursor(int y, int x);
void putchar(char ch, int color);
void puts(const char* text, int color);
int getchar(void);
int gets(char* buf, int len);
#endif
/*end*/
