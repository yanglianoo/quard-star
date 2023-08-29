#ifndef TOS_STRING_H__
#define TOS_STRING_H__

#include <timeros/os.h>
size_t strlen(const char *str);
void* memcpy(void *dest, const void *src, size_t count);
void* memset(void *dest, int ch, size_t count);

#endif