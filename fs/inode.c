#include "types.h"
#include "fs.h"
#include "sys.h"
#include "list.h"
#include "unistd.h"

List inode_in_core;

extern struct minix_super_block msb;

/* ino - 需要寻找的inode
 * 输出：锁定的inode
 * */
unsigned int iget(unsigned int ino)
{
  size_t i;
  void* p;
  unsigned int rev = -1;

  for (i = 0; i < inode_in_core.length; i++) {
    ListNode* ln = list_get(&inode_in_core, i);

    if (ln != NULL && ln->tags.sub & SUB_PTR && ln->data.ptr != NULL) {
      struct inode* inop = (struct inode*) ln->data.ptr;

      if (inop != NULL && inop->i_ino == ino) {
        inop->i_locked = 1;
        return ino;
      }
    }
  }

  super_init();

  if (msb.s_magic == 0x123f) {
    unsigned int off = ino * sizeof(struct minix_inode);
    unsigned int n = (msb.s_ninodes * sizeof(struct minix_inode) - 1) / 512 + 1;
    
    if ((p = new_memory(NULL, n)) != NULL) {
      if (hread(2 + 2 + msb.s_imap_blocks * 2 + msb.s_zmap_blocks * 2, p, n)) {
        struct minix_inode* mi = (struct minix_inode*) (p + off);
        struct inode* inop;

        if (imap_get(ino)) {
          ListNode ln;

          FUN(memset)(&ln, 0, sizeof(ln));
          ln.tags.sub = SUB_DATA | SUB_PTR | SUB_LEN | SUB_FREE;
          ln.data.len = sizeof(struct inode);
          ln.data.ptr = new_memory(NULL, sizeof(struct inode));

          inop = (struct inode*) ln.data.ptr;

          inop->i_ino = ino;
          inop->i_mode = mi->i_mode;
          inop->i_nlinks = mi->i_nlinks;
          inop->i_uid = mi->i_uid;
          inop->i_gid = mi->i_gid;
          inop->i_size = mi->i_size;
          inop->i_atime = inop->i_ctime = inop->i_mtime = mi->i_time;
          inop->i_locked = 1;

          for (i = 0; i < 10; i++) {
            inop->i_zone[i] = mi->i_zone[i];
          }

          list_insert(&inode_in_core, -1, &ln);

          rev = ino;
        }
      }
      free_memory(NULL, p);
    }
  }

  return -1;
}

/*ino - 需要释放的in-core inode*/
void iput(unsigned int ino)
{
  size_t i, j;
  uint32_t m, n;
  struct inode* inop = NULL;

  for (i = 0; i < inode_in_core.length; i++) {
    ListNode* ln = list_get(&inode_in_core, i);

    if (ln != NULL && ln->tags.sub & SUB_PTR && ln->data.ptr != NULL) {
      inop = (struct inode*) ln->data.ptr;

      if (inop != NULL && inop->i_ino == ino) {
        inop->i_locked = 0;
      }
    }
  }

  n = ino * sizeof(struct minix_inode) / 512;
  
  if (msb.s_state && ino < msb.s_ninodes && inop != NULL) {
    char buf[512];
    struct minix_inode mi;

    FUN(memset)(&mi, 0, sizeof(mi));

    mi.i_mode = inop->i_mode;
    mi.i_uid = inop->i_uid;
    mi.i_size = inop->i_size;
    mi.i_time = inop->i_mtime;
    mi.i_gid = inop->i_gid;
    mi.i_nlinks = inop->i_nlinks;
    
    for (j = 0; j < 10; j++) {
      mi.i_zone[j] = inop->i_zone[j];
    }

    hread(2 + 2 + msb.s_imap_blocks * 2 + msb.s_zmap_blocks * 2 + n, buf, 1);
    m = ino - n * (512 / sizeof(struct minix_inode));

    FUN(memcpy)(buf + m * sizeof(struct minix_inode), &mi, sizeof(mi));

    // 更新到磁盘
    hwrite(2 + 2 + msb.s_imap_blocks * 2 + msb.s_zmap_blocks * 2 + n, buf, 1);
  }
}

/* 申请新的inode
 * 输出：锁定的inode
 * */
unsigned int ialloc(void)
{
  return 0;
}

/*ino - 需要释放的文件系统（磁盘上面）的inode*/
void ifree(unsigned int ino)
{

}
