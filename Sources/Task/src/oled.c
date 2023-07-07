#include "FreeRTOS.h"
#include "task.h"

#include "oled.h"
#include "font.h"
#include "stc.h"
#include "spi.h"
#include "string.h"
#include "pic.h"
#include "DS18B20.h"
#include "Power_detection.h"
#include "Infrared.h"
#include "beep.h"
#include "general.h"
//#include "STC32G_SPI.h"

#define  DELAY_TIME   2000
#define normal_state 0
#define alarm_state  1 //����״̬
#define manage_state 2 //����״̬

unsigned char xdata ShowBUFF[8][128];                           //OLEDȫ�ֻ���


portTASK_FUNCTION( vOLEDTask, pvParameters )
{
	float i;
	float V;
	UNUSED( pvParameters );
  EA = 1;                             //�����ж�
	OLED_Init();                        //OLED��ʼ��
    while(1)
    {
			i=((temp % 1000) / 100*10)+((temp % 100) / 10)+(temp % 10)*0.1;
			V=(float)adcResult/819.0;
//-----------------------------------------״̬��ʾ-----------------------------------------   
        OLED_BuffClear();                                                       //���ȫ������
        OLED_BuffShowString(32,0,"״̬��ʾ",0);				//��ʾ����
        OLED_BuffShowString(0,2," ",1);
				OLED_BuffShowBigString(8,2,"�����Ǿ����ʶ",1);                                       //��ʾ����
        OLED_BuffShowString(120,2," ",1);
				OLED_BuffShow68String(2,0,4,"Temperature=",0);
				OLED_BuffShow68Num02F(2,72,4,i,0);
				OLED_BuffShow68String(2,96,4,"C",0);
				OLED_BuffShow68String(1,0,4,"Voltage=",0);
				OLED_BuffShow68Num02F(1,48,4,V,0);                                         //��ʾ����
        OLED_BuffShow68String(1,66,4,"V",0);
				if(general_mode==alarm_state)
				{
					if(flag)
					{
						OLED_BuffShowString(32,6,"��ѹ�쳣",0);
						beep_1hz();
					}
					if(flag1)
					{
						OLED_BuffShowString(32,6,"���˾���\xfd",0);
						beep_4hz();
	//					beep_output();
					}
					if(flag2)
					{
						OLED_BuffShowString(32,6,"�¶��쳣",0);
						beep_8hz();
					}
				}
				OLED_BuffShow();                                                        //������д����ʾ����ʾ
        //vTaskDelay(DELAY_TIME/5);//delay_ms1(DELAY_TIME); 
        vTaskDelay(50);
    }
    
}

//========================================================================
// ��������: delay
// ��������: OLED�����õ���ʱ
// ��ڲ���: ��
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void delay(void)
{
    int xdata i;
    
    for (i=0; i<100; i++);
}

//========================================================================
// ��������: delay_ms1
// ��������: ms��ʱ����
// ��ڲ���: ��
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void delay_ms1(u16 ms)
{
     u16 xdata i;
     do{
          i = MAIN_Fosc1 / 6000;
          while(--i);   //6T per loop
     }while(--ms);
}

//========================================================================
// ��������: OLED_WR_Byte
// ��������: OLED��ַ/����д�뺯��
// ��ڲ���: @dat:����    @cmd:����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_WR_Byte(u8 dat,u8 cmd)
{
    u8 udat[1] ;
    udat[0] = dat;
    if(cmd)
    {
        OLED_DC = 1;
    }
    else
    {
        OLED_DC = 0;
    }
    OLED_CS = 0;
    SPI_WriteByte1(dat);
    delay();
    OLED_CS = 1;
    OLED_DC = 1;
}

//========================================================================
// ��������: OLED_Init
// ��������: OLED��ʼ������
// ��ڲ���: ��
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_Init(void)
{
    SPI_init1();
    vTaskDelay(100);//vTaskDelay(100);
    OLED_REST = 1;
    vTaskDelay(200);//vTaskDelay(200);
    OLED_REST=0;//��λ
    vTaskDelay(200);//vTaskDelay(200);
    OLED_REST = 1;

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x00,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
    OLED_WR_Byte(0xAF,OLED_CMD);
    
    OLED_DisplayOn();
}


//========================================================================
// ��������: OLED_Set_Pos
// ��������: OLED������ʾλ��
// ��ڲ���: @x:x����     @y:y����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte((u8)(0xb0+y),OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   


//========================================================================
// ��������: OLED_DrawBMP
// ��������: OLED��ʾͼƬ
// ��ڲ���: @x0:x���  @y0:y���    @x1:x�յ�   @y1:y�յ�  @BMP����ʾ����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
    unsigned int xdata j=0;
    unsigned char xdata x,y;

    for(y=y0;y<(y1+y0);y++)
    {
        OLED_Set_Pos(x0,y);
        for(x=0;x<x1;x++)
        {      
            OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
        }
    }
} 

//========================================================================
// ��������: OLED_DisplayOn
// ��������: OLED����ʾ
// ��ڲ���: ��
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_DisplayOn(void)     //����ʾ
{
    OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
    OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ       
}
  
//========================================================================
// ��������: OLED_DisplayOff
// ��������: OLED����ʾ
// ��ڲ���: ��
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_DisplayOff(void)     //����ʾ
{
    OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
    OLED_WR_Byte(0xAF,OLED_CMD);//�ر���Ļ        
}

//========================================================================
// ��������: OLED_LightSet
// ��������: OLED��������
// ��ڲ���: @num�� 0-255
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_LightSet(unsigned char num)     //��������
{
    OLED_WR_Byte(0x81,OLED_CMD);//
    OLED_WR_Byte(num,OLED_CMD);//  
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x20,OLED_CMD);//Set VCOM Deselect Level   
}


//========================================================================
// ��������: OLED_BuffClear
// ��������: OLED��ջ���
// ��ڲ���: ��
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//========================================================================
void OLED_BuffClear(void)     //����
{
    memset(ShowBUFF,0,128*8);
}

//========================================================================
// ��������: OLED_BuffShow
// ��������: OLEDˢ����ʾ
// ��ڲ���: ��
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_BuffShow(void)     
{
    u8 xdata i,n;

    for(i=0;i<8;i++)
    {
       OLED_WR_Byte(0xb0+i,OLED_CMD); //��������ʼ��ַ
       OLED_WR_Byte(0x00,OLED_CMD);   //���õ�����ʼ��ַ
       OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ

       for(n=0;n<128;n++)
         OLED_WR_Byte(ShowBUFF[i][n],OLED_DATA);
    }
}

//========================================================================
// ��������: OLED_BuffShowPoint
// ��������: OLED��ʾһ����
// ��ڲ���: @x��x���   @y:y���
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//========================================================================
void OLED_BuffShowPoint(unsigned char x,unsigned char y)     //OLED��ʾһ����
{
    ShowBUFF[y/8][x] |= 1<<(y%8);
}

//========================================================================
// ��������: OLED_BuffShowPoint
// ��������: OLED��ʾһ����
// ��ڲ���: @x1��x���  @y1��y���  @x2��x�յ�   @y2��y�յ�
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//========================================================================
void OLED_BuffShowLine( unsigned char x1, unsigned char y1, unsigned char x2,unsigned char y2)
{
    unsigned char x,y;
    if( x1>x2 )
    {
        x=x1;x1=x2;x2=x;
        y=y1;y1=y2;y2=y;
    }
    if(x1!=x2)
    {
        for( x = x1; x <= x2; x++ )
        {
            if( y2>y1 )
                OLED_BuffShowPoint(x, (unsigned char)(y1+(u16)(y2-y1)*(u16)x/(u16)(x2-x1)));
            else
                OLED_BuffShowPoint(x, (unsigned char)(y1-(u16)(y1-y2)*(u16)x/(u16)(x2-x1)));
        }        
    }
    else
    {
        if( y1>y2 )
        {
            for( y = y2; y <= y1; y++ )
               OLED_BuffShowPoint(x1, y); 
        }
        else
        {
            for( y = y1; y <= y2; y++ )
               OLED_BuffShowPoint(x1, y);             
        }
    }
}

//========================================================================
// ��������: OLED_BuffShowRectangle
// ��������: OLED��ʾһ������
// ��ڲ���: @x1��x���  @y1��y���  @x2��x�յ�   @y2��y�յ�
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//========================================================================
void OLED_BuffShowRectangle(u8 x1, u8 y1, u8 x2, u8 y2)
{
    OLED_BuffShowLine(x1, y1, x2, y1);
    OLED_BuffShowLine(x1, y1, x1, y2);
    OLED_BuffShowLine(x1, y2, x2, y2);
    OLED_BuffShowLine(x2, y1, x2, y2);
}

//========================================================================
// ��������: OLED_BuffShowCircle
// ��������: OLED��ʾһ��Բ��
// ��ڲ���: @x��x��  @y��y��  @r:�뾶
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//========================================================================
void OLED_BuffShowCircle(u8 x, u8 y, u8 r)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);       //�ж��¸���λ�õı�־
 
    while (a <= b)
    {
        OLED_BuffShowPoint(x + a, y - b);        //5
        OLED_BuffShowPoint(x + b, y - a);        //0
        OLED_BuffShowPoint(x + b, y + a);        //4
        OLED_BuffShowPoint(x + a, y + b);        //6
        OLED_BuffShowPoint(x - a, y + b);        //1
        OLED_BuffShowPoint(x - b, y + a);
        OLED_BuffShowPoint(x - a, y - b);        //2
        OLED_BuffShowPoint(x - b, y - a);        //7
        a++;
 
        //ʹ��Bresenham�㷨��Բ
        if (di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

//========================================================================
// ��������: OLED_BuffShowChar
// ��������: OLED��ʾһ��8*16���ַ�
// ��ڲ���: @x��x��  @y��y��  @asc:�ַ�   @mode��0����  1����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//========================================================================
void OLED_BuffShowChar(unsigned char x,unsigned char y,char asc, u8 mode)     //OLED��ʾһ��8*16�ַ�
{
    unsigned char j,k;
    for( j=0;j<2;j++ )
    {
        for( k=0;k<8;k++ )
        {
            if(mode==0)            
                ShowBUFF[j+y][x+k]=Ascll_16[(asc-' ')*2][j*8+k];
            else
                ShowBUFF[j+y][x+k]=~Ascll_16[(asc-' ')*2][j*8+k];
        }
    }    
}
void OLED_BuffShowChar68(unsigned char where,unsigned char x,unsigned char y,char asc, u8 mode)     //OLED��ʾһ��6*8�ַ�
{
    unsigned char j,k,n;
		if(where==1)
		{j=0;n=1;}
		else if(where==2)
		{j=1;n=2;}
    for( ;j<n;j++ )
    {
        for( k=0;k<6;k++ )
        {
            if(mode==0)            
                ShowBUFF[j+y][x+k]=F6x8[(asc-' ')][k];
            else
                ShowBUFF[j+y][x+k]=~F6x8[(asc-' ')][k];
        }
    }    
}

void OLED_BuffShow68String(unsigned char where,unsigned char x,unsigned char y,char *s,u8 mode)
{
    char s_num[2];
	while(*s != '\0')       //�ַ�����Ϊ�գ�ѭ��
	{
		OLED_BuffShowChar68(where,x,y,*s,mode);
		x+=6;
		s++;
		if(x>127)
		{
				x=0;
				y+=2;
		}
	}       
}

void OLED_BuffShow68Num02F(unsigned char where,u8 x,u8 y,float num,u8 mode)
{
    u8 xdata str[10];
    memset(str,0,10);
    sprintf(str,"%.1f",num);
    OLED_BuffShow68String(where,x,y,(u8*)str,mode);
}
//========================================================================
// ��������: OLED_BuffShowGBK
// ��������: OLED��ʾ����
// ��ڲ���: @x��x��  @y��y��  @gbk:����   @mode��0����  1����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//          �õ��ĺ�����Ҫ�Լ�ȡģ�ŵ��ֿ���ֿ��Ļ����������ѭ������Ҫ�Ŵ�
//========================================================================
void OLED_BuffShowGBK(unsigned char x,unsigned char y,char *gbk,u8 mode)     //OLED��ʾһ��16*16����
{
    unsigned char i;
    unsigned char j,k;
    for( i=0;i<200;i++ )
    {
        if(( gbk[0]== GBK16[i].gbn_bum[0] )&&( gbk[1]== GBK16[i].gbn_bum[1] ))
        {
            for( j=0;j<2;j++ )
            {
                for( k=0;k<16;k++ )
                {
                    if( mode ==0 )
                        ShowBUFF[j+y][x+k]=GBK16[i].gbk_font[j*16+k];
                    else
                        ShowBUFF[j+y][x+k]=~GBK16[i].gbk_font[j*16+k];
                }
            }
            break;
        }
    }    
}

//========================================================================
// ��������: OLED_BuffShowString
// ��������: OLED��ʾ�ַ����������ַ�����
// ��ڲ���: @x��x��  @y��y��  @s�ַ���   @mode��0����  1����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//          �õ��ĺ�����Ҫ�Լ�ȡģ�ŵ��ֿ���ֿ��Ļ����������ѭ������Ҫ�Ŵ�
//========================================================================

void OLED_BuffShowBigString(unsigned char x,unsigned char y,char *s,u8 mode)
{
	char s_num[2];
	while(*s != '\0')       //�ַ�����Ϊ�գ�ѭ��
	{
		s_num[0] = *s ;
		s_num[1] = *(s+1) ;
		OLED_BuffShowGBK(x,y,s_num,mode);
		x+=16;
		s+=2;
		if(x>127)
		{
				x=0;
				y+=2;
		}
	}
}

void OLED_BuffShowString(unsigned char x,unsigned char y,char *s,u8 mode)
{
    char s_num[2];
	while(*s != '\0')       //�ַ�����Ϊ�գ�ѭ��
	{
        if ((unsigned char)*s < 0x80)     //�����������ݵĴ�С�ж����ַ����Ǻ��֣�
        {
            OLED_BuffShowChar(x,y,*s,mode);
            x+=8;
            s++;
        }
        else
        {
            s_num[0] = *s ;
            s_num[1] = *(s+1) ;
            OLED_BuffShowGBK(x,y,s_num,mode);
            x+=16;
            s+=2;
        }
		if(x>127)
        {
            x=0;
            y+=2;
        }
	}       
}

//========================================================================
// ��������: OLED_BuffShowNum
// ��������: OLED��ʾ���α���
// ��ڲ���: @x��x��  @y��y��  @num�����α���   @mode��0����  1����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//========================================================================
void OLED_BuffShowNum(u8 x,u8 y,long num,u8 mode)
{
    u8 xdata str[10];
    memset(str,0,10);
    sprintf(str,"%ld",num);
    OLED_BuffShowString(x,y,(u8*)str);
}

//========================================================================
// ��������: OLED_BuffShowNum02F
// ��������: OLED��ʾһ����λС������
// ��ڲ���: @x��x��  @y��y��  @num��С������   @mode��0����  1����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//========================================================================
void OLED_BuffShowNum02F(u8 x,u8 y,float num,u8 mode)
{
    u8 xdata str[10];
    memset(str,0,10);
    sprintf(str,"%.2f",num);
    OLED_BuffShowString(x,y,(u8*)str,mode);
}

//========================================================================
// ��������: OLED_BuffShowChar32
// ��������: OLED��ʾ16*32���ַ�
// ��ڲ���: @x��x��  @y��y��  @num����ʾ�ڼ����ַ�   @mode��0����  1����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע: д����ǻ��棬��Ҫ�����OLED_BuffShow������ʾ����
//          �õ�����Ҫ�Լ�ȡģŶ
//========================================================================
void OLED_BuffShowChar32(unsigned char  x,unsigned char  y,u8 num, u8 mode)     
{
    unsigned char j,k;
    for( j=0;j<4;j++ )
    {
        for( k=0;k<16;k++ )
        {
            if( mode==0 )
                ShowBUFF[j+y][x+k]=lib_num1632[num][j*16+k];
            else
                ShowBUFF[j+y][x+k]=~lib_num1632[num][j*16+k];
        }
    }            
}


//========================================================================
// ��������: OLED_BuffShowBMP
// ��������: OLED����д��ͼƬ
// ��ڲ���: @x0:x���  @y0:y���    @x1:x�յ�   @y1:y�յ�  @BMP����ʾ����
// ��������: ��
// ��ǰ�汾: VER1.0
// �޸�����: 2023 - 6 - 7
// ��ǰ����: ZCF
// ������ע:
//========================================================================
void OLED_BuffShowBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
    unsigned int xdata num=0;
    u8 k,j;
    
    for( j=0;j<y1;j++ )
    {
        for( k=0;k<x1;k++ )
        {
            ShowBUFF[j+y0][x0+k]=BMP[num++];
        }
    }  
} 
 