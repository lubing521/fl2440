#include "includes.h"

void FL_Lcd_Init(void)
{
	Lcd_Init();
	Lcd_PowerEnable(0,1);
  	Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0x1111);
}
