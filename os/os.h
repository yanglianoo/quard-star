#ifndef __OS_H__
#define __OS_H__



#include <stddef.h>
#include <stdarg.h>
#include "types.h"
#include "context.h"
#include "riscv.h"
/* printf.c */
extern int  printf(const char* s, ...);
extern void panic(char *s);
extern void sbi_console_putchar(int ch);

/* batch.c */
extern void app_init_context();

/* trap.c */
extern void trap_init();
#endif /* __OS_H__ */