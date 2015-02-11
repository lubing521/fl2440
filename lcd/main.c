/*********************LED实验*********************************/
// 公司名称 ：保定飞凌嵌入式技术有限公司
// 描    述 ：跑马灯
//#define GPFCON (*(volatile unsigned *)0x56000050)
//#define GPFDAT (*(volatile unsigned *)0x56000054)
//#define GPFUP (*(volatile unsigned *)0x56000058)
#define GLOBAL_CLK 1
#include "lcd.h"

#define RGB(r,g,b)   (unsigned int)( (r << 16) + (g << 8) + b )
#define FROM_BCD(n)		((((n) >> 4) * 10) + ((n) & 0xf))
#define TO_BCD(n)		((((n) / 10) << 4) | ((n) % 10))

extern unsigned char ok2440[];
extern unsigned char TE2440[];
extern unsigned char TE2440_ii[];

unsigned char *bmp[3]=
{
	ok2440,
	TE2440,
	TE2440_ii
	};
	


int LedMain()
{
	
	void Delay(unsigned int x );
	int i;
	Lcd_Init();
	Lcd_PowerEnable(0,1);
	Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0x0);
	Delay(100);
	//Paint_Bmp(0,0,320,240,ok2440);
	while(1)
	{
		for(i=0;i<3;i++)
		{
			//Glib_Rectangle(i,i,i+10,i+10,0x11111);
			//Delay(100);
			//Glib_FilledRectangle(i,i,i+10,i+10,0x11111);
			//Delay(100);
			//Lcd_ClearScr(0xffff);
			Lcd_printf(40,100,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"哈哈哈哈哈，洽洽瓜子 ");
			Delay(1000);
			Paint_Bmp(0,0,320,240,bmp[i]);
			
		}
		
	}
	return(0);
	
}

void Delay(unsigned int x )
{
	unsigned int i,j,k;
	for(i=0;i<=x;i++)
		for(j=0;j<=0xff;j++)
			for(k=0;k<=0xff;k++);

}

