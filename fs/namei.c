#include "types.h"

uint32_t next_filename(char* pathname)
{
  uint32_t i;

  if (pathname != NULL) {
    if (*pathname == '/') return 0;
    for (i = 0; pathname[i]; i++) {
      if (pathname[i] == '/') {
        pathname[i] = '\0';
        return i;
      }
    }
  }

  return -1;
}

/* pathname - 文件路径
 * 输出：锁定的inode
 * */
int namei(char* pathname)
{
  return 0;
}
