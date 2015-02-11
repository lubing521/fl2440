/*
*********************************************************
* Copyright (c)
* All rights reserved.				            
*
* 文件名称：key_Eint.c
* 文件标识：
* 摘    要：S3C2410 External interrupt test Test Program
* 当前版本：1.0
* 作    者：刘征
* 完成日期：2005.4.3
*
* 取代版本：
* 作    者：
* 完成日期：
*********************************************************
*/

/*
*********************************************************
*   					 头文件
*********************************************************
*/
#include  "Includes.H"
extern volatile float speed;
/* semaphores event control blocks */
//extern OS_EVENT *Mboxx ,*Mboxy;

/*
*********************************************************
* 函数介绍：本函数是Eint0中断处理程序。				
* 输入参数：无
* 输出参数：无
* 返回值  ：无
*********************************************************
*/
static void __irq Eint0Int(void)
{
	
	
    OSIntEnter();
    ClearPending(BIT_EINT0);
    OS_ENTER_CRITICAL();
    Uart_Printf("EINT0 interrupt is occurred.\n");
    OS_EXIT_CRITICAL();
    speed=speed/2;
//    coordinatex=1999;
    //coordinatey=888;  
//    OSSemPost(Sem1);
   
    
    //OSMboxPost(Mboxy,(void *)coordinateY);
    OSIntExit();
}

/*
*********************************************************
* 函数介绍：本函数是Eint1中断处理程序。				
* 输入参数：无
* 输出参数：无
* 返回值  ：无
*********************************************************
*/
static void __irq Eint1Int(void)
{
    OSIntEnter();
    ClearPending(BIT_EINT1);
    OS_ENTER_CRITICAL();
    Uart_Printf("EINT1 interrupt is occurred.\n");
    OS_EXIT_CRITICAL();
    OSIntExit();
}

/*
*********************************************************
* 函数介绍：本函数是Eint2中断处理程序。				
* 输入参数：无
* 输出参数：无
* 返回值  ：无
*********************************************************
*/
static void __irq Eint2Int(void)
{
    OSIntEnter();
    ClearPending(BIT_EINT2);
    OS_ENTER_CRITICAL();
    Uart_Printf("EINT2 interrupt is occurred.\n");
    OS_EXIT_CRITICAL();
    speed=speed/2;
    OSIntExit();
}

/*
*********************************************************
* 函数介绍：本函数是Eint3中断处理程序。				
* 输入参数：无
* 输出参数：无
* 返回值  ：无
*********************************************************
*/
static void __irq Eint3Int(void)
{
    OSIntEnter();
    ClearPending(BIT_EINT3);
    OS_ENTER_CRITICAL();
    Uart_Printf("EINT3 interrupt is occurred.\n");
    OS_EXIT_CRITICAL();
    OSIntExit();
}

/*
*********************************************************
* 函数介绍：本函数是测试Eint程序。				
* 输入参数：无
* 输出参数：无
* 返回值  ：无
*********************************************************
*/
void key_Eint(void)
{    
    rGPFCON = (rGPFCON & 0xff00)|(1<<7)|(1<<5)|(1<<3)|(1<<1);		//PF0/1/2/3 = EINT0/1/2/3
    
    rEXTINT0=(rEXTINT0 & ~(7<<0))|(2<<0);
    rEXTINT2=(rEXTINT2 & ~(7<<8))|(2<<8);
    //rEXTINT0 = (rEXTINT0 & ~((7<<8)  | (0x7<<0))) | 0x0<<12 | 0x0<<8 | 0x0<<4 | 0x0<<0; //EINT0/1/2/3=low level triggered
    	
    //设置中断入口函数
    pISR_EINT0=(U32)Eint0Int;
    pISR_EINT1=(U32)Eint1Int;
    pISR_EINT2=(U32)Eint2Int;
    pISR_EINT3=(U32)Eint3Int;

    rEINTPEND = 0xffffff;
    rSRCPND = BIT_EINT0|BIT_EINT1|BIT_EINT2|BIT_EINT3; //to clear the previous pending states
    rINTPND = BIT_EINT0|BIT_EINT1|BIT_EINT2|BIT_EINT3;
    
    rINTMSK=~(BIT_EINT0|BIT_EINT1|BIT_EINT2|BIT_EINT3);//开中断
}

