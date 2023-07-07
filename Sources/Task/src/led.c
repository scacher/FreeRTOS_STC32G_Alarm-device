#include "FreeRTOS.h"
#include "task.h"

#include "adckey.h"
#include "led.h"
#include "display.h"
#include "Power_detection.h"

void ledA1hz();
//void ledB4hz();
//void ledC4hz();
void prvLEDInit();


portTASK_FUNCTION( vLEDTask, pvParameters)
{
	UNUSED( pvParameters );
    
    prvLEDInit();
    while(1)
    {
			vTaskDelay(5);
    }
    vTaskDelete(NULL);
}

void prvLEDInit()
{
	P40=0;
	P44=0;
}

void ledA1hz()
{
	//const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );
	P46=0;
	vTaskDelay(500);
	P46=1;
	vTaskDelay(500);
}

void ledB4hz()
{
	//const TickType_t xDelay125ms = pdMS_TO_TICKS( 125UL );
	P47=0;
	vTaskDelay(125);
	P47=1;
	vTaskDelay(125);
}


void ledC4hz()
{
	//const TickType_t xDelay125ms = pdMS_TO_TICKS( 125UL );
	P44=0;
	vTaskDelay(125);
	P44=1;
	vTaskDelay(125);
}

void ledB4C4hz()
{
	P47=0;P44=1;
	vTaskDelay(125);
	P47=1;P44=0;
	vTaskDelay(125);
}