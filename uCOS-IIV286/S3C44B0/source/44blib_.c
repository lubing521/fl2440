/************************************************
 * NAME    : 44BLIB.C				*
 * Version : 17.APR.00				*
 ************************************************/
#include "..\inc\44b.h"
#include "..\inc\44blib.h"
#include "..\inc\def.h"
#include "..\inc\option.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define STACKSIZE    0xa00 //SVC satck size(do not use user stack)
#define HEAPEND     (_ISR_STARTADDRESS-STACKSIZE-0x500) // = 0xc7ff000
	//SVC Stack Area:0xc(e)7ff000-0xc(e)7ffaff

extern char Image$$RW$$Limit[];

void *mallocPt=Image$$RW$$Limit;


/************************* SYSTEM *************************/
static int delayLoopCount=400;

void Delay(int time)
// time=0: adjust the Delay function by WatchDog timer.
// time>0: the number of loop time
// 100us resolution.
{
    int i,adjust=0;
    if(time==0)
    {
	time=200;
	adjust=1;
	delayLoopCount=400;
	rWTCON=((MCLK/1000000-1)<<8)|(2<<3);  // 1M/64,Watch-dog,nRESET,interrupt disable
	rWTDAT=0xffff;
	rWTCNT=0xffff;	 
	rWTCON=((MCLK/1000000-1)<<8)|(2<<3)|(1<<5); // 1M/64,Watch-dog enable,nRESET,interrupt disable 
    }
    for(;time>0;time--)
	for(i=0;i<delayLoopCount;i++);
    if(adjust==1)
    {
	rWTCON=((MCLK/1000000-1)<<8)|(2<<3);
	i=0xffff-rWTCNT;   //  1count/16us?????????
	delayLoopCount=8000000/(i*64);	//400*100/(i*64/200)   
    }
}

/************************* PORTS ****************************/

void Port_Init(void)
{    //CAUTION:Follow the configuration order for setting the ports. 
    // 1) setting value 
    // 2) setting control register 
    // 3) configure pull-up resistor.  

    //PORT A GROUP
    //GPA9 ADDR23 ADDR22 ADDR21 ADDR20 ADDR19 ADDR18 ADDR17 ADDR16 ADDR0		      
    //  0,    1,     1,    1,     1,     1,     1,      1,     1,    1
    rPCONA=0x1ff;	
    rPDATA=0x0;
    //PORT B GROUP
    //GPB10  GPB9 nGCS3 nGCS2 nGCS1 GPB5 GPB4 nSRAS nSCAS SCLK SCKE
    //  0,    0,    1,	1,    1,    0,    0,	1,    1,   1,   1	
    rPCONB=0x1Cf;
    
    //PORT C GROUP
    //IISLRCK  IISD0 IISDI IISCLK VD7 VD6 VD5 VD4 nXDACK1 nXDREQ1 GPC10 GPC11 TXD1 RXD1 GPC14 GPC15
    //All input
    //  11      11    11    11    11  11   11  11   11      11     01	01     11   11   01    01
    rPDATC=0x8400;
    rPCONC=0x5F5FFFFF;	
    rPUPC=0x33ff;	//should be enabled	
   
    //PORT D GROUP
    //VFRAME VM VLINE VCLK VD3 VD2 VD1 VD0
    //    10,10,   10,	10, 10,	10, 10,	10  
    rPCOND=0xaaaa;	
    rPUPD=0xff;
/*
    //PORT E GROUP
    //FOUT, GPE1, GPE2, GPE3, GPE4, GPE5, GPE6, GPE7, CODECLK   
    // 11     01,   01,   01,   01,   01,   01,   01,   00
    rPCONE=0x2552A;	
    rPUPE=0xff;		
    rPDATE=0X68;

    //PORT F GROUP
    //IICSCL IICSDA  nWAIT nXBACK0 nXDREQ0 GPF5 GPF6 GPF7 GPF8 
    //  10     10      10    10      10     0    0    0    0
    rPCONF=0x2A;
    rPUPF=0xff;
*/
	//PORT E GROUP
	//PE0:FOUT, PE1:TxD0, PE2:RxD0, GPE3, GPE4, GPE5,GPE6,GPE7, CODECLK   
	//    10       10,      10,    01  , 01,   01,  01,  01,   10
	rPCONE=0x2B;//0x2552A;	
	rPUPE=0x00;	//0xff;	
	rPDATE=0xff;
	
	//PORT F GROUP
	//IICSCL IICSDA  nWAIT nXBACK0 nXDREQ0 GPF5 GPF6 GPF7 GPF8 
	//  10     10      10    10      10     0    0    0    0
	rPCONF=0x9256A;//0x2A;
	rPUPF=0xff;

    //PORT G GROUP
    //EINT0 EINT1 EINT2 EINT3 GPG4 GPG5 GPG6 GPG7
    //	  0x0
    //  11      11   11     11   01   01   01   01
    rPDATG=0x0;
    rPCONG=0x55FF;
    rPUPG=0xf;
    
    rSPUCR=0x7;  //pull-up disable
    rEXTINT=0x22222022;  //All EINT[7:0] will be falling edge triggered.
}
/************************* UART ****************************/
static int whichUart=0;

void Uart_Init(int mclk,int baud)
{
    int i;
    if(mclk==0)
	mclk=MCLK;
    rUFCON0=0x0;     //FIFO disable
    rUFCON1=0x0;
    rUMCON0=0x0;
    rUMCON1=0x0;
//UART0
    rULCON0=0x3;     //Normal,No parity,1 stop,8 bit
//    rULCON0=0x7;     //Normal,No parity,2 stop,8 bit
    rUCON0=0x245;    //rx=edge,tx=level,disable timeout int.,enable rx error int.,normal,interrupt or polling
    rUBRDIV0=( (int)(mclk/16./baud + 0.5) -1 );
//UART1
//    rULCON1=0x7;     //Normal,No parity,2 stop,8 bit
    rULCON1=0x3;
    rUCON1=0x245;
    rUBRDIV1=( (int)(mclk/16./baud + 0.5) -1 );

    for(i=0;i<100;i++);
}


