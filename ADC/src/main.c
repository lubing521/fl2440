#define GLOBAL_CLK 1
#include"2440addr.h"
#include"2440lib.h"
#include"def.h"
#include"option.h"

#define BIT_GPF(x) (rGPFDAT & (1<<x))
#define PCLK	50000000
#define ADC_FREQ	50000
#define S3C2440_MPLL_400MHZ     ((0x64<<12)|(0x03<<4)|(0x01))

void mDelay(int count);
void Test_adc(void);
int Read_ADC(void);

int Main(void)
{	
	int mpll_val1,mpll_val2;
	mpll_val1=rMPLLCON;
	mpll_val2=rMPLLCON;
	Uart_Init(PCLK,115200);
	Uart_Printf("welcome\n");
	Uart_Printf("MPLL1: 0x%x\nMPLL2: 0x%x\n",mpll_val1,mpll_val2);
	Test_adc();
	Uart_Printf("bye\n");
	while(1);
}

void Test_adc(void)
{
	int value=0;
	int preScaler;
	preScaler=PCLK/ADC_FREQ-1;
	Uart_Printf("PCLK/ADC-FREQ-1=%d\n",preScaler);
	while(Uart_GetKey()!=ESC_KEY){
		value=Read_ADC();
		Uart_Printf("Val:%d\n",value);
		mDelay(500);
	}
	Uart_Printf("Over\n");
}

int Read_ADC(void)
{
	rADCCON=(1<<14)|(49<<6)|(0<<3);
	rADCCON|=0x1;
	while(rADCCON&0x1);
	while(!(rADCCON&0x8000));
	return((int) rADCDAT0 & 0x3ff);
}


void mDelay(int time)
{
	int i,j,k;
	for(i=time;i>=0;i--)
		for(j=time;j>=0;j--)
			for(k=time;k>=0;k--)
				;
}

