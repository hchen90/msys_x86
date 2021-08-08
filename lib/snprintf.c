/* $ @sprintf.c
 * Copyright (C) 2016 Hsiang Chen
 * Bug Report: master@hsiangchen.top
 *
 * This program is free software; you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; ; either version 2 of the License, 
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details. 
 * */
#include "sys.h"
#include "stdarg.h"

int KNR(snprintf)(char* buf, size_t len, const char* fmt, ...)
{
  int n = 0;
  va_list ap = 0;
  char* p = (char*) fmt,* last = buf + len;
  if (fmt != 0) {
    char escape = 0, type = 0;
    va_start(ap, fmt);
    for ( ; n < len && *p != '\0'; p++) {
      if (escape == 1) {
        switch (*p) {
          case 'c': type = 0x1; break;
          case 'd': type = 0x2; break;
          case 'i': type = 0x2; break;
          case 'o': type = 0x3; break;
          case 's': type = 0x4; break;
          case 'u': type = 0x5; break;
          case 'x': type = 0x6; break;
          case 'X': type = 0x7; break;
          case 'p': type = 0x8; break;
          case 'P': type = 0x9; break;
          case '%': type = 0xa; break;
          default : type = 0x0; break;
        }
        escape = 0;
      } else if (escape == 2) {
        switch (*p) {
          case 'n': buf[n++] = '\n'; break;
          case 'r': buf[n++] = '\r'; break;
          case 't': buf[n++] = '\t'; break;
          case '\\': buf[n++] = '\\'; break;
          default: buf[n++] = *p; break;
        }
        escape = 0;
        continue;
      }
      if (*p == '%' && !type) { /*ignore %%*/
        escape = 1; continue;
      } else if (*p == '\\') {
        escape = 2; continue;
      }
      if (type) {
        if (type == 1) { /*character*/
          buf[n++] = va_arg(ap, int);
        } else if (type == 3) { /*octal*/
          unsigned int val = va_arg(ap, unsigned int);
          char swap = 0,* p_s = &buf[n],* p_e = &buf[n];
          if (val == 0) {
            buf[n++] = '0';
            buf[n++] = '0';
          } else {
            for ( ; val > 0; val /= 8) {
              buf[n++] = val % 8 + '0';
              p_e++;
            }
            for ( ; p_s < p_e; ) { /*swap*/
              swap = *p_s;
              *p_s = *(p_e - 1);
              *(p_e - 1) = swap;
              p_s++; p_e--;
            }
          }
        } else if (type == 4) { /*string*/
          char* str = va_arg(ap, char*);
          if (str != 0) {
            for ( ; *str != '\0'; str++) {
              if (*str >= 0x20 && *str < 0x7f) {
                buf[n++] = *str;
              } else break;
            }
          }
        } else if (type == 2 || type == 5) { /*signed/unsigned integers*/
          unsigned int val = va_arg(ap, unsigned int);
          char swap = 0,* p_s = &buf[n],* p_e = &buf[n];
          if (val == 0) {
            buf[n++] = '0';
          } else {
            if (type == 2) {
              if ((int) val < 0) {
                buf[n++] = '-';
                p_s++; p_e++;
                val = 0 - val;
              }
            }
            for ( ; val > 0; val /= 10) {
              buf[n++] = val % 10 + '0';
              p_e++;
            }
            for ( ; p_s < p_e; ) { /*swap buffer*/
              swap = *p_s;
              *p_s = *(p_e - 1);
              *(p_e - 1) = swap;
              p_s++; p_e--;
            }
          }
        } else if (type == 6 || type == 7) { /*hexadecimal, lowercase/upcase*/
          unsigned int val = va_arg(ap, unsigned int);
          char swap = 0,* p_s = &buf[n],* p_e = &buf[n];
          if (val == 0) {
            buf[n++] = '0';
          } else {
            for ( ; val > 0; val /= 16) {
              if (val % 16 >= 0 && val % 16 <= 9) {
                buf[n++] = val % 16 + '0';
              } else {
                if (type == 6) {
                  buf[n++] = val % 16 + 'a' - 10;
                } else {
                  buf[n++] = val % 16 + 'A' - 10;
                }
              }
              p_e++;
            }
            for ( ; p_s < p_e; ) { /*swap buffer*/
              swap = *p_s;
              *p_s = *(p_e - 1);
              *(p_e - 1) = swap;
              p_s++; p_e--;
            }
          }
        } else if (type == 8 || type == 9) { /*pointer*/
          unsigned long val = va_arg(ap, unsigned long);
          char swap = 0,* p_s = &buf[n + 2],* p_e = &buf[n + 2];
          buf[n++] = '0'; buf[n++] = 'x';
          for ( ; val > 0; val /= 16) {
            if (val % 16 >= 0 && val % 16 <= 9) {
              buf[n++] = val % 16 + '0';
            } else {
              if (type == 8) {
                buf[n++] = val % 16 + 'a' - 10;
              } else {
                buf[n++] = val % 16 + 'A' - 10;
              }
            }
            p_e++;
          }
          for ( ; p_s < p_e; ) { /*swap buffer*/
            swap = *p_s;
            *p_s = *(p_e - 1);
            *(p_e - 1) = swap;
            p_s++; p_e--;
          }
        } else if (type == 10) { /*%*/
          buf[n++] = '%';
        }
        type = 0;
      } else {
        buf[n++] = *p;
      }
    }
    va_end(ap);
  }
  
  if (n < len) {
    buf[n] = '\0';
  }
  
  return n;
}

/*end*/
