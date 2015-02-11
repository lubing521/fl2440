
// *************************************************
// 这是学ARM9和ADS1.2的一个很好的例程，这个例程简单易懂。
// 这个例程可以用开发板是的LED灯和仿真器来测试硬件的好坏，
// 还可以CACHE对程序运行速度的影响，测试设置FCLK的频率。
// LED test
//2004.10.25
//***************************************************
#include "def.h"
#include "option.h"
#include "2440addr.h"     //
#include "2440lib.h"
#include "2440slib.h"     //void MMU_EnableDCache(); 
//================================


void __rt_lib_init(void);
void dely(U32 tt)
{
   U32 i;
   for(;tt>0;tt--)
   {
     for(i=0;i<10000;i++){}
   }
}
   

int Main(int argc, char **argv)
{
	int i;
	U8 key;
	U32 mpll_val=0;
	
	char *st="LED is OK!\n\0";
	
    #if ADS10   
    __rt_lib_init(); //for ADS 1.0
    #endif
    
	i = 2 ;	//hzh, don't use 100M!
		//boot_params.cpu_clk.val = 3;
	switch ( i ) {
	case 0:	//200
		key = 12;
		mpll_val = (92<<12)|(4<<4)|(1);
		break;
	case 1:	//300
		key = 13;
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

//     ChangeClockDivider(1,1);    // 1:2:4    FCLK:HCLK:PCLK
     // rCLKDIVN=0x4;    //  1:4:4
    //ChangeMPllValue(82,2,1);     //FCLK=135.0Mhz     
 //   ChangeMPllValue(82,1,1);     //FCLK=180.0Mhz     
    //ChangeMPllValue(161,3,1);    //FCLK=202.8Mhz 
    //ChangeMPllValue(117,1,1);    //FCLK=250.0Mhz 
    //ChangeMPllValue(122,1,1);    //FCLK=260.0Mhz 
    //ChangeMPllValue(125,1,1);    //FCLK=266.0Mhz 
    //ChangeMPllValue(127,1,1);    //FCLK=270.0Mhz  
    
    //MMU_EnableICache();
    //MMU_EnableDCache();
    
    MMU_DisableICache();
    MMU_DisableDCache();

    Uart_Init(0,115200);
    Uart_Select(0);
    Uart_SendString("hello,qq2440\n");
    Uart_Printf("hello,qq2440, printf\n");

   rGPBCON = 0x00555555;
   rGPBDAT = 0x0000;
   rGPBDAT = 0x0ffffff;
   
   while(1)
   {
   	 
     rGPBDAT = 0x0000;
     dely(120);
     rGPBDAT = 0x0ffffff;
     dely(120);
     Uart_SendString(st);
   }
   
   return 0;
}