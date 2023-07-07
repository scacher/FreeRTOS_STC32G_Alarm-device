#include "stc32g.h"
#include "FreeRTOS.h"
#include "task.h"
#include "DS18B20.h"
#include "display.h"
#include "STC32G_GPIO.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#define DQ  P45
#define		usrNOP()    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_()

u8 MinusFlag;
short temp;
u8 flag2;
/*************  本地函数声明    **************/
void DS18B20_Reset();
void DS18B20_WriteByte(u8 dat);
u8 DS18B20_ReadByte();

void delay_us(u8 us);
u16 ReadTemperature();
void DisplayTemperature(u16 temp);

portTASK_FUNCTION( vDS18B20Task, pvParameters)
{
	short temp1;
	UNUSED( pvParameters );
//    // ds1820 start convert
//	  WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
//    EAXFR = 1; //扩展寄存器(XFR)访问使能
//    CKCON = 0; //提高访问XRAM速度
    while(1)
    {
			temp1 = ReadTemperature();
			if(temp1)
				temp=temp1;
			if(temp>=300)
				flag2=1;
			else
				flag2=0;
			DisplayTemperature(temp);
			vTaskDelay(200);
    }
    vTaskDelete(NULL);
}

u16 ReadTemperature()
{
    u16 TempH, TempL, Temperature;
	
    DS18B20_Reset();                //设备复位
    DS18B20_WriteByte(0xCC);        //跳过ROM命令
    DS18B20_WriteByte(0x44);        //开始转换命令
    while (!DQ);                    //等待转换完成

    DS18B20_Reset();                //设备复位
    DS18B20_WriteByte(0xCC);        //跳过ROM命令
    DS18B20_WriteByte(0xBE);        //读暂存存储器命令
    TempL = DS18B20_ReadByte();     //读温度低字节
    TempH = DS18B20_ReadByte();     //读温度高字节
	
    if(TempH & 0xf8)    //判断是否位负数
    {
        MinusFlag = 1;  //设置负数标志
        Temperature = (TempH<<8) | TempL;
        Temperature = ~Temperature + 1;
        Temperature *= 0.625;       //0.0625 * 10，保留1位小数点
    }
    else
    {
        MinusFlag = 0;  //清除负数标志
        Temperature = (((TempH<<8) | TempL) * 0.625); //0.0625 * 10，保留1位小数点
    }

    return Temperature;
}

void DisplayTemperature(u16 temp)
{
//    if(MinusFlag)
//    {
//        if(temp > 999)
//        {
//            pucLEDBuffer[3] = DIS_;
//            pucLEDBuffer[4] = temp / 1000;
//        }
//        else
//        {
//            pucLEDBuffer[3] = DIS_BLACK;
//            pucLEDBuffer[4] = DIS_;
//        }
//    }
//    else
//    {
//        pucLEDBuffer[3] = DIS_BLACK;
//        if(temp > 999)
//        {
//            pucLEDBuffer[4] = temp / 1000;
//        }
//        else
//        {
//            pucLEDBuffer[4] = DIS_BLACK;
//        }
//    }
    pucLEDBuffer[1] = (temp % 1000) / 100;
    pucLEDBuffer[2] = ((temp % 100) / 10) + DIS_DOT;
    pucLEDBuffer[3] = temp % 10;
}

void delay_us(u8 us)
{
    do{
        usrNOP();
        usrNOP();
    }while(--us);
}

void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //送出低电平复位信号
        delay_us(240);              //延时至少480us
        delay_us(240);
        DQ = 1;                     //释放数据线
        delay_us(60);               //等待60us
        CY = DQ;                    //检测存在脉冲
        delay_us(240);              //等待设备释放数据线
        delay_us(180);
    }
}

u8 DS18B20_ReadByte()
{
    u8 i;
    u8 dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
        dat >>= 1;
        DQ = 0;                     //开始时间片
        delay_us(1);                //延时等待
        DQ = 1;                     //准备接收
        delay_us(1);                //接收延时
        if (DQ) dat |= 0x80;        //读取数据
        delay_us(60);               //等待时间片结束
    }

    return dat;
}

void DS18B20_WriteByte(u8 dat)
{
    char i;

    for (i=0; i<8; i++)             //8位计数器
    {
        DQ = 0;                     //开始时间片
        delay_us(1);                //延时等待
        dat >>= 1;                  //送出数据
        DQ = CY;
        delay_us(60);               //等待时间片结束
        DQ = 1;                     //恢复数据线
        delay_us(1);                //恢复延时
    }
}

