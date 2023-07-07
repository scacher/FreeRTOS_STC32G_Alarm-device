/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/


/*************  功能说明    **************

本例程基于STC32G为主控芯片的实验箱9进行编写测试.

短接实验箱上的J7、J8跳线进行测试.

串口3定时发送一串数据给串口2.

串口2输出从串口3接收到的数据.

往串口2发送数据，MCU收到后从串口2原样输出.

******************************************/

#include "FreeRTOS.h"
#include "task.h"

#include "STC32G_UART.h"
#include "STC32G_GPIO.h"
#include "display.h"
#include "uart3.h"

extern void TX3_write2buff(uint8_t dat);
void uart3_gpio_config(void);

uint16_t Sec_Cnt;    //1秒计数

/* UART任务函数 */
portTASK_FUNCTION( vUart3Task, pvParameters )
{
    uint8_t i;
    uint8_t temp;
	  uart3_gpio_config();
//    Sec_Cnt = 0;

//    PrintString3("STC32G UART3 Test Programme!\r\n");  //UART2发送一个字符串
    //PrintString3("STC32G UART3-UART2 Test Programme!\r\n");  //UART3发送一个字符串
    while(1)
    {
//        Sec_Cnt++;
//        if(Sec_Cnt >= (uint16_t)pvParameters)
//        {
//            Sec_Cnt = 0;
//            PrintString3("STC32G UART3 Test Programme!\r\n");  //UART3发送一个字符串
//        }
//				PrintString3("STC32G UART3 Test Programme!\r\n");
        if(COM3.RX_TimeOut > 0)
        {
            if(--COM3.RX_TimeOut == 0)
            {
                if(COM3.RX_Cnt > 0)
                {
                    for(i=0; i<COM3.RX_Cnt; i++);    
												//TX3_write2buff(RX3_Buffer[1]);    //把收到的数据通过串口2输出
										temp=RX3_Buffer[63];
                    COM3.RX_Cnt = 0;   //清除字节数
										pucLEDBuffer[1]=temp/100;
										pucLEDBuffer[2]=temp%100/10;
										pucLEDBuffer[3]=temp%100%10;
                }
            }
        }
        
        vTaskDelay(1);
    }
    vTaskDelete(NULL);
}   
void uart3_gpio_config(void)
{
	P5_MODE_IN_HIZ(GPIO_Pin_0);
	P5_MODE_IN_HIZ(GPIO_Pin_1);
}