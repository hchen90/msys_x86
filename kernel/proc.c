#include "proc.h"
#include "list.h"
#include "sys.h"
#include "unistd.h"

extern List proc_queue[3];

/*进程信息管理，映射到`/proc'目录下*/

struct proc* current_proc = NULL;
extern struct proc* current_proc;

int global_inc_pid = 0;

struct proc* proc_new(void)
{
  struct proc* p = (struct proc*) new_memory(NULL, sizeof(struct proc));

  if (p != NULL) {
    FUN(memset)(p, 0, sizeof(struct proc));
    p->pid = global_inc_pid; global_inc_pid++;
    p->state = RUNNABLE;
    p->mf.free_base = (void*) 0x400000;
    p->mf.free_top = (void*) 0x600000;
    p->mf.virt_base = (void*) 0x800000;
    p->mf.virt_top = (void*) 0xa00000;
  }
}

void proc_free(struct proc* procp)
{
  if (procp != NULL) {
    //
  }
}

void proc_nice(int pid, int incr)
{
}

int proc_fork(void)
{
  if (current_proc != NULL) {
    
  }
}

void proc_sleep(uint32_t slice)
{
}

void proc_kill(struct proc* procp)
{
}
