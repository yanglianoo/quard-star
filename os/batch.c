#include <stddef.h>
#include "os.h"
#include "context.h"
#define USER_STACK_SIZE (4096 * 2)
#define KERNEL_STACK_SIZE (4096 * 2)
#define APP_BASE_ADDRESS 0x80600000


size_t syscall(size_t id, reg_t arg1, reg_t arg2, reg_t arg3) {
    long ret;
    asm volatile (
        "mv a7, %1\n\t"   // Move syscall id to a0 register
        "mv a0, %2\n\t"   // Move args[0] to a1 register
        "mv a1, %3\n\t"   // Move args[1] to a2 register
        "mv a2, %4\n\t"   // Move args[2] to a3 register
        "ecall\n\t"       // Perform syscall
        "mv %0, a0"       // Move return value to 'ret' variable
        : "=r" (ret)
        : "r" (id), "r" (arg1), "r" (arg2), "r" (arg3)
        : "a7", "a0", "a1", "a2", "memory"
    );
    return ret;
}

void testsys() {
    
    // //int len = strlen(message);
    //reg_t sstatus = r_sstatus();
    //printf("sstatus:%x\n", sstatus);
    int ret = syscall(2,3,4,5);
    // while (1)
    // {
    //     /* code */
    // }
    
    //printf("ret:%d\n",ret);
}


uint8_t KernelStack[KERNEL_STACK_SIZE];
uint8_t UserStack[USER_STACK_SIZE]={0};

extern void __restore(pt_regs *next);

struct pt_regs tasks;
void app_init_context()
{

    reg_t user_sp = &UserStack + USER_STACK_SIZE;
    printf("user_sp:%p\n", user_sp);

    reg_t stvec = r_stvec();
    printf("stvec:%x\n", stvec);


    trap_init();

    reg_t sstatus = r_sstatus();
    // 设置 sstatus 寄存器第8位即SPP位为0 表示为U模式
    sstatus &= (0U << 8);
    w_sstatus(sstatus);
    printf("sstatus:%x\n", sstatus);


    tasks.sepc = (reg_t)testsys;
    printf("tasks sepc:%x\n", tasks.sepc);

    tasks.sstatus = sstatus;

    tasks.sp = user_sp;



    pt_regs* cx_ptr = &KernelStack[0] + KERNEL_STACK_SIZE - sizeof(pt_regs);
    printf("pt_regs: %d\n",sizeof(pt_regs));
    cx_ptr->sepc = tasks.sepc;
    printf("cx_ptr sepc :%x\n", cx_ptr->sepc);
    printf("cx_ptr sepc adress:%x\n", &(cx_ptr->sepc));
    cx_ptr->sstatus = tasks.sstatus;
    cx_ptr->sp = tasks.sp;
    // *cx_ptr = tasks[0];
    printf("cx_ptr adress:%x\n", cx_ptr);
    
    __restore(cx_ptr); 

}


