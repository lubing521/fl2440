#define GLOBAL_CLK 1
#include"2440addr.h"
#include"def.h"
#include"option.h"

#define BIT_GPF(x) (rGPFDAT & (1<<x))
#define PCLK	50000000

void Delay(int time)
{
	int i,j,k;
	for(i=time;i>=0;i--)
		for(j=time;j>=0;j--)
			for(k=time;k>=0;k--)
				;
}


int LedMain(void)
{	
	int time=200;
	int count_flag=0;
	int mpll_val;
	rGPBCON=(1<<0)|(1<<10)|(1<<12)|(1<<16)|(1<<20);
	rGPBDAT=(1<<0)|(1<<5)|(1<<6)|(1<<8)|(1<<10);
	rGPBUP=0xffffffff;
	
	rGPFCON=0x0;
	rGPFUP = ~((1<<0)|(1<<2)|(1<<3)|(1<<4));
	while(1)
	{
		rGPBDAT=~((1<<0)|(1<<5));
		Delay(time);
		rGPBDAT=~((1<<0)|(1<<6));
		Delay(time);
		rGPBDAT=~((1<<0)|(1<<8));
		Delay(time);
		rGPBDAT=~((1<<0)|(1<<10));
		Delay(time);
		
		if(BIT_GPF(0)==0)	count_flag=0;
		if(BIT_GPF(2)==0) count_flag=1;
		if(time <= 0)
			count_flag=1;
		else if(time >=200)
			count_flag=0;
		if(count_flag == 0)
			time--;
		else
			time++;
			
		}
}





