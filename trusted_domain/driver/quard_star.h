#ifndef QUARD_STAR_H_
#define QUARD_STAR_H_



#ifdef __ASSEMBLER__
#define CONS(NUM, TYPE)NUM
#else
#define CONS(NUM, TYPE)NUM##TYPE
#endif /* __ASSEMBLER__ */

#define PRIM_HART			7
#define CLINT_ADDR			CONS(0x02000000, UL)
#define CLINT_MSIP			CONS(0x0000, UL)
#define CLINT_MTIMECMP		CONS(0x4000, UL)
#define CLINT_MTIME			CONS(0xbff8, UL)
//NS16550 串口映射的地址
#define NS16550_ADDR		CONS(0x10002000, UL)


#ifndef __ASSEMBLER__

#endif /* __ASSEMBLER__ */


#endif