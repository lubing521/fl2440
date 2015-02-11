/**************************************************************
The initial and control for 640×480 16Bpp TFT LCD----V16C6448AC
**************************************************************/
/*
*********************************************************
* Copyright (c)
* All rights reserved.				            
*
* 文件名称：LCD_lq080v3dg1.c
* 文件标识：
* 摘    要：本文件是对8寸TFT进行初始化和操作。
* 当前版本：1.0
* 作    者：刘征
* 完成日期：2005.4.3
*
* 取代版本：
* 作    者：
* 完成日期：
*********************************************************
*/

/*
*********************************************************
*   					 头文件
*********************************************************
*/
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 

/*
*********************************************************
*   					 宏定义
*********************************************************
*/
#define CLKVAL_TFT_800480	(0)		
//夏普8寸TFT液晶屏LQ080V3DG1

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

//以下参数只针对元太6.4寸TFT液晶屏V16C6448AC
#define VGA350_MODE		0
#define FREEDOM_MODE    0

//以下参数只针对夏普8寸TFT液晶屏LQ080V3DG1
#define LQ080_MODE      1

#if VGA350_MODE
	#define VBPD_800480		(33)	//垂直同步信号的后肩		VGA350模式，都是负极性
	#define VFPD_800480		(10)		//垂直同步信号的前肩
	#define VSPW_800480		(2)		//垂直同步信号的脉宽

	#define HBPD_800480		(48)		//水平同步信号的后肩
	#define HFPD_800480		(16)		//水平同步信号的前肩
	#define HSPW_800480		(96)		//水平同步信号的脉宽
#elif   FREEDOM_MODE
	#define VBPD_800480		(20)	//垂直同步信号的后肩		FREEDOM模式，都是正极性
	#define VFPD_800480		(16)		//垂直同步信号的前肩
	#define VSPW_800480		(2)		//垂直同步信号的脉宽

	#define HBPD_800480		(48)		//水平同步信号的后肩
	#define HFPD_800480		(16)		//水平同步信号的前肩
	#define HSPW_800480		(96)		//水平同步信号的脉宽
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
*   					 变量
*********************************************************
*/


volatile static unsigned short LCD_BUFER[SCR_YSIZE_TFT_800480][SCR_XSIZE_TFT_800480];

/*
*********************************************************
*   					 函数
*********************************************************
*/
/**************************************************************
640×480 16Bpp TFT LCD功能模块初始化
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
LCD视频和控制信号输出或者停止，1开启视频输出
**************************************************************/
void Lcd_EnvidOnOff(int onoff)
{
    if(onoff==1)
	rLCDCON1|=1; // ENVID=ON
    else
	rLCDCON1 =rLCDCON1 & 0x3fffe; // ENVID Off
}

/**************************************************************
640×480 8Bpp TFT LCD 电源控制引脚使能
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
640×480 16Bpp TFT LCD单个象素的显示数据输出
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
640×480 16Bpp TFT LCD全屏填充特定颜色单元或清屏
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

