#ifndef TOS_ELDEF_H__
#define TOS_ELDEF_H__

#include <timeros/os.h>


#define EI_NIDENT 16
//所有的ASCII码都可以用“\”加数字（一般是8进制数字）来表示。
#define ELFMAG 0x464C457FU   // 0x464C457FU  "\177ELF"

#define EM_RISCV 0xF3  //risc-v 对应的 e_machine 的value值

#define EI_CLASS     4 //EI_NIDENT 的 第四位
#define ELFCLASSNONE 0
#define ELFCLASS32   1
#define ELFCLASS64   2
#define ELFCLASSNUM  3

#define PT_LOAD 1

#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4


/**
 * @brief  elf header 结构体定义
 */
typedef struct {
    u8 e_ident[EI_NIDENT];
    u16 e_type;
    u16 e_machine;
    u32 e_version;
    u64 e_entry;
    u64 e_phoff;
    u64 e_shoff;
    u32 e_flags;
    u16 e_ehsize;
    u16 e_phentsize;
    u16 e_phnum;
    u16 e_shentsize;
    u16 e_shnum;
    u16 e_shstrndx;
} elf64_ehdr_t;

/**
 * @brief  program header 结构体定义
 */
typedef struct {
    u32 p_type;
    u32 p_flags;
    u64 p_offset;
    u64 p_vaddr;
    u64 p_paddr;
    u64 p_filesz;
    u64 p_memsz;
    u64 p_align;
} elf64_phdr_t;

#endif