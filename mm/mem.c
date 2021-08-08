#include "sys.h"
#include "x86.h"
#include "unistd.h"
#include "proc.h"

#define MAP_PHY_BASE 0x200000
#define MAP_VIR_BASE 0x800000

/*初始化*/
void mem_setup(void)
{
  uint32_t baddr = 0, i;
  uint32_t* pdt = (uint32_t*) 0x40000;
  uint32_t* ptt = (uint32_t*) 0x50000;

  FUN(memset)(pdt, 0, 1024 * 4);
  
  /*目录项*/
  pdt[0] = (uint32_t) ptt | 3; /*R=1, P=1*/
  pdt[1023] = (uint32_t) pdt | 3; /*R=1, P=1*/
  
  /*页面表*/
  for (i = 0; i < 1024; i++) { /*低端4M有效*/
    ptt[i] = (uint32_t) baddr | 3; /*登记页面*/
    baddr += 0x1000;
  }

  set_cr3((uint32_t) pdt & ~0xfff | 0x000); /*设置PDE*/
  set_cr0(get_cr0() | 0x80000000);
}

void* get_phyaddr(void* virtaddr)
{
  uint32_t pdindex = ((uint32_t) virtaddr) >> 22;
  uint32_t ptindex = ((uint32_t) virtaddr) >> 12 & 0x3ff;

  uint32_t* pd = (uint32_t*) 0xfffff000;
  uint32_t* pt = ((uint32_t*) 0xffc00000) + (0x400 * pdindex);

  if (pd[pdindex] & 0x1 && pt[ptindex] & 0x1) { /*检查是否存在*/
    return (void*) ((pt[ptindex] & ~0xfff) + ((uint32_t) virtaddr & 0xfff));
  } else return NULL;
}

/*将虚拟地址映射到物理地址上*/
int map_page(void* phyaddr, void* virtaddr, uint32_t flags)
{
  uint32_t pdindex = ((uint32_t) virtaddr) >> 22;
  uint32_t ptindex = ((uint32_t) virtaddr) >> 12 & 0x3ff;

  uint32_t* pd = (uint32_t*) 0xfffff000;
  uint32_t* pt = ((uint32_t*) 0xffc00000) + (0x400 * pdindex);

  if ((uint32_t) phyaddr & 0xfff) {
    logsc(1, "physical address must be 4K-aligned");
  }

  if (! pd[pdindex] & 0x1) { /*如果页目录不存在登记该目录*/
    pd[pdindex] = ((uint32_t) pt) | 0x1;
  }

  if (! pt[ptindex] & 0x1) {
    pt[ptindex] = ((uint32_t) phyaddr & ~0xfff) | (flags & 0xfff) | 0x1;
  } else return -1; /*如果已经映射，直接返回-1*/

  return 0;
}

/*将虚拟地址取消映射*/
int umap_page(void* virtaddr)
{
  uint32_t pdindex = ((uint32_t) virtaddr) >> 22;
  uint32_t ptindex = ((uint32_t) virtaddr) >> 12 & 0x3ff;

  uint32_t* pd = (uint32_t*) 0xfffff000;
  uint32_t* pt = ((uint32_t*) 0xffc00000) + (0x400 * pdindex);

  if (pd[pdindex] & 0x1 && pt[ptindex] & 0x1) {
    pt[ptindex] = 0;
  } else return -1;

  return 0;
}

void* iner_free_base = (void*) MAP_PHY_BASE;
void* iner_free_top = (void*) MAP_PHY_BASE;
void* iner_virt_base = (void*) MAP_VIR_BASE;
void* iner_virt_top = (void*) MAP_VIR_BASE;

void* new_link_memory(void* base, void* top, size_t len)
{
  int old = 0;

  while (base < top && *(uint32_t*) base > 0) { /*size*/
    if (! *(uint8_t*) (base + 4) && *(uint32_t*) base >= (len + 8)) {
      old = 1;
      break; // 如果该块没有占用，并且大小合适，则直接终止查找
    }
    base += *(uint32_t*) base;
  }

  if (base < top) {
    if ((uint32_t) (top - base - 8) > len) {
      if (! old) {
        len = 64 * ((len - 1) / 64 + 1); // 长度对齐
        *(uint32_t*) base = len + 8;
        if (base + len + 8 < top) {
          *(uint32_t*) (base + len + 8) = 0;
        }
      }
      *(uint8_t*) (base + 4) = 1; // 设置占用位
      return base;
    }
  }

  return NULL;
}

void free_link_memory(void* base, void* top, void* ptr)
{
  while (base < top && *(uint32_t*) base > 0 && ptr != base + 8) {
    base += *(uint32_t*) base;
  }

  if (base < top && ptr == base + 8) {
    *(uint8_t*) (base + 4) = 0; // 清除占用位
  }
}

void* new_memory(struct proc* procp, size_t len)
{
  void* free_base = iner_free_base,* free_top = iner_free_top, \
      * virt_base = iner_virt_base,* virt_top = iner_virt_top;
  void* mptr;
  uint32_t cr3 = get_cr3();
  uint32_t num = (len + 8 - 1) / 4096 + 1, i;

  if (procp != NULL) {
    set_cr3(procp->ctx.cr3); // 设置页目录PDE
    free_base = procp->mf.free_base;
    free_top = procp->mf.free_top;
    virt_base = procp->mf.virt_base;
    virt_top = procp->mf.virt_top;
  }

  if ((mptr = new_link_memory(free_base, free_top, len)) == NULL) {
    for (i = 0; i < num; i++) {
      map_page(free_top, virt_top, 0x03);
      free_top += 4096; virt_top += 4096;
    }

    *(uint32_t*) free_base = 0;
    *(uint8_t*) (free_base + 4) = 0;

    mptr = new_link_memory(free_base, free_top, len);
  }

  if (procp != NULL) {
    procp->mf.free_top = free_top;
    procp->mf.virt_top = virt_top;
  } else {
    iner_free_top = free_top;
    iner_virt_top = virt_top;
  }

  set_cr3(cr3);

  if (mptr != NULL) {
    mptr += 8;
  }

  return mptr;
}

void free_memory(struct proc* procp, void* ptr)
{
  void* free_base = iner_free_base,* free_top = iner_free_top;
  uint32_t cr3 = get_cr3();

  if (procp != NULL) {
    set_cr3(procp->ctx.cr3); // 设置页目录PDE
    free_base = procp->mf.free_base;
    free_top = procp->mf.free_top;
  }

  free_link_memory(free_base, free_top, ptr);

  set_cr3(cr3);
}
