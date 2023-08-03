#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
extern size_t syscall(size_t id, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);
int main() {
    const char *message = "Hello, RISC-V!\n";
    int len = strlen(message);
    int ret = syscall(0x40,1,message, len);
    printf("ret:%d\n",ret);
    return 0;
}
