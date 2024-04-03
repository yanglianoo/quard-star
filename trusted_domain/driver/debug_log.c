#include <FreeRTOS.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "quard_star.h"
#include "ns16550.h"
#include "debug_log.h"

#define UART_LOG_BUFF_SIZE 1024



int _puts(char *str)
{
    int counter = 0;
    if (!str)
    {
        return 0;
    }
    while (*str && (counter < UART_LOG_BUFF_SIZE))
    {
        if(*str == '\n')
	        ns16550_tx( NS16550_ADDR, '\r' );
	    ns16550_tx( NS16550_ADDR, *str++ );
        counter++;
    }
    return counter;
}

