
/*
 * FreeRTOS STC port V1.0.2
 * 
 * STC32G12K128���ܼ�飺
 *    2�� CAN, 3��Lin, USB, 32λ 8051, RMB5,
 *    �߾�׼12λADC, DMA ֧��(TFT����, ADC, 4�鴮��, SPI, I2C )
 *  
 *  ��λ�û��ں�����ʹ�ù����У���������в���ȷ�ĵط�����
 *  �пɸ��Ƶĵط������STC��ϵ����STC�ٷ�����ͳһ�޸Ĳ����°汾
 *
 *  �����Ĵ������������Ҫ���ñ�������еĲ�����ֲ���룬��������
 *  �����������ע���ο���ʹ����STC��FreeRTOS����ֲ����
*/

#include "FreeRTOS.h"
#include "task.h"
#include "System_init.h"

#include "display.h"
//#include "rtc.h"
#include "led.h"
#include "adckey.h"
#include "DS18B20.h"
//#include "uart3.h"
#include "Power_detection.h"
#include "Infrared.h"
#include "general.h"
#include "oled.h"
#include "beep.h"


void vSystemInit( void );

void main( void )
{
    /* ϵͳ��ʼ�� */
    vSystemInit();

	/* �������� */

    xTaskCreate((TaskFunction_t )vDisplayTask,
                (const char*    )"DISPLAY",
                (uint16_t       )configDEFAULT_STACK_SIZE/2,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES+1),
                (TaskHandle_t*  )NULL);

		xTaskCreate((TaskFunction_t )vOLEDTask,
                (const char*    )"OLED",
                (uint16_t       )configDEFAULT_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES),
                (TaskHandle_t*  )NULL);
								
		xTaskCreate((TaskFunction_t )vDS18B20Task,
                (const char*    )"DS18B20",
                (uint16_t       )configDEFAULT_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES),
                (TaskHandle_t*  )NULL);
								
		xTaskCreate((TaskFunction_t )vLEDTask,
                (const char*    )"LED",
                (uint16_t       )configDEFAULT_STACK_SIZE/2 ,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES),
                (TaskHandle_t*  )NULL);
								
		xTaskCreate((TaskFunction_t )vAdcKeyTask,
                (const char*    )"ADCKEY",
                (uint16_t       )configDEFAULT_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES),
                (TaskHandle_t*  )NULL);
								
		xTaskCreate((TaskFunction_t )vPowerTask,
                (const char*    )"Power_detection",
                (uint16_t       )configDEFAULT_STACK_SIZE/2,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES),
                (TaskHandle_t*  )NULL);
								
		xTaskCreate((TaskFunction_t )vInfraredTask,
                (const char*    )"Infrared",
                (uint16_t       )configDEFAULT_STACK_SIZE/2,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES),
                (TaskHandle_t*  )NULL);
								
		xTaskCreate((TaskFunction_t )vPWMA_beepTask,
                (const char*    )"PWMA_beep",
                (uint16_t       )configDEFAULT_STACK_SIZE/2,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES),
                (TaskHandle_t*  )NULL);
														
		xTaskCreate((TaskFunction_t )vgeneralTask,
                (const char*    )"general",
                (uint16_t       )configDEFAULT_STACK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(configDEFAULT_PRIORITIES),
                (TaskHandle_t*  )NULL);
								
//		xTaskCreate((TaskFunction_t )vUart3Task,
//                (const char*    )"UART3",
//                (uint16_t       )configDEFAULT_STACK_SIZE,
//                (void*          )1000,  //ע��pvParameters��������ַ����������ʱֻ�е�24λ����Чλ�����8λ����ʱ���Զ���0. ��ͨ���������߳�������32�ֽ�����.
//                (UBaseType_t    )(configDEFAULT_PRIORITIES),
//                (TaskHandle_t*  )NULL);
    /* ���������������ʼ������� */
    vTaskStartScheduler();

    /* ��������²������е��˴� */
    while (1);
}
