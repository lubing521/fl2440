#define GLOBAL_CLK 1
#include"2440addr.h"
#include"def.h"
#include"option.h"

#define BIT_GPF(x) (rGPFDAT & (1<<x))
#define PCLK	50000000
#define S3C2440_MPLL_400MHZ     ((0x64<<12)|(0x03<<4)|(0x01))


void disable_watchdog(void)
{
	rWTCON=0;
}

void clock_init(void)
{
	rCLKDIVN=0x05;
	 /* ¶Á³ö¿ØÖÆ¼Ä´æÆ÷ */ 
	 /* ÉèÖÃÎª¡°asynchronous bus mode¡± */
	 /* Ð´Èë¿ØÖÆ¼Ä´æÆ÷ */
	/*
	__asm__(
    "mrc    p15, 0, r1, c1, c0, 0\n"       
    "orr    r1, r1, #0xc0000000\n"          
    "mcr    p15, 0, r1, c1, c0, 0\n"        
    );
    */
   rMPLLCON = S3C2440_MPLL_400MHZ;
}
void Delay(int time)
{
	U32 val = (PCLK>>3)/1000-1;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	rTCFG1 &= ~(0xf<<12);
	rTCFG1 |= 0<<12;		//mux = 1/2

	rTCNTB3 = val;
	rTCMPB3 = val>>1;		// 50%
	rTCON &= ~(0xf<<16);
	rTCON |= 0xb<<16;		//interval, inv-off, update TCNTB3&TCMPB3, start timer 3
	rTCON &= ~(2<<16);		//clear manual update bit
	while(time--) {
		while(rTCNTO3>=val>>1);
		while(rTCNTO3<val>>1);
	};
}
int LedMain(void)
{	
	int time=200;
	int count_flag=0;

	disable_watchdog();
	clock_init();
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

/*
void Delay(int time)
{
	int i,j,k;
	for(i=time;i>=0;i--)
		for(j=time;j>=0;j--)
			for(k=time;k>=0;k--)
				;
}
*/