void Uart_Select(int ch)
{
    whichUart=ch;
}


void Uart_TxEmpty(int ch)
{
    if(ch==0)
	while(!(rUTRSTAT0 & 0x4)); //wait until tx shifter is empty.
    else
    	while(!(rUTRSTAT1 & 0x4)); //wait until tx shifter is empty.
}


char Uart_Getch(void)
{
    if(whichUart==0)
    {	    
	while(!(rUTRSTAT0 & 0x1)); //Receive data read
	return RdURXH0();
    }
    else
    {
	while(!(rUTRSTAT1 & 0x1)); //Receive data ready
	return	rURXH1;
    }
}


char Uart_GetKey(void)
{
    if(whichUart==0)
    {	    
	if(rUTRSTAT0 & 0x1)    //Receive data ready
    	    return RdURXH0();
	else
	    return 0;
    }
    else
    {
	if(rUTRSTAT1 & 0x1)    //Receive data ready
	    return rURXH1;
	else
	    return 0;
    }
}


void Uart_GetString(char *string)
{
    char *string2=string;
    char c;
    while((c=Uart_Getch())!='\r')
    {
	if(c=='\b')
	{
	    if(	(int)string2 < (int)string )
	    {
		Uart_Printf("\b \b");
		string--;
	    }
	}
	else 
	{
	    *string++=c;
	    Uart_SendByte(c);
	}
    }
    *string='\0';
    Uart_SendByte('\n');
}


int Uart_GetIntNum(void)
{
    char str[30];
    char *string=str;
    int base=10;
    int minus=0;
    int lastIndex;
    int result=0;
    int i;
    
    Uart_GetString(string);
    
    if(string[0]=='-')
    {
        minus=1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
	base=16;
	string+=2;
    }
    
    lastIndex=strlen(string)-1;
    if( string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
	base=16;
	string[lastIndex]=0;
	lastIndex--;
    }

    if(base==10)
    {
	result=atoi(string);
	result=minus ? (-1*result):result;
    }
    else
    {
	for(i=0;i<=lastIndex;i++)
	{
    	    if(isalpha(string[i]))
	    {
		if(isupper(string[i]))
		    result=(result<<4)+string[i]-'A'+10;
		else
		    result=(result<<4)+string[i]-'a'+10;
	    }
	    else
	    {
		result=(result<<4)+string[i]-'0';
	    }
	}
	result=minus ? (-1*result):result;
    }
    return result;
}


void Uart_SendByte(int data)
{
    if(whichUart==0)
    {
	if(data=='\n')
	{
	    while(!(rUTRSTAT0 & 0x2));
	    Delay(10);	//because the slow response of hyper_terminal 
	    WrUTXH0('\r');
	}
	while(!(rUTRSTAT0 & 0x2)); //Wait until THR is empty.
	Delay(10);
	WrUTXH0(data);
    }
    else
    {
	if(data=='\n')
	{
    	    while(!(rUTRSTAT1 & 0x2));
	    Delay(10);	//because the slow response of hyper_terminal 
	    rUTXH1='\r';
	}
	while(!(rUTRSTAT1 & 0x2));  //Wait until THR is empty.
	Delay(10);
	rUTXH1=data;
    }	
}		


void Uart_SendString(char *pt)
{
    while(*pt)
	Uart_SendByte(*pt++);
}


//if you don't use vsprintf(), the code size is reduced very much.
void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(string);
    va_end(ap);
}


/******************** S3C44B0X EV. BOARD LED **********************/
//		 -g--		a: data0	b: data1		
//	       a/_b_/f		c: data2	d: data3	dp: data4
//	       c/_d_/e dp	e: data5	f: data6	g: data7
void Led_Display(unsigned char data)
{
//    Uart_Printf("\n keyboard is %d\n\n",data);
}

/************************* PLL ********************************/
void ChangePllValue(int mdiv,int pdiv,int sdiv)
{
    rPLLCON=(mdiv<<12)|(pdiv<<4)|sdiv;
}


/************************* General Library **********************/

void * malloc(unsigned nbyte) 
{
    void *returnPt=mallocPt;

    mallocPt= (int *)mallocPt+nbyte/4+((nbyte%4)>0); //to align 4byte

    if( (int)mallocPt > HEAPEND )
    {
	mallocPt=returnPt;
	return NULL;
    }
    return returnPt;
}
void free(void *pt)
{
    mallocPt=pt;
}


void Cache_Flush(void)
{
    int i,saveSyscfg;
    
    saveSyscfg=rSYSCFG;

    rSYSCFG=SYSCFG_0KB; 		      
    for(i=0x10004000;i<0x10004800;i+=16)    
    {					   
	*((int *)i)=0x0;		   
    }
    rSYSCFG=saveSyscfg; 			    
}
/************************* Timer ********************************/

void Timer_Start(int divider)  //0:16us,1:32us 2:64us 3:128us
{
    rWTCON=((MCLK/1000000-1)<<8)|(divider<<3);
    rWTDAT=0xffff;
    rWTCNT=0xffff;   

    // 1/16/(65+1),nRESET & interrupt  disable
    rWTCON=((MCLK/1000000-1)<<8)|(divider<<3)|(1<<5);	
}


int Timer_Stop(void)
{
//    int i;
    rWTCON=((MCLK/1000000-1)<<8);
    return (0xffff-rWTCNT);
}
