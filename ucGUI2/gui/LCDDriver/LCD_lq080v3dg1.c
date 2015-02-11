/**************************************************************
The initial and control for 640��480 16Bpp TFT LCD----V16C6448AC
**************************************************************/
/*
*********************************************************
* Copyright (c)
* All rights reserved.				            
*
* �ļ����ƣ�LCD_lq080v3dg1.c
* �ļ���ʶ��
* ժ    Ҫ�����ļ��Ƕ�8��TFT���г�ʼ���Ͳ�����
* ��ǰ�汾��1.0
* ��    �ߣ�����
* ������ڣ�2005.4.3
*
* ȡ���汾��
* ��    �ߣ�
* ������ڣ�
*********************************************************
*/

/*
*********************************************************
*   					 ͷ�ļ�
*********************************************************
*/
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 

/*
*********************************************************
*   					 �궨��
*********************************************************
*/
#define CLKVAL_TFT_800480	(0)		
//����8��TFTҺ����LQ080V3DG1

#define MVAL		(13)
#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control

#define M5D(n) ((n) & 0x1fffff)	// To get lower 21bits

//TFT 800480
#define LCD_XSIZE_TFT_800480 	(800)	
#define LCD_YSIZE_TFT_800480 	(480)

//TFT 800480
//#define SCR_XSIZE_TFT_800480 	(640)
//#define SCR_YSIZE_TFT_800480 	(480)
#define SCR_XSIZE_TFT_800480 	(800)
#define SCR_YSIZE_TFT_800480 	(480)

//TFT800480
#define HOZVAL_TFT_800480	(LCD_XSIZE_TFT_800480-1)
#define LINEVAL_TFT_800480	(LCD_YSIZE_TFT_800480-1)

//���²���ֻ���Ԫ̫6.4��TFTҺ����V16C6448AC
#define VGA350_MODE		0
#define FREEDOM_MODE    0

//���²���ֻ�������8��TFTҺ����LQ080V3DG1
#define LQ080_MODE      1

#if VGA350_MODE
	#define VBPD_800480		(33)	//��ֱͬ���źŵĺ��		VGA350ģʽ�����Ǹ�����
	#define VFPD_800480		(10)		//��ֱͬ���źŵ�ǰ��
	#define VSPW_800480		(2)		//��ֱͬ���źŵ�����

	#define HBPD_800480		(48)		//ˮƽͬ���źŵĺ��
	#define HFPD_800480		(16)		//ˮƽͬ���źŵ�ǰ��
	#define HSPW_800480		(96)		//ˮƽͬ���źŵ�����
#elif   FREEDOM_MODE
	#define VBPD_800480		(20)	//��ֱͬ���źŵĺ��		FREEDOMģʽ������������
	#define VFPD_800480		(16)		//��ֱͬ���źŵ�ǰ��
	#define VSPW_800480		(2)		//��ֱͬ���źŵ�����

	#define HBPD_800480		(48)		//ˮƽͬ���źŵĺ��
	#define HFPD_800480		(16)		//ˮƽͬ���źŵ�ǰ��
	#define HSPW_800480		(96)		//ˮƽͬ���źŵ�����
#elif   LQ080_MODE
	#define VBPD_800480		((29)&0xff)//1//
	#define VFPD_800480		((13)&0xff)//2//
	#define VSPW_800480		((3) &0x3f)//1//
	#define HBPD_800480		((40)&0x7f)//6//
	#define HFPD_800480		((40)&0xff)//2//
	#define HSPW_800480		((48)&0xff)//4//
#endif

/*
*********************************************************
*   					 ����
*********************************************************
*/


volatile static unsigned short LCD_BUFER[SCR_YSIZE_TFT_800480][SCR_XSIZE_TFT_800480];

/*
*********************************************************
*   					 ����
*********************************************************
*/
/**************************************************************
640��480 16Bpp TFT LCD����ģ���ʼ��
**************************************************************/
void Lcd_Init(void)
{
    rGPCUP=0xffffffff; // Disable Pull-up register
    rGPCCON=0xaaaa56a9; //Initialize VD[7:0],LCDVF[2:0],VM,VFRAME,VLINE,VCLK,LEND 

    rGPDUP=0xffffffff; // Disable Pull-up register
    rGPDCON=0xaaaaaaaa; //Initialize VD[15:8]

	rLCDCON1=(CLKVAL_TFT_800480<<8)|(MVAL_USED<<7)|(3<<5)|(12<<1)|0;
    	// TFT LCD panel,12bpp TFT,ENVID=off
	rLCDCON2=(VBPD_800480<<24)|(LINEVAL_TFT_800480<<14)|(VFPD_800480<<6)|(VSPW_800480);
	rLCDCON3=(HBPD_800480<<19)|(HOZVAL_TFT_800480<<8)|(HFPD_800480);
	rLCDCON4=(MVAL<<8)|(HSPW_800480);
	rLCDCON5=(1<<11)|(0<<9)|(0<<8)|(0<<6)|(BSWP<<1)|(HWSWP);	//FRM5:6:5,HSYNC and VSYNC are inverted

	rLCDSADDR1=(((U32)LCD_BUFER>>22)<<21)|M5D((U32)LCD_BUFER>>1);
	rLCDSADDR2=M5D( ((U32)LCD_BUFER+(SCR_XSIZE_TFT_800480*LCD_YSIZE_TFT_800480*2))>>1 );
	rLCDSADDR3=(((SCR_XSIZE_TFT_800480-LCD_XSIZE_TFT_800480)/1)<<11)|(LCD_XSIZE_TFT_800480/1);
	rLCDINTMSK|=(3); // MASK LCD Sub Interrupt
    rTCONSEL&=~((1<<4)|1); // Disable LCC3600, LPC3600
	rTPAL=0; // Disable Temp Palette
}

/**************************************************************
LCD��Ƶ�Ϳ����ź��������ֹͣ��1������Ƶ���
**************************************************************/
void Lcd_EnvidOnOff(int onoff)
{
    if(onoff==1)
	rLCDCON1|=1; // ENVID=ON
    else
	rLCDCON1 =rLCDCON1 & 0x3fffe; // ENVID Off
}

/**************************************************************
640��480 8Bpp TFT LCD ��Դ��������ʹ��
**************************************************************/
void Lcd_PowerEnable(int invpwren,int pwren)
{
    //GPG4 is setted as LCD_PWREN
    rGPGUP=rGPGUP&(~(1<<4))|(1<<4); // Pull-up disable
    rGPGCON=rGPGCON&(~(3<<8))|(3<<8); //GPG4=LCD_PWREN
    //Enable LCD POWER ENABLE Function
    rLCDCON5=rLCDCON5&(~(1<<3))|(pwren<<3);   // PWREN
    rLCDCON5=rLCDCON5&(~(1<<5))|(invpwren<<5);   // INVPWREN
}




/**************************************************************
640��480 16Bpp TFT LCD�������ص���ʾ�������
**************************************************************/
void PutPixel(U32 x,U32 y, U32 c )
{
	if ( (x < SCR_XSIZE_TFT_800480) && (y < SCR_YSIZE_TFT_800480) )
		LCD_BUFER[(y)][(x)] = c;
}

void GUI_Point(U32 x,U32 y, U32 c )
{
	if ( (x < SCR_XSIZE_TFT_800480) && (y < SCR_YSIZE_TFT_800480) )
		 c=LCD_BUFER[(y)][(x)];
}


/**************************************************************
640��480 16Bpp TFT LCDȫ������ض���ɫ��Ԫ������
**************************************************************/
void Lcd_ClearScr( U32 c)
{
	unsigned int x,y ;
		
    for( y = 0 ; y < SCR_YSIZE_TFT_800480 ; y++ )
    {
    	for( x = 0 ; x < SCR_XSIZE_TFT_800480 ; x++ )
    	{
			LCD_BUFER[y][x] = c ;
    	}
    }
}

