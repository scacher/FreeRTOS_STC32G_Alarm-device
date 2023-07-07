#include "FreeRTOS.h"
#include "task.h"

#include "ledC4hz.h"

void ledC4hz();

portTASK_FUNCTION( vLEDC4hzTask, pvParameters)
{
	UNUSED( pvParameters );
    while(1)
    {
			vTaskDelay(5);
    }
    vTaskDelete(NULL);
}

void ledC4hz()
{
	P44=1;
	vTaskDelay(125);
	P44=0;
	vTaskDelay(125);
}