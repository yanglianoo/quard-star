#include <FreeRTOS.h>
#include <task.h>
#include "debug_log.h"
#include "sbi.h"
#include "riscv_asm.h"
#include "queue.h"
//消息队列控制权柄
QueueHandle_t xMyQueueHandle;

void task1(void *p_arg)
{ 
    int time = 1;
    const char* message = "task1\n";
    for(;;)
    {
        _puts("task1 send\n");
        xQueueSend(xMyQueueHandle,&time,0);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task2(void *p_arg)
{ 
    int time = 2;
    const char* message = "task2\n";

    for(;;)
    {
        _puts("task2 send\n");
        xQueueSend(xMyQueueHandle,&time,0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task3(void *p_arg)
{ 
    int time = 0;
    for(;;)
    {
      // _puts("task3\n");
    
        xQueueReceive(xMyQueueHandle,&time,portMAX_DELAY);
        if(time == 1){
            _puts("task1 receive\n");
        }
        if(time == 2){
            _puts("task2 receive\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTaskCreate ()
{ 

    xTaskCreate(task1,"task1",1024,NULL,3,NULL);
    xTaskCreate(task2,"task2",1024,NULL,4,NULL);
    xTaskCreate(task3,"task2",1024,NULL,5,NULL);
}

int main( void )
{
	_puts( "Hello FreeRTOS!\n");
    const char * str = "taskn\n";
    xMyQueueHandle = xQueueCreate(20,sizeof(int32_t));

    vTaskCreate();
	vTaskStartScheduler();
	return 0;
}

