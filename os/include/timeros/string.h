#ifndef TOS_STRING_H__
#define TOS_STRING_H__

#include <timeros/os.h>
size_t strlen(const char *str);
void* memcpy(void *dest, const void *src, size_t count);
void* memset(void *dest, int ch, size_t count);
int strcmp(const char *lhs, const char *rhs);
void strncat(char *dest, const char *src, int n);
#endif