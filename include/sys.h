/* 内核空间函数 */
#ifndef _SYS_INTERNAL_INC_
#define _SYS_INTERNAL_INC_

#include "types.h"
#include "proc.h"
#include "x86.h"

#define KNR(name) k ## name

/* chunk.c */
void* new_chunk(void);
void free_chunk(void* ptr);

/* mem.c */
void mem_setup(void);
void* get_phyaddr(void* virtaddr);
int map_page(void* phyaddr, void* virtaddr, uint32_t flags);
int umap_page(void* virtaddr);
void* new_memory(struct proc* procp, size_t len);
void free_memory(struct proc* procp, void* ptr);

/*snprintf.c*/
int KNR(snprintf)(char* buf, size_t len, const char* fmt, ...);

/*sys.c*/
void task_setup(void);
void kmain(void);
void logsc(int lev, char* text);

/*syscall.c*/
uint32_t new_segment(struct gdtdesc* sg);
void new_interupt(struct ingdesc* id, uint8_t inum);
void setup_interupts(void);

/*irq.c*/
void init8259(void);
void enable_irq(uint16_t irq_no);
void disable_irq(uint16_t irq_no);
void enable_all_irq(void);
void disable_all_irq(void);

/*timer.c*/
void init_timer(uint32_t hz);


/*sched.c*/
void sched(void);

#endif
