#include <timeros/os.h>
#include <timeros/stdio.h>
void __sys_write(size_t fd, const char* data, size_t len)
{
    if(fd == stdout || fd == stderr)
    {
        printk(data);
    }
    else
    {
        panic("Unsupported fd in sys_write!");
    }
}

void __sys_yield()
{
    schedule();
}

uint64_t __sys_gettime()
{
    return get_time_us();
}

uint64_t __SYSCALL(size_t syscall_id, reg_t arg1, reg_t arg2, reg_t arg3) {
        switch (syscall_id)
        {
        
        case __NR_write:
            __sys_write(arg1, arg2, arg3);
            break;
        case __NR_sched_yield:
            __sys_yield();
            break;
        case __NR_gettimeofday:
            return __sys_gettime();
        default:
            printk("Unsupported syscall id:%d\n",syscall_id);
            break;
        }
}
