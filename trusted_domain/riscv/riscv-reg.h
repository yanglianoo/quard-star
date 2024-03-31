#ifndef RISCV_REG_H_
#define RISCV_REG_H_

//__riscv_xlen 是编译器预定义的宏
#if __riscv_xlen == 32
#define REGSIZE		4
#define REGSHIFT	2
#define LOAD		lw
#define STOR		sw
#elif __riscv_xlen == 64
#define REGSIZE		8
#define REGSHIFT	3
#define LOAD		ld
#define STOR		sd
#endif /* __riscv_xlen */

#endif