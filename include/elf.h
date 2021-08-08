/*ELF文件结构*/

struct ELF_hdr { /*ELF头结构*/
  uint32_t e_magic;
  uint8_t e_ident[12];
  uint16_t e_type;
  uint16_t e_machine;
  uint16_t e_version;
  uint32_t e_entry; /*程序入口*/
  uint32_t e_phoff;
  uint32_t e_shoff;
  uint16_t e_flags;
  uint16_t e_ehsize;
  uint16_t e_phentsize;
  uint16_t e_phnum;
  uint16_t e_shentsize;
  uint16_t e_shnum;
  uint16_t e_shstrndx;
};

#define EH_MAGIC "\177ELF"

/* e_type */
#define EHT_NONE 0
#define EHT_EXEC 1

/* e_machine */
#define EHM_NONE 0
#define EHM_386 1

/* e_version */
#define EHV_NONE 0
#define EVH_CURRENT 1

struct ELF_phdr { /*程序信息结构*/
  uint32_t p_type;
  uint32_t p_offset;
  uint32_t p_vaddr;
  uint32_t p_paddr;
  uint32_t p_filesz;
  uint32_t p_memsz;
  uint32_t p_flags;
  uint32_t p_align;
};

#define PHT_NULL 0
#define PHT_LOAD 1
#define PHT_GNU_STACK 0x6474e551

struct ELF_shdr { /*节区信息结构*/
  uint16_t s_name;
  uint16_t s_type;
  uint16_t s_flags;
  uint32_t s_size;
  uint32_t s_addr;
  uint32_t s_offset;
};

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3

struct ELF_sym { /*symbol table*/
  uint16_t s_name;
  uint32_t s_value;
  uint16_t s_size;
};

struct ELF_str { /*string table*/
  uint16_t s_size;
  uint8_t s_value[];
};
