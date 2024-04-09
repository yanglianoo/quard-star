#include <FreeRTOS.h>
#include <task.h>
#include "debug_log.h"
#include "sbi.h"
#include "riscv_asm.h"

void task1(void *p_arg)
{ 
    int time = 0;
    for(;;)
    {
        _puts("task1\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task2(void *p_arg)
{ 
    int time = 0;
    for(;;)
    {
        _puts("task2\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTaskCreate ()
{ 

    xTaskCreate(task1,"task1",1024,NULL,4,NULL);
    xTaskCreate(task2,"task2",1024,NULL,4,NULL);
}

int main( void )
{
	_puts( "Hello FreeRTOS!\n");
    vTaskCreate();
	vTaskStartScheduler();
	return 0;
}

