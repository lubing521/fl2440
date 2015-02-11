
#define GLOBAL_CLK 1
#include "2440addr.h"
#include "def.h"
void Led_On(void);
void Led_Off(void);
void LedCount(U8 count);
void Beep_On(void);
void Beep_Off(void);
void BeepCount(U8 count);

U8 KeyNum(void );
void Delay(U32 x );
int KeyMain()
{
	rGPFCON=0x3ffc0c;
	rGPBCON=0x3ff7fd;
	//rGPBCON|=0x01;
	rGPBUP=0xfff;
	rGPFUP=0xfff;
	//rGPBDAT&=0x0;
	Led_On();
	//Beep_Off();
	while(1){
		LedCount(KeyNum());
		//BeepCount(KeyNum());
	}
	
	return(0);
	
}
void Led_On(void)
{
	rGPBDAT&=~(1<<5);
}
void Led_Off(void)
{
	rGPBDAT|=(1<<5);
}

void LedCount(U8 count)
{
	U32 time;
	time=count*100;
	if(time!=0){
		Led_On();
		Delay(time);
		Led_Off();
		Delay(time);
	}
}

void Beep_On(void)
{
	rGPBDAT|=0x01;
}
void Beep_Off(void)
{
	rGPBDAT &= 0xfffe;
}

void BeepCount(U8 count)
{
	U32 time;
	time=count*100;
	if(time!=0){
		Beep_On();
		Delay(time);
		Beep_Off();
		Delay(time);
	}
}


U8 KeyNum(void)
{
	U8 keytemp;
	keytemp=0;
	if((rGPFDAT&0x01)==0) 	keytemp=1;
	if((rGPFDAT&0x04)==0)	keytemp=2;
	if((rGPFDAT&0x08)==0)	keytemp=4;
	if((rGPFDAT&0x10)==0)	keytemp=8;
		
	return keytemp;
	
}
void Delay(unsigned int x )
{
	unsigned int i,j,k;
	for(i=0;i<=x;i++)
		for(j=0;j<=0xff;j++)
			for(k=0;k<=0xff;k++);

}

