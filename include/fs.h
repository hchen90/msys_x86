#ifndef _FS_H_
#define _FS_H_

#define S_IFSOCK 0140000
#define S_IFLNK  0120000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000
#define S_ISUID  0004000
#define S_ISGID  0002000
#define S_ISVTX  0001000

#define S_IRUSR  00400
#define S_IWUSR  00200
#define S_IXUSR  00100

#define S_IRGRP  00040
#define S_IWGRP  00020
#define S_IXGRP  00010

#define S_IROTH  00004
#define S_IWOTH  00002
#define S_IXOTH  00001

/*使用MINIX格式的文件系统*/
struct minix_super_block {
  unsigned short s_ninodes; /* number of inodes */
  unsigned short s_nzones; /* number of data zones */
  unsigned short s_imap_blocks; /* space used by inode map */
  unsigned short s_zmap_blocks; /* space used by zone map */
  unsigned short s_firstdatazone; /* first zone with 'file' data */
  unsigned short s_log_zone_size; /* size of a data zone */
  unsigned long  s_max_size; /* maximum file size */
  unsigned short s_magic; /* 0x138f */
  unsigned short s_state; /* 1 - mounted, 0 - unmounted */
};

struct minix_inode {
  unsigned short i_mode;
  unsigned short i_uid;
  unsigned long  i_size;
  unsigned long  i_time;
  unsigned char  i_gid;
  unsigned char  i_nlinks;
  unsigned short i_zone[9];
};


struct inode {
  unsigned int i_ino;
  unsigned int i_mode;
  unsigned int i_nlinks;
  unsigned int i_uid;
  unsigned int i_gid;
  unsigned int i_size;
  unsigned int i_atime;
  unsigned int i_ctime;
  unsigned int i_mtime;
  unsigned int i_flags;
  unsigned int i_zone[9];
  unsigned int i_locked;
};

#define O_RDONLY 0x01
#define O_WRONLY 0x02
#define O_RDWR   (O_RDONLY | O_WRONLY)
#define O_CREAT  0x04
#define O_APPEND 0x08

struct file {
  unsigned int f_mode;
  unsigned int f_off;
  unsigned int f_flags;
  struct inode* f_inode;
};

struct dirent {
  unsigned int d_reclen;
  unsigned char d_name[256];
  struct inode* d_inode;
};

int bmap_get(unsigned int nblk);
void bmap_set(unsigned int nblk);
void bmap_clear(unsigned int nblk);
int imap_get(unsigned int ino);
void imap_set(unsigned int ino);
void imap_clear(unsigned int ino);

int dir_readdir(struct inode* inop, struct file* filp, struct dirent* ent, int len);
int file_read(struct inode* inop, struct file* filp, char* buf, int len);
int file_write(struct inode* inop, struct file* filp, char* buf, int len);

unsigned int iget(unsigned int ino);
void iput(unsigned int ino);
unsigned int ialloc(void);
void ifree(unsigned int ino);

int namei(char* pathname);

int hread(int lba, void* ptr, int count);
int hwrite(int lba, void* ptr, int count);

void super_init(void);
#endif
