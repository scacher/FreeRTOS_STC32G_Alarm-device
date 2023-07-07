#include "FreeRTOS.h"
#include "task.h"
#include "STC32G_ADC.h"

#include "Power_detection.h"
#include "display.h"

#define ADCKEY_CHANNEL          1                       //ADC按键的输入通道（P1.0）
#define ADC_OFFSET              100  

uint8_t flag;
uint16_t adcResult;
static uint8_t VolState;                              //ADC按键状态
static uint8_t VolState1;
static uint8_t VolState2;
static uint8_t VolState3;
static uint8_t VolState4;
static uint8_t VolState5;

uint16_t prvAdcPower_detection( void );
static void prvVolInit( void );

//typedef struct Result
//{
//	int flag;
//	int adc1Result;
//}
//Result,*PResult;

portTASK_FUNCTION( vPowerTask, pvParameters )
{
    UNUSED( pvParameters );
		prvVolInit();
    while(1)
    {
        adcResult=prvAdcPower_detection();
//				if(!flag)
//				{
					pucLEDBuffer[4] = adcResult/ 1000;    
					pucLEDBuffer[5] = adcResult% 1000/100;
					pucLEDBuffer[6] = adcResult% 1000%100/10;
					pucLEDBuffer[7] = adcResult% 1000%100%10;
//				}
//				else
//				{
//					pucLEDBuffer[4] = 1;    //显示电压值
//					pucLEDBuffer[5] = 1;
//					pucLEDBuffer[6] = 1;
//					pucLEDBuffer[7] = 1;
//				}
				vTaskDelay(200);
    }
    
    vTaskDelete(NULL);
} 

static void prvVolInit( void )
{
//    P1M1 |= 0x01;                                       //设置 P1.0 为 ADC 输入口
//    P1M0 &= ~0x01;

    VolState  = 0;
    VolState1 = 0;
    VolState2 = 0;
    VolState3 = 0; 
		VolState4 = 0;
		VolState5 = 0;	
		flag=0;
}

uint16_t prvAdcPower_detection( void )
{

    uint16_t usResult;
//    uint8_t flag;
		VolState5 = VolState4;
		VolState4 = VolState3;
		VolState3 = VolState2;
    VolState2 = VolState1;
    usResult = Get_ADCResult(ADCKEY_CHANNEL);
		if((usResult>=(4050-ADC_OFFSET))&&
				(usResult<=(4050+ADC_OFFSET)))
			VolState1=1;
		else
			VolState1=0;
		if ( (VolState5 == VolState4)&&
				 (VolState4 == VolState3)&&
				 (VolState3 == VolState2)&&
				 (VolState2 == VolState1)&&
					(VolState5 > 0) &&
					(VolState4 > 0) &&
					(VolState3 > 0) &&
					(VolState2 > 0) &&
					(VolState1 > 0))
			flag=1;
		else
			flag=0;
		
		return usResult;
}
