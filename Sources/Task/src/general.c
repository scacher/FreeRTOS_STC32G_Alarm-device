#include "FreeRTOS.h"
#include "task.h"

#include "adckey.h"
#include "led.h"
//#include "ledC4Hz.h"
//#include "ledB4Hz.h"
#include "display.h"
#include "Power_detection.h"
#include "Infrared.h"
#include "DS18B20.h"
#include "general.h"
#include "oled.h"
#include "beep.h"

#define normal_state 0
#define alarm_state  1 //报警状态
#define manage_state 2 //处理状态

u8 general_mode=0;


portTASK_FUNCTION( vgeneralTask, pvParameters)
{
	u8 led_delay=0;
	UNUSED( pvParameters );

    while(1)
    {
			if(flag || flag1 || flag2)
			{
				general_mode=alarm_state;
			}
			if(ucKeyCode==1) //按下处理按键
			{
				ucKeyCode=0;
				general_mode=manage_state ;
			}
			if((general_mode==alarm_state) && ucKeyCode==2 && (!flag && !flag1 && !flag2)) //ucKeyCode==2恢复按键
			{
				ucKeyCode=0;
				general_mode=normal_state ;
			}
			switch(general_mode)
			{
				case  normal_state :  {pucLEDBuffer[0]=0; ledA1hz();break;}
				case  alarm_state  :  {pucLEDBuffer[0]=1;ledB4hz();break;}
				case  manage_state :  {pucLEDBuffer[0]=2;while(led_delay<10){ledB4C4hz();led_delay++;}led_delay=0;general_mode=normal_state;break;}//处理时间(5s)到了之后自动回归初始状态
			}

			vTaskDelay(5);
    }
    vTaskDelete(NULL);
}
