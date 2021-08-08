#ifndef __UNISTD_H_
#define __UNISTD_H_

#include "types.h"
#include "syscall.h"

#define FUN(name) _ ## name

/*以下调用syscall的函数位于用户层ring 3级别*/

static inline void*
FUN(memcpy)(void* des, void* src, size_t len) /*内存拷贝*/
{
  size_t i;
  for (i = 0; i < len; i++) *(uint8_t*)(des + i) = *(uint8_t*)(src +i);
  return des;
}

static inline void*
FUN(memset)(void* ptr, int n, size_t len) /*内存数值设置*/
{
  size_t i;
  for (i = 0; i < len; i++) *(uint8_t*)(ptr + i) = n;
  return ptr;
}

static inline size_t
FUN(strlen)(const char* ptr)
{
  size_t len = 0;
  while (ptr[len] != '\0') len++;
  return len;
}

static inline int
FUN(strcmp)(const char* src, const char* des)
{
  size_t i, r = 0, n = FUN(strlen)(src), m = FUN(strlen)(des);
  if (n != m) return n - m;
  for (i = 0; i < n; i++) if (src[i] != des[i]) r |= 1;
  return r;
}

/* syscall */

static inline int
FUN(access)(const char* filename, int mode) /*判断文件属性*/
{
  int rev;
  _syscall2(access, filename, mode, rev);
  return rev;
}

static inline int
FUN(chmod)(const char* pathname, mode_t mode) /*修改文件属性*/
{
  int rev;
  _syscall2(chmod, pathname, mode, rev);
  return rev;
}

static inline int
FUN(chown)(const char* pathname, uid_t owner, gid_t group) /*修改文件所属*/
{
  int rev;
  _syscall3(chown, pathname, owner, group, rev);
  return rev;
}

static inline int
FUN(close)(int fd) /*文件关闭*/
{
  int rev;
  _syscall1(close, fd, rev);
  return rev;
}

static inline int
FUN(exit)(int status) /*进程退出，该函数返回值永远是0*/
{
  int rev;
  _syscall1(exit, status, rev);
  return rev;
}

static inline pid_t
FUN(fork)(void) /*fork新的进程*/
{
  pid_t rev;
  _syscall0(fork, rev);
  return rev;
}

static inline int
FUN(ioctl)(int fd, unsigned int request)
{
  int rev;
  _syscall2(ioctl, fd, request, rev);
  return rev;
}

static inline int
FUN(kill)(pid_t pid, int sig) /*杀死进程*/
{
  int rev;
  _syscall2(kill, pid, sig, rev);
  return rev;
}

static inline int
FUN(mkdir)(const char* pathname, mode_t mode) /*创建文件目录*/
{
  int rev;
  _syscall2(mkdir, pathname, mode, rev);
  return rev;
}

static inline int
FUN(open)(const char* pathname, int flags) /*打开文件*/
{
  int rev;
  _syscall2(open, pathname, flags, rev);
  return rev;
}

static inline size_t
FUN(read)(int fd, void* buf, size_t count) /*读文件*/
{
  size_t rev;
  _syscall3(read, fd, buf, count, rev);
  return rev;
}

static inline int
FUN(reboot)(int cmd) /*机器重新启动或关机*/
{
  int rev;
  _syscall1(reboot, cmd, rev);
  return rev;
}

static inline int
FUN(rename)(const char* oldpath, const char* newpath) /*重命名*/
{
  int rev;
  _syscall2(rename, oldpath, newpath, rev);
  return rev;
}

static inline int
FUN(rmdir)(const char* pathname) /*删除目录*/
{
  int rev;
  _syscall1(rmdir, pathname, rev);
  return rev;
}

static inline off_t
FUN(seek)(int fd, off_t offset, int where) /*文件定位*/
{
  off_t rev;
  _syscall3(seek, fd, offset, where, rev);
  return rev;
}

struct stat {
  mode_t st_mode;
  uid_t st_uid;
  gid_t st_gid;
  off_t st_size;
  time_t st_atim;
  time_t st_mtim;
  time_t st_ctim;
};

static inline int
FUN(stat)(const char* pathname, struct stat* buf) /*文件属性信息获取*/
{
  int rev;
  _syscall2(stat, pathname, buf, rev);
  return rev;
}

static inline int
FUN(stime)(const time_t* tm) /*设置时间*/
{
  int rev;
  _syscall1(stime, tm, rev);
  return rev;
}

static inline time_t
FUN(time)(time_t* tloc) /*获取时间*/
{
  time_t rev;
  _syscall1(time, tloc, rev);
  return rev;
}

static inline char*
FUN(ctime)(time_t tm)
{
  char* tms = NULL;
  _syscall1(ctime, tm, tms);
  return tms;
}

struct timbuf {
  time_t actime;
  time_t modtime;
};

static inline int
FUN(utime)(const char* filename, const struct timbuf* times) /*修改文件时间*/
{
  int rev;
  _syscall2(utime, filename, times, rev);
  return rev;
}

static inline pid_t
FUN(wait)(int* wstatus) /*进程等待*/
{
  pid_t rev;
  _syscall1(wait, wstatus, rev);
  return rev;
}

static inline size_t
FUN(write)(int fd, const void* buf, size_t count) /*写文件*/
{
  size_t rev;
  _syscall3(write, fd, buf, count, rev);
  return rev;
}

typedef void (*sighandler_t)(int);

static inline sighandler_t
FUN(signal)(int signum, sighandler_t handler)
{
  return NULL;
}

static inline int
FUN(shmget)(key_t key, size_t size, int flags)
{
  int rev;
  _syscall3(shmget, key, size, flags, rev);
  return rev;
}

static inline void*
FUN(shmat)(int shmid, const void* vptr, int flags)
{
  void* rev;
  _syscall3(shmat, shmid, vptr, flags, rev);
  return rev;
}

static inline int
FUN(shmdt)(const void* vptr)
{
  int rev;
  _syscall1(shmdt, vptr, rev);
  return rev;
}

#endif
