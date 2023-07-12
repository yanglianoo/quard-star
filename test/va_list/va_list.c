#include "stdio.h"
//将可变参数全部入栈
typedef void* Va_list;
//在64位系统栈帧分配的单元大小是8字节(一个参数占8字节)
#define Va_start(ap,v) (ap = (Va_list)&v )
#define Va_arg(ap,t) (*(t*)(ap += sizeof(t *)))
#define Va_end(ap) (ap = (Va_list)0)   //直接将Va_list 置为空指针


void print_str(const char *fmt, ...) {

#if 1
    void* ap = (void*)&fmt;
    printf("%s\n", *(char**)ap);
    ap += sizeof(char**);
    printf("%s\n", *(char**)ap);
    ap += sizeof(char**);
    printf("%s\n", *(char**)ap);
    ap += sizeof(char**);
    printf("%s\n", *(char**)ap);
#else
    void* ap = (void*)&fmt;
    printf("%s\n", *(char**)ap);
    ap += sizeof(char**) * 6;
    printf("%s\n", *(char**)ap);
    ap += sizeof(char**);
    printf("%s\n", *(char**)ap);
    ap += sizeof(char**);
    printf("%s\n", *(char**)ap);





#endif




}




int main() {
    char * s1 = "hello";
    char * s2 = "world";
    char * s3 = "fuck you";
    char * s4 = "riscv";
    print_str(s1,s2,s3,s4);
    return 0;
}