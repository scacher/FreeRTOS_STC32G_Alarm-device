#include "FreeRTOS.h"
#include "task.h"

#include "beep.h"

#define beep P52
#define on   0
#define off  1
#define uchar unsigned char

//bit flag3;  //标志音乐输出脚电平的高低
//uchar ptr=0x00;
//uchar high;
//uchar low;
//uchar code music[]={
//0xfc,0x44,0x7f, 0xfc,0x44,0x7f, 0xfc,0x44,0xff, 0xfa,0x68,0xff,
//0xfd,0x23,0x7f, 0xfd,0x23,0x7f, 0xfd,0x23,0xff, 0xfc,0x44,0xff,
//0xfc,0x44,0x7f, 0xfd,0x23,0x7f, 0xfd,0x82,0xff, 0xfd,0x82,0xff,
//0xfd,0x23,0x7f, 0xfd,0x23,0x7f, 0xfc,0xac,0xff, 0xff,0xff,0xff,
//0xfc,0xac,0x7f, 0xfd,0x23,0x7f, 0xfd,0x34,0xff, 0xfd,0x34,0xff,
//};    //happy new year 歌曲

//void PWMA_Output_init(void);
//void Init();
void beep_1hz(void);
void beep_4hz(void);
void beep_8hz(void);

/* PWMB任务函数 */
portTASK_FUNCTION( vPWMA_beepTask, pvParameters )
{
    UNUSED( pvParameters );

		beep=off;

    while(1)
    {
			vTaskDelay(1);
		}
		
}

void beep_1hz(void)
{
	beep=on;
	vTaskDelay(500);
	beep=off;
	vTaskDelay(500);
}
void beep_4hz(void)
{
	beep=on;
	vTaskDelay(125);
	beep=off;
	vTaskDelay(125);
}
void beep_8hz(void)
{
	beep=on;
	vTaskDelay(62);
	beep=off;
	vTaskDelay(63);
}

//void beep_output(void)
//{
//	uchar time;
//	if(music[ptr]!=0xff&&music[ptr]!=0x00)
//	{
//		 TR1=0;
//		 beep=1;
//		 vTaskDelay(10);
//		 high=music[ptr];
//		 low= music[ptr+1];
//		 time=music[ptr+2];
//		 vTaskDelay(time);
//		 ptr+=3;
//	}
//	else if(music[ptr]==0xff)
//	{
//		time=music[ptr+2];
//		vTaskDelay(time);
//		ptr+=3;
//	}
//	else
//	{
//		TR1=0;
//		beep=1;
//		vTaskDelay(2000);
//		ptr=0;
//	}
//}

//void PWMA_Output_init(void)
//{
//	Init();
//   TH1=high;
//   TL1=low;
//}

//void Count1(void) interrupt 3
//{
//  TH1=high;
//  TL1=low;
//  if(flag3==0)
//  {
//     beep=0;
//     flag3=1;
//  }
//  else
//  {
//     beep=1;
//     flag3=0;
//  }
//}
//void Init()
//{
//   TMOD=0x01;
//   EA=1;
//   ET1=1;
//}


