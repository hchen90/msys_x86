#include "sys.h"
#include "x86.h"
#include "proc.h"
#include "list.h"
#include "video.h"

List proc_queue[3];
extern List proc_queue[3];

extern struct proc* current_proc;
extern void switch_to(struct context** old, struct context** new);
/*进程调度*/

void sched(void)
{
  size_t i, j;
  ListNode* ln;
  struct proc* p;

  if (current_proc == NULL) return;

  for (j = 0; j < 3; j++) {
    for (i = 0; i < proc_queue[j].length; i++) {
      if ((ln = list_get(&proc_queue[j], i)) != NULL) {
        if (ln->tags.sub & SUB_PTR) {
          if ((p = (struct proc*) ln->data.ptr) != NULL && p->state == RUNNABLE) {
            switch_to(&current_proc->ctx, &p->ctx);
            return;
          }
        }
      }
    }
  }
}
