#ifndef _PROC_H_
#define _PROC_H_

#include "types.h"
#include "fs.h"

#define MAXOFILE 256

enum procstate { UNKNOWN, RUNNING, RUNNABLE, SLEEPING, ZOMBIE };
/* RUNNING  - 正在运行
 * RUNNABLE - 就绪
 * SLEEPING - 等待
 * ZOMBIE   - 以及结束
 */

struct context {
  uint16_t cs;
  uint16_t ds;
  uint32_t cr3;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebx;
  uint32_t ebp;
  uint32_t eip;
};

struct proc {
  int pid;
  enum procstate state;
  struct context* ctx;
  struct proc* parent;
  struct inode* cwd;
  struct file* of[MAXOFILE];
  struct {
    void* free_base;
    void* free_top;
    void* virt_base;
    void* virt_top;
  } mf;
};

#endif
