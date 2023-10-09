#include <timeros/string.h>
//计算字符串的长度 
size_t strlen(const char *str)
{
    char *ptr = (char *)str;
    while (*ptr != EOS)
    {
        ptr++;
    }
    return ptr - str;
}

// 从存储区 src 复制 n 个字节到存储区 dest。
void* memcpy(void *dest, const void *src, size_t count)
{
    char *ptr = dest;
    while (count--)
    {
        *ptr++ = *((char *)(src++));
    }
    return dest;
}


//复制字符 ch（一个无符号字符）到参数 dest 所指向的字符串的前 n 个字符。
void* memset(void *dest, int ch, size_t count)
{
    char *ptr = dest;
    while (count--)
    {
        *ptr++ = ch;
    }
    return dest;
}


//把 lhs1 所指向的字符串和 rhs2 所指向的字符串进行比较
int strcmp(const char *lhs, const char *rhs)
{
    while (*lhs == *rhs && *lhs!= EOS && *rhs != EOS)
    {
        lhs++;
        rhs++;
    }
    return *lhs < *rhs ? -1 : *lhs > *rhs;
}