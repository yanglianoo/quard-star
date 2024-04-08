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
	_puts( "vTaskCreate\n");

    xTaskCreate(task1,"task1",2048,NULL,4,NULL);
    xTaskCreate(task2,"task2",2048,NULL,5,NULL);

    vTaskDelete(NULL);
}

int main( void )
{
	_puts( "Hello FreeRTOS!\n");
    vTaskCreate();
   /// xTaskCreate(vTaskCreate,"task creat",256,NULL,4,NULL);

	vTaskStartScheduler();
	return 0;
}

