#ifndef QUARD_STAR_H_
#define QUARD_STAR_H_



#ifdef __ASSEMBLER__
#define CONS(NUM, TYPE)NUM
#else
#define CONS(NUM, TYPE)NUM##TYPE
#endif /* __ASSEMBLER__ */

#define PRIM_HART			7

//NS16550 串口映射的地址
#define NS16550_ADDR		CONS(0x10002000, UL)


#ifndef __ASSEMBLER__

#endif /* __ASSEMBLER__ */


#endif