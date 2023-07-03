extern sbi_console_putchar(int ch);

void os_main()
{
    for(int i=0;i<6;i++)
    {
    sbi_console_putchar('a');
    }
}