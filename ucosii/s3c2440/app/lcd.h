
#include "def.h"

#define LCD_XSIZE_TFT_240320 	(320)	
#define LCD_YSIZE_TFT_240320 	(240)


void Lcd_Init(void);
void Lcd_EnvidOnOff(int onoff); 
void Lcd_PowerEnable(int invpwren,int pwren);
void Lcd_ClearScr( U16 c);
void PutPixel(U32 x,U32 y, U16 c );
void GUI_Point(U32 x,U32 y, U16 c );
//void PutPixel(unsigned int x , unsigned int y , unsigned int c);
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color);
//void Glib_Rectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color);
//void Glib_FilledRectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Glib_Line(int x1,int y1,int x2,int y2,int color);
//void Glib_Line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Paint_Bmp(int x0,int y0,int h,int l,unsigned char bmp[]);

//================================
// �������
//================================
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st); 
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st);  
void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...); 



