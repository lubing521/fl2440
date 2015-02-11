#include "led.h"
void Led_On(void)
{
	rGPBCON=0x1dd7fc;
	rGPBUP=0xfff;
	rGPBDAT&=~(1<<5|1<<6|1<<8|1<<10);
}
void Led_Off(void)
{
	rGPBCON=0x1dd7fc;
	rGPBUP=0xfff;
	rGPBDAT|=(1<<5|1<<6|1<<8|1<<10);
}


void Beep_On(void)
{
	rGPBCON|=0x01;
	rGPBUP=0xfff;
	rGPBDAT|=0x01;
}
void Beep_Off(void)
{
	rGPBCON|=0x01;
	rGPBUP=0xfff;
	rGPBDAT &= 0xfffe;
}