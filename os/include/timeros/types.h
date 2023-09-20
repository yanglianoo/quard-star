#ifndef TOS_TYPES_H__
#define TOS_TYPES_H__

// 定义无符号整型
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int  uint32_t;
typedef unsigned long size_t;
typedef unsigned long long uint64_t;

typedef uint64_t u64;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint8_t u8;

typedef unsigned long int uintptr_t;
/*
 * RISCV64: 寄存器的大小是64位的
 */ 
typedef uint64_t reg_t;


#define EOF -1  
#define NULL ((void *)0)
#define EOS '\0'

#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0
#endif

#endif