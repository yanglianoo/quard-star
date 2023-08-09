#include "os.h"
#include "context.h"
#include "riscv.h"



TrapContext* trap_handler(TrapContext* cx)
{
    reg_t scause = r_scause();
	switch (scause)
	{
	case 8:
			__SYSCALL(cx->a7,cx->a0,cx->a1,cx->a2);
		break;
	default:
			printf("undfined scause:%d\n",scause);
			//panic("error!");
		break;
	}
	
	cx->sepc += 8;

	return cx;
}


void trap_init()
{
	/*
	 * 设置 trap 时调用函数的基地址
	 */
	w_stvec((reg_t)__alltraps);
}