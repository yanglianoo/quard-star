#include "stdio.h"

void print_str(const char * fmt, ...) {

#if 0
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