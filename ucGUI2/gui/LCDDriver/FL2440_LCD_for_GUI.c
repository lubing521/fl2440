#define LCD_XSIZE_TFT_240320 	(320)	
#define LCD_YSIZE_TFT_240320 	(240)

#define SCR_XSIZE_TFT_240320 	(320)
#define SCR_YSIZE_TFT_240320 	(240)

#define MVAL		(13)
#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define INVVDEN		(1)		//0=normal       1=inverted
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control

#define M5D(n) ((n) & 0x1fffff)	// To get lower 21bits

//TFT 240320
#define LCD_XSIZE_TFT_240320 	(320)	
#define LCD_YSIZE_TFT_240320 	(240)

#define SCR_XSIZE_TFT_240320 	(320)
#define SCR_YSIZE_TFT_240320 	(240)

//TFT240320
#define HOZVAL_TFT_240320	(LCD_XSIZE_TFT_240320-1)
#define LINEVAL_TFT_240320	(LCD_YSIZE_TFT_240320-1)

//Timing parameter for LCD LQ035Q7DB02
#define VBPD_240320		(3)		//垂直同步信号的后肩1
#define VFPD_240320		(5)		//垂直同步信号的前肩5
#define VSPW_240320		(15)		//垂直同步信号的脉宽1

#define HBPD_240320		(58)		//水平同步信号的后肩35
#define HFPD_240320		(15)		//水平同步信号的前肩19
#define HSPW_240320		(8)		//水平同步信号的脉宽5

#define CLKVAL_TFT_240320	(4) 	
//FCLK=180MHz,HCLK=90MHz,VCLK=6.5MHz

#include "2440addr.h"


unsigned short LCD_BUFFER[SCR_YSIZE_TFT_240320][SCR_XSIZE_TFT_240320];//240*320

void Lcd_Init(void)
{
    rGPCUP  = 0x00000000;
	 rGPCCON = 0xaaaa02a9; 
	 
//    rGPDUP=0xffffffff; // Disable Pull-up register
     rGPDUP  = 0x00000000;
   rGPDCON=0xaaaaaaaa; //Initialize VD[15:8]

	rLCDCON1=(CLKVAL_TFT_240320<<8)|(MVAL_USED<<7)|(3<<5)|(12<<1)|0;
    	// TFT LCD panel,16bpp TFT,ENVID=off
	rLCDCON2=(VBPD_240320<<24)|(LINEVAL_TFT_240320<<14)|(VFPD_240320<<6)|(VSPW_240320);
	rLCDCON3=(HBPD_240320<<19)|(HOZVAL_TFT_240320<<8)|(HFPD_240320);
	rLCDCON4=(MVAL<<8)|(HSPW_240320);
    rLCDCON5 = (1<<11) | (1<<10) | (1<<9) | (1<<8) | (0<<7) | (0<<6)
             | (1<<3)  |(BSWP<<1) | (HWSWP);
             //5:6:5 VCLK posedge BSWP=0,HWSWP=1;

	rLCDSADDR1=(((U32)LCD_BUFFER>>22)<<21)|M5D((U32)LCD_BUFFER>>1);
	rLCDSADDR2=M5D( ((U32)LCD_BUFFER+(SCR_XSIZE_TFT_240320*LCD_YSIZE_TFT_240320*2))>>1 );
	rLCDSADDR3=(((SCR_XSIZE_TFT_240320-LCD_XSIZE_TFT_240320)/1)<<11)|(LCD_XSIZE_TFT_240320/1);
	rLCDINTMSK|=(3); // MASK LCD Sub Interrupt
    rTCONSEL &= (~7) ;     // Disable LPC3480
	rTPAL=0; // Disable Temp Palette
}

/**************************************************************
LCD视频和控制信号输出或者停止，1开启视频输出
**************************************************************/
static void Lcd_EnvidOnOff(int onoff)
{
    if(onoff==1)
	rLCDCON1|=1; // ENVID=ON
    else
	rLCDCON1 =rLCDCON1 & 0x3fffe; // ENVID Off
}

/**************************************************************
320×240 16Bpp TFT LCD 电源控制引脚使能
**************************************************************/
static void Lcd_PowerEnable(int invpwren,int pwren)
{
    //GPG4 is setted as LCD_PWREN
    rGPGUP=rGPGUP&(~(1<<4))|(1<<4); // Pull-up disable
    rGPGCON=rGPGCON&(~(3<<8))|(3<<8); //GPG4=LCD_PWREN
    rGPGDAT = rGPGDAT | (1<<4) ;
//	invpwren=pwren;
    //Enable LCD POWER ENABLE Function
    rLCDCON5=rLCDCON5&(~(1<<3))|(pwren<<3);   // PWREN
    rLCDCON5=rLCDCON5&(~(1<<5))|(invpwren<<5);   // INVPWREN
}

/**************************************************************
320×240 16Bpp TFT LCD单个象素的显示数据输出
**************************************************************/
static void PutPixel(U32 x,U32 y, U16 c )
{
	if ( (x < SCR_XSIZE_TFT_240320) && (y < SCR_YSIZE_TFT_240320) )
		LCD_BUFFER[(y)][(x)] = c;
}
 
static U16 GUI_Point(U32 x,U32 y)
{
	U16 c;
	if ( (x < SCR_XSIZE_TFT_240320) && (y < SCR_YSIZE_TFT_240320) )
		c=LCD_BUFFER[(y)][(x)];
	return c;
}

/**************************************************************
320×240 16Bpp TFT LCD全屏填充特定颜色单元或清屏
**************************************************************/
static void Lcd_ClearScr( U16 c)
{
	unsigned int x,y ;
		
    for( y = 0 ; y < SCR_YSIZE_TFT_240320 ; y++ )
    {
    	for( x = 0 ; x < SCR_XSIZE_TFT_240320 ; x++ )
    	{
			LCD_BUFFER[y][x] = c ;
    	}
    }
}

void FL_Lcd_Init(void)
{
	Lcd_Init();
	Lcd_PowerEnable(0,1);
 	Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0x1111);
}

void FL2440_LCD_SetPixel(U32 x,U32 y,U16 c)
{
	PutPixel(x,y,c);
}

U16 FL2440_LCD_GetPixel(U32 x,U32 y)
{
	return (GUI_Point(x,y));
}

void FL2440_LCD_Off(void)
{
	Lcd_EnvidOnOff(0);
}

void FL2440_LCD_On(void)
{
	Lcd_EnvidOnOff(1);
}
		