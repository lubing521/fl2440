#include "includes.h"
//#include "config.h"

//extern void Lcd_N35_Init(void);

void TargetInit(void)
{
    int i;          
/*1. 设定系统时钟*/
	U8 key;
	U32 mpll_val=0;
    #if ADS10   
    __rt_lib_init(0,0); //for ADS 1.0
 
    #endif
    
	i = 2 ;	//use 400M!
		
	switch ( i ) {
	case 0:	//200
		key = 12;
		mpll_val = (92<<12)|(4<<4)|(1);
		break;
	case 1:	//300
		key = 14;
		mpll_val = (67<<12)|(1<<4)|(1);
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

    //MMU_EnableICache();
    //MMU_EnableDCache();
    
    //MMU_DisableICache();
    //MMU_DisableDCache();

/*端口初始化*/
  	Port_Init();

		Isr_Init();

/*MMU初始化*/
    //MMU_Init();


/* 串口初始化*/
    Delay(0);
    Uart_Init(0,115200);
    Uart_Select(0);
    Uart_SendString("Target Initializing...\n");
  
 /* LED指示灯初始化  */
   rGPFCON = 0x001540;
   rGPFDAT = 0x0000;

 	
}