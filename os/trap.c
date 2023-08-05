#include "os.h"
#include "context.h"
#include "riscv.h"



pt_regs* trap_handler(pt_regs* cx)
{
    reg_t scause = r_scause() ;
	switch (scause)
	{
	case 8:
			__SYSCALL(cx->a7,cx->a0,cx->a1,cx->a2);
		break;
	
	default:
			panic("unknown scause\n");
		break;
	}

	cx->sepc += 8;
	__restore(cx);
	return cx;
}


void trap_init()
{
	/*
	 * 设置 trap 时调用函数的基地址
	 */
	w_stvec((reg_t)__alltraps);
}