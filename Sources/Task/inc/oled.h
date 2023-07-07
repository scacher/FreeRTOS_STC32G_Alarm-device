#ifndef __OLED_H
#define __OLED_H

#include "stc.h"
#include "spi.h"


//-----------------------------------------����궨��-----------------------------------------
#define OLED_CMD  0	                                        //д����
#define OLED_DATA 1	                                        //д����

#define SIZE 12
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

//------------------------------------------����ѡ��------------------------------------------
sbit    OLED_CS     = P4^2;                                 //ʹ�ܽ� 
sbit    OLED_DC     = P3^4;                                 //����/��ַѡ��
sbit    OLED_REST   = P2^4;                                 //��λ��

//------------------------------------------��������------------------------------------------
extern  unsigned char xdata ShowBUFF[8][128];               //OLEDȫ�ֻ���


//------------------------------------------��������------------------------------------------
portTASK_FUNCTION( vOLEDTask, pvParameters );
void delay_ms1(u16 ms);
void OLED_WR_Byte(u8 dat,u8 cmd);                           //OLED��ַ/����д�뺯��    
void OLED_Init(void);                                       //OLED��ʼ������
void OLED_Set_Pos(unsigned char x, unsigned char y);        //OLED������ʾλ��
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1, u8 y1,u8 BMP[]);       //OLED��ʾͼƬ
void OLED_DisplayOn(void);                                  //����ʾ
void OLED_DisplayOff(void);                                 //����ʾ

void OLED_BuffClear(void);                                  //����
void OLED_BuffShow(void);                                   //OLEDˢ����ʾ
void OLED_BuffShowPoint(unsigned char x,unsigned char y);   //OLED��ʾһ����
void OLED_BuffShowLine( u8 x1, u8 y1, u8 x2,u8 y2);         //OLED��ʾһ����
void OLED_BuffShowRectangle(u8 x1, u8 y1, u8 x2, u8 y2);    //OLED��ʾһ������
void OLED_BuffShowCircle(u8 x, u8 y, u8 r);                 //OLED��ʾһ��Բ��
void OLED_BuffShowChar68(u8 where,u8 x,u8 y,char asc, u8 mode);			//OLED��ʾһ��6*8�ַ�
void OLED_BuffShow68String(u8 where,u8 x,u8 y,char *s,u8 mode);//OLED��ʾһ��6*8�ַ���
void OLED_BuffShow68Num02F(u8 where,u8 x,u8 y,float num,u8 mode);
void OLED_BuffShowChar(u8 x,u8 y,char asc, u8 mode);        //OLED��ʾһ��8*16�ַ�
void OLED_BuffShowGBK(u8 x,u8 y,char *gbk,u8 mode);         //OLED��ʾһ��16*16����
void OLED_BuffShowBigString(u8 x,u8 y,char *s,u8 mode);
void OLED_BuffShowString(u8 x,u8 y,char *s,u8 mode);        //OLED��ʾһ��16���صĺ��ֺ��ַ�
void OLED_BuffShowNum(u8 x,u8 y,long num,u8 mode);          //OLED��ʾһ�����α���
void OLED_BuffShowNum02F(u8 x,u8 y,float num,u8 mode);      //OLED��ʾһ����λС������
void OLED_BuffShowChar32(u8  x,u8  y,u8 num, u8 mode) ;     //OLED��ʾһ��16*32���ַ�   
void OLED_BuffShowBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);  //OLED��ʾһ��ͼƬ 




#endif



