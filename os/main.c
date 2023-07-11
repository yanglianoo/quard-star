extern sbi_console_putchar(int ch);

void os_main()
{
    sbi_console_putchar('h');
    sbi_console_putchar('e');
    sbi_console_putchar('l');
    sbi_console_putchar('l');
    sbi_console_putchar('o');
    sbi_console_putchar('!');
}