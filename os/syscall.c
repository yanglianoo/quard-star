#include "os.h"


void __SYSCALL(size_t syscall_id, reg_t arg1, reg_t arg2, reg_t arg3) {
        switch (syscall_id)
        {
        case sys_write:
            __sys_write(arg1, arg2, arg3);
            break;
        default:
            panic("Unsupported syscall id");
            break;
        }
}


void __sys_write(size_t fd, const char* data, size_t len)
{
    if(fd ==1)
    {
        printf(data);
    }
    else
    {
        panic("Unsupported fd in sys_write!");
    }
}