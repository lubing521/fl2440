#include "app.h"

#define S3C2440_MPLL_400MHZ     ((0x64<<12)|(0x03<<4)|(0x01))
#define MEM_CTL_BASE	13

void disable_watchdog(void)
{
	rWTCON=0;
}

void clock_init(void)
{
	rCLKDIVN=0x05;
	 /* 读出控制寄存器 */ 
	 /* 设置为“asynchronous bus mode” */
	 /* 写入控制寄存器 */
	/*
	__asm__(
    "mrc    p15, 0, r1, c1, c0, 0\n"       
    "orr    r1, r1, #0xc0000000\n"          
    "mcr    p15, 0, r1, c1, c0, 0\n"        
    );
    */
   rMPLLCON = S3C2440_MPLL_400MHZ;
}

void memsetup(void)
{
    volatile unsigned long *p = (volatile unsigned long *)MEM_CTL_BASE;

    /* 这个函数之所以这样赋值，而不是像前面的实验(比如mmu实验)那样将配置值
     * 写在数组中，是因为要生成”位置无关的代码”，使得这个函数可以在被复制到
     * SDRAM之前就可以在steppingstone中运行
     */
    /* 存储控制器13个寄存器的值 */
    p[0] = 0x22011110;     //BWSCON
    p[1] = 0x00000700;     //BANKCON0
    p[2] = 0x00000700;     //BANKCON1
    p[3] = 0x00000700;     //BANKCON2
    p[4] = 0x00000700;     //BANKCON3  
    p[5] = 0x00000700;     //BANKCON4
    p[6] = 0x00000700;     //BANKCON5
    p[7] = 0x00018005;     //BANKCON6
    p[8] = 0x00018005;     //BANKCON7
    
    /* REFRESH,
     * HCLK=12MHz:  0x008C07A3,
     * HCLK=100MHz: 0x008C04F4
     */ 
    p[9]  = 0x008C04F4;
    p[10] = 0x000000B1;     //BANKSIZE
    p[11] = 0x00000030;     //MRSRB6
    p[12] = 0x00000030;     //MRSRB7
}

void copy_steppingstone_to_sdram(void)
{
    unsigned int *pdwSrc  = (unsigned int *)0;
    unsigned int *pdwDest = (unsigned int *)0x30000000;
    
    while (pdwSrc < (unsigned int *)4096)
    {
        *pdwDest = *pdwSrc;
        pdwDest++;
        pdwSrc++;
    }
}

void uart_init(void)
{
	rGPBCON = 0x015551;
	rGPBUP  = 0x7ff;
	rGPBDAT = 0x1e0;
   
	rGPHCON = 0x00faaa;                //使用UART0功能
	rGPHUP  = 0x7ff;
	
	rULCON0=0x03;
	rUCON0=0x0805;
	rUBRDIV0=0x1A;
}

