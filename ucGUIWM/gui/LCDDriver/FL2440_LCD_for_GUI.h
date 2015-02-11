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

void Lcd_Init(void);
void Lcd_EnvidOnOff(int onoff);
void Lcd_PowerEnable(int invpwren,int pwren);
void PutPixel(U32 x,U32 y, U16 c );
void Lcd_ClearScr( U16 c);