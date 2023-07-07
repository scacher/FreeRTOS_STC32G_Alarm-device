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
/*************  ���غ�������    **************/
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
//	  WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
//    EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
//    CKCON = 0; //��߷���XRAM�ٶ�
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
	
    DS18B20_Reset();                //�豸��λ
    DS18B20_WriteByte(0xCC);        //����ROM����
    DS18B20_WriteByte(0x44);        //��ʼת������
    while (!DQ);                    //�ȴ�ת�����

    DS18B20_Reset();                //�豸��λ
    DS18B20_WriteByte(0xCC);        //����ROM����
    DS18B20_WriteByte(0xBE);        //���ݴ�洢������
    TempL = DS18B20_ReadByte();     //���¶ȵ��ֽ�
    TempH = DS18B20_ReadByte();     //���¶ȸ��ֽ�
	
    if(TempH & 0xf8)    //�ж��Ƿ�λ����
    {
        MinusFlag = 1;  //���ø�����־
        Temperature = (TempH<<8) | TempL;
        Temperature = ~Temperature + 1;
        Temperature *= 0.625;       //0.0625 * 10������1λС����
    }
    else
    {
        MinusFlag = 0;  //���������־
        Temperature = (((TempH<<8) | TempL) * 0.625); //0.0625 * 10������1λС����
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
        DQ = 0;                     //�ͳ��͵�ƽ��λ�ź�
        delay_us(240);              //��ʱ����480us
        delay_us(240);
        DQ = 1;                     //�ͷ�������
        delay_us(60);               //�ȴ�60us
        CY = DQ;                    //����������
        delay_us(240);              //�ȴ��豸�ͷ�������
        delay_us(180);
    }
}

u8 DS18B20_ReadByte()
{
    u8 i;
    u8 dat = 0;

    for (i=0; i<8; i++)             //8λ������
    {
        dat >>= 1;
        DQ = 0;                     //��ʼʱ��Ƭ
        delay_us(1);                //��ʱ�ȴ�
        DQ = 1;                     //׼������
        delay_us(1);                //������ʱ
        if (DQ) dat |= 0x80;        //��ȡ����
        delay_us(60);               //�ȴ�ʱ��Ƭ����
    }

    return dat;
}

void DS18B20_WriteByte(u8 dat)
{
    char i;

    for (i=0; i<8; i++)             //8λ������
    {
        DQ = 0;                     //��ʼʱ��Ƭ
        delay_us(1);                //��ʱ�ȴ�
        dat >>= 1;                  //�ͳ�����
        DQ = CY;
        delay_us(60);               //�ȴ�ʱ��Ƭ����
        DQ = 1;                     //�ָ�������
        delay_us(1);                //�ָ���ʱ
    }
}

