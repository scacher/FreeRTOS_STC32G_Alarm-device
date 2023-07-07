#include "FreeRTOS.h"
#include "task.h"
#include "STC32G_ADC.h"

#include "Infrared.h"

uint8_t flag1;

void prvInfraredInit();

portTASK_FUNCTION( vInfraredTask, pvParameters )
{
  //const TickType_t xDelay10ms = pdMS_TO_TICKS( 10UL );  
	UNUSED( pvParameters );

    prvInfraredInit();
    while(1)
    {
				if(P43==1)
					flag1=1;
				else
					flag1=0;
        vTaskDelay(10);
    }
    
    vTaskDelete(NULL);
}

void prvInfraredInit()
{
		P43=0;
}