// #include <stddef.h>
// #include "os.h"
// #include "context.h"
// #define USER_STACK_SIZE (4096 * 2)
// #define KERNEL_STACK_SIZE (4096 * 2)



// uint8_t KernelStack[KERNEL_STACK_SIZE];
// uint8_t UserStack[USER_STACK_SIZE];





// struct TrapContext  app_init_context(size_t entry, size_t sp)
// {
//     struct TrapContext cx;
//     //reg_t user_sp = &UserStack + USER_STACK_SIZE;


//     reg_t sstatus = r_sstatus();
//     // 设置 sstatus 寄存器第8位即SPP位为0 表示为U模式
//     sstatus &= (0U << 8);
//     w_sstatus(sstatus);

//     cx.sepc = (reg_t)entry;
//     cx.sstatus = sstatus;
//     cx.sp = sp;

//     return cx;



//     // TrapContext* cx_ptr = &KernelStack + KERNEL_STACK_SIZE - sizeof(TrapContext);
//     // cx_ptr->sepc = tasks.sepc;
//     // cx_ptr->sstatus = tasks.sstatus;
//     // cx_ptr->sp = tasks.sp;

//     // __restore(cx_ptr); 
// }


