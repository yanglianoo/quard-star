#include <timeros/os.h>

void trap_from_kernel()
{
	panic("a trap from kernel!\n");
}

void set_kernel_trap_entry()
{
	w_stvec((reg_t)trap_from_kernel);
}

void set_user_trap_entry()
{
	w_stvec((reg_t)TRAMPOLINE);  
}

void trap_handler()
{
	set_kernel_trap_entry();
	TrapContext* cx = get_current_trap_cx();
    reg_t scause = r_scause();
	reg_t cause_code = scause & 0xfff;
	if(scause & 0x8000000000000000)
	{
		switch (cause_code)
		{
		/* rtc 中断*/
		case 5:
			set_next_trigger();
			schedule();
			break;
		default:
			printk("undfined interrrupt scause:%x\n",scause);
			break;
		}
	}
	else
	{
		switch (cause_code)
		{
		/* U模式下的syscall */
		case 8:
			cx->sepc += 8;
			int result = __SYSCALL(cx->a7,cx->a0,cx->a1,cx->a2);
			cx = get_current_trap_cx();
			cx->a0 = result;
			break;
		default:
			printk("undfined exception scause:%x\n",scause);
			break;
		}
	}
	trap_return();
}

void trap_return()  
{  
    /* 把 stvec 设置为内核和应用地址空间共享的跳板页面的起始地址 */  
    set_user_trap_entry();
    /* Trap 上下文在应用地址空间中的虚拟地址 */  
    u64 trap_cx_ptr = TRAPFRAME;  
    /* 要继续执行的应用地址空间的 token */  
    u64  user_satp = current_user_token();  
	/* 计算_restore函数的虚拟地址 */
    u64  restore_va = (u64)__restore - (u64)__alltraps + TRAMPOLINE;  
	asm volatile (    
			"fence.i\n\t"    
			"mv a0, %0\n\t"  // 将trap_cx_ptr传递给a0寄存器  
			"mv a1, %1\n\t"  // 将user_satp传递给a1寄存器  
			"jr %2\n\t"      // 跳转到restore_va的位置执行代码  
			:    
			: "r" (trap_cx_ptr),    
			"r" (user_satp),
			"r" (restore_va)
			: "a0", "a1"
		);
  
}



