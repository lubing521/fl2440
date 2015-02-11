#define GLOBAL_CLK 1
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 


void portinit(void);
void isrinit(void);
void output(void);
void mydelay(int time);
void shift_data(void);
void __irq  inputisr(void);
void myLed_Display(int data);
//void cal_cpu_bus_clk1(void);

volatile int flag=0;
int data=0x2;

static U32 cpu_freq;
static U32 UPLL;
void Main(void)
{
	char *mode;
	int i;
	U8 key;
	U32 mpll_val = 0 ;
	//U32 divn_upll = 0 ;
    
	#if ADS10   
//	__rt_lib_init(); //for ADS 1.0
	#endif

	Port_Init();
	
	Isr_Init();
	
	i = 2 ;	//don't use 100M!
		//boot_params.cpu_clk.val = 3;
	switch ( i ) {
	case 0:	//200
		key = 14;
		mpll_val = (100<<12)|(2<<4)|(2);
		break;
	case 1:	//300
		key = 14;
		mpll_val = (60<<12)|(1<<4)|(1);
		break;
	case 2:	//400
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	case 3:	//440!!!
		key = 14;
		mpll_val = (102<<12)|(1<<4)|(1);
		break;
	default:
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	}
	
	//init FCLK=400M, so change MPLL first
	ChangeMPllValue((mpll_val>>12)&0xff, (mpll_val>>4)&0x3f, mpll_val&3);
	ChangeClockDivider(key, 12);
	
	cal_cpu_bus_clk();
	portinit();
	isrinit();
	output();
}



void portinit(void)
{
	Port_Init();
	rGPFCON = rGPFCON & (~((3<<4)|(3<<0))) | ((2<<4)|(2<<0)) ; //GPF2,0 set  EINT
	rEXTINT0 &= ~(7|(7<<8));	
	rEXTINT0 |= (0|(0<<8));	//set eint0,2 falling edge int
	
	EnableIrq(BIT_EINT0);
	
	
}

void isrinit(void)
{
	pISR_EINT0=(U32)inputisr;
	
}

void output(void)
{
	while(1){
			Led_Display(data);
			mydelay(500);
			shift_data();	
			//data=0x04;
			//Led_Display(data);	
		}
		
}


void shift_data()
{
	if (flag==0)
		{
			data=data<<1;
			if ((data&~(1<<4))==0)
					data=0x1;
		}
	else
		{
			data=data>>1;
			if(data==0)
				data=0x8;
		}
}

void __irq  inputisr(void)
{
	ClearPending(BIT_EINT0);
	flag=~flag;
}

	
void mydelay(int time)
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
	