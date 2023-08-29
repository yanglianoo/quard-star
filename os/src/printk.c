#include <timeros/os.h>
#include <timeros/stdio.h>
void uart_puts(char *s)
{
	while (*s) {
		sbi_console_putchar(*s++);
	}
}


static char out_buf[1000]; // buffer for _vprintf()
static int _vprintf(const char* s, va_list vl)
{
	int res = _vsnprintf(NULL, -1, s, vl);
	if (res+1 >= sizeof(out_buf)) {
		uart_puts("error: output string size overflow\n");
		while(1) {}
	}
	_vsnprintf(out_buf, res + 1, s, vl);
	uart_puts(out_buf);
	return res;
}

int printk(const char* s, ...)
{
	int res = 0;
	va_list vl;
	va_start(vl, s);
	res = _vprintf(s, vl);
	va_end(vl);
	return res;
}

void panic(char *s)
{
	printk("panic: ");
	printk(s);
	printk("\n");
	while(1){};
}