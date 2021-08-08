#ifndef _SHM_H_
#define _SHM_H_

#undef key_t
#define key_t unsigned int

#undef IPC_CREAT
#define IPC_CREAT 0x0001

#undef IPC_EXCL
#define IPC_EXCL 0x0002

#undef IPC_PRIVATE
#define IPC_PRIVATE 0x0004

int shmget(key_t key, size_t size, int flags);
void* shmat(int shmid, const char* shmaddr, int flags);
int shmdt(void* shmaddr);

#endif
