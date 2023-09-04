#ifndef TOS_ADDRESS_H
#define TOS_ADDRESS_H

#include <timeros/os.h>
#include <timeros/stack.h>
#include <timeros/string.h>
#include <timeros/assert.h>

// Sv39的最大地址空间 512G
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))

//跳板页开始位置
#define TRAMPOLINE (MAXVA - PGSIZE)

//计算应用内核栈的地址，每个应用的内核栈下都有一个无效的守卫页
#define KSTACK(p) (TRAMPOLINE - ((p)+1)* 2*PGSIZE)

#define PAGE_SIZE 0x1000      // 4kb  一页的大小
#define PAGE_SIZE_BITS   0xc  // 12   页内偏移地址长度

#define PA_WIDTH_SV39 56      //物理地址长度
#define VA_WIDTH_SV39 39      //虚拟地址长度
#define PPN_WIDTH_SV39 (PA_WIDTH_SV39 - PAGE_SIZE_BITS)  // 物理页号 44位 [55:12]
#define VPN_WIDTH_SV39 (VA_WIDTH_SV39 - PAGE_SIZE_BITS)  // 虚拟页号 27位 [38:12]

#define MEMORY_END 0x80800000    // 0x80200000 ~ 0x80800000
#define KERNBASE 0x80200000L  

/* 物理地址 */
typedef struct {
    uint64_t value; 
} PhysAddr;

/* 虚拟地址 */
typedef struct {
    uint64_t value;
} VirtAddr;

/* 物理页号 */
typedef struct {
    uint64_t value;
} PhysPageNum;

/* 虚拟页号 */
typedef struct {
    uint64_t value;
} VirtPageNum;



/* 定义页表项 */
typedef struct  
{
    uint64_t bits;
}PageTableEntry;

// 定义位掩码常量
#define PTE_V (1 << 0)   //有效位
#define PTE_R (1 << 1)   //可读属性
#define PTE_W (1 << 2)   //可写属性
#define PTE_X (1 << 3)   //可执行属性
#define PTE_U (1 << 4)   //用户访问模式
#define PTE_G (1 << 5)   //全局映射
#define PTE_A (1 << 6)   //访问标志位
#define PTE_D (1 << 7)   //脏位

/* 定义页表 */
typedef struct {
    PhysPageNum root_ppn; //根节点
    //Stack frames;         //页帧
}PageTable;


// 
typedef struct SimpleRange {
    u64 l;
    u64 r;
} SimpleRange;

// 内存映射方式
typedef enum 
{
    Identical, //恒等映射
    Framed,    //随机映射
}MapType;

typedef struct {
    SimpleRange vpn_range;
    MapType map_type;
    u8 map_perm;

}MapArea;
#endif