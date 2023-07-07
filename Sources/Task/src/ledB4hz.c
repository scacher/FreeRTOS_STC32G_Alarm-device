#include "FreeRTOS.h"
#include "task.h"

#include "ledB4hz.h"

void ledB4hz();

portTASK_FUNCTION( vLEDB4hzTask, pvParameters)
{
	UNUSED( pvParameters );

    while(1)
    {
			vTaskDelay(5);
    }
    vTaskDelete(NULL);
}

void ledB4hz()
{
	//const TickType_t xDelay125ms = pdMS_TO_TICKS( 125UL );
	P47=0;
	vTaskDelay(125);
	P47=1;
	vTaskDelay(125);
}
