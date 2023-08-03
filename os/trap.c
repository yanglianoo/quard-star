#include "os.h"
#include "context.h"
#include "riscv.h"

extern void __alltraps(void);

pt_regs* trap_handler(pt_regs* cx)
{
    reg_t scause = r_scause() ;
    printf("cause:%x\n",scause);
	printf("a0:%x\n",cx->a0);
	printf("a1:%x\n",cx->a1);
	printf("a2:%x\n",cx->a2);
	printf("a7:%x\n",cx->a7);
	printf("sepc:%x\n",cx->sepc);
	printf("sstatus:%x\n",cx->sstatus);
	printf("sp:%x\n",cx->sp);
	while (1)
	{
	}
    return cx;
}


void trap_init()
{
	/*
	 * 设置 trap 时调用函数的基地址
	 */
	w_stvec((reg_t)__alltraps);
}