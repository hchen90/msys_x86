#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#define __SYSCALL_empty     0
#define __SYSCALL_exit      1
#define __SYSCALL_fork      2
#define __SYSCALL_read      3
#define __SYSCALL_write     4
#define __SYSCALL_open      5
#define __SYSCALL_close     6
#define __SYSCALL_time      7
#define __SYSCALL_ctime     8
#define __SYSCALL_chmod     9
#define __SYSCALL_chown     10
#define __SYSCALL_seek      11
#define __SYSCALL_kill      12
#define __SYSCALL_rename    13
#define __SYSCALL_mkdir     14
#define __SYSCALL_rmdir     15
#define __SYSCALL_ioctl     16
#define __SYSCALL_reboot    17
#define __SYSCALL_stat      18
#define __SYSCALL_stime     19
#define __SYSCALL_utime     20
#define __SYSCALL_access    21
#define __SYSCALL_wait      22
#define __SYSCALL_exec      23
#define __SYSCALL_signal    24
#define __SYSCALL_shmget    25
#define __SYSCALL_shmat     26
#define __SYSCALL_shmdt     27

#define _syscall0(name, rev)                                                  \
  asm volatile (                                                              \
    "int $0x32"                                                               \
    : "=a" (rev)                                                              \
    : "a" (__SYSCALL_##name)                                                  \
  )

#define _syscall1(name, arg0, rev)                                            \
  asm volatile (                                                              \
    "int $0x32"                                                               \
    : "=a" (rev)                                                              \
    : "a" (__SYSCALL_##name), "b" ((uint32_t)(arg0))                          \
  )

#define _syscall2(name, arg0, arg1, rev)                                      \
  asm volatile (                                                              \
    "int $0x32"                                                               \
    : "=a" (rev)                                                              \
    : "a" (__SYSCALL_##name), "b" ((uint32_t)(arg0)), "c" ((uint32_t)(arg1))  \
  )

#define _syscall3(name, arg0, arg1, arg2, rev)                                \
  asm volatile (                                                              \
    "int $0x32"                                                               \
    : "=a" (rev)                                                              \
    : "a" (__SYSCALL_##name), "b" ((uint32_t)(arg0)), "c" ((uint32_t)(arg1)), \
      "d" ((uint32_t)(arg2))                                                  \
  )

#define _syscall4(name, arg0, arg1, arg2, arg3, rev)                          \
  asm volatile (                                                              \
    "pushl %5\n\t"                                                            \
    "int $0x32"                                                               \
    : "=a" (rev)                                                              \
    : "a" (__SYSCALL_##name), "b" ((uint32_t)(arg0)), "c" ((uint32_t)(arg1)), \
      "d" ((uint32_t)(arg2)), "g" ((uint32_t)(arg3))                          \
  )
#endif
