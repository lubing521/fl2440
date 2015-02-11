#include <stdarg.h>
#include "2440addr.h"

void Uart_Init(int baud)
{
    int i;
    rUFCON0 = 0x0;   //UART channel 0 FIFO control register, FIFO disable
  
 
//UART0
    rULCON0 = 0x3;   //Line control register : Normal,No parity,1 stop,8 bits
     //    [10]       [9]     [8]        [7]        [6]      [5]         [4]           [3:2]        [1:0]
     // Clock Sel,  Tx Int,  Rx Int, Rx Time Out, Rx err, Loop-back, Send break,  Transmit Mode, Receive Mode
     //     0          1       0    ,     0          1        0           0     ,       01          01
     //   PCLK       Level    Pulse    Disable    Generate  Normal      Normal        Interrupt or Polling
    rUCON0  = 0x805;   // Control register
    rUBRDIV0=( (int)(50000000/16./baud+0.5) -1 );   //Baud rate divisior register 0
//UART1

    for(i=0;i<100;i++);
}


    
//=====================================================================
void Uart_SendByte(int data)
{
   
        if(data=='\n')
        {
            while(!(rUTRSTAT0 & 0x2));
           // Delay(1);                 //because the slow response of hyper_terminal 
            WrUTXH0('\r');
        }
        while(!(rUTRSTAT0 & 0x2));   //Wait until THR is empty.
      //  Delay(1);
        WrUTXH0(data);
  
   
}               

//====================================================================
void Uart_SendString(char *pt)
{
    while(*pt)
        Uart_SendByte(*pt++);
}

//=====================================================================
//If you don't use vsprintf(), the code size is reduced very much.
void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char str[255];

    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    Uart_SendString(str);
    va_end(ap);
}