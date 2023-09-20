#include <timeros/types.h>
#include <timeros/syscall.h>


uint64_t syscall(size_t id, reg_t arg1, reg_t arg2, reg_t arg3) {

    register uintptr_t a0 asm ("a0") = (uintptr_t)(arg1);
    register uintptr_t a1 asm ("a1") = (uintptr_t)(arg2);
    register uintptr_t a2 asm ("a2") = (uintptr_t)(arg3);
    register uintptr_t a7 asm ("a7") = (uintptr_t)(id);

    asm volatile ("ecall"
		      : "+r" (a0)
		      : "r" (a1), "r" (a2), "r" (a7)
		      : "memory");
    return a0;
}

uint64_t sys_gettime()
{
    return syscall(__NR_gettimeofday,0,0,0);
}

int main()
{
    uint64_t current_timer = 0;
    int a = 1;
    int b = a *a + a ;
    while (1)
    {
        current_timer = sys_gettime();
    }
    
    
    return 0;
}
