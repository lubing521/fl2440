#define GLOBAL_CLK 1
#include "2440lib.h"
#include "2440addr.h"
#include "def.h"
#include "option.h"
#include "2440slib.h"

#define MCLK	50000000
char myUart_GetKey(void);
void Main()
{
	char *str;
	char key;
	Uart_Init(MCLK,115200);
	Uart_Printf("hello!\n");
	//Uart_Select(0);
	while(1){
		//Uart_GetString(str);
		//Uart_Printf("The input string is:%s\n",str);
		//key=myUart_GetKey();
		Uart_Printf("The input char is:%d\n",key);
		
		}
}

char myUart_GetKey(void)
{
   while(1)
   	if(rUTRSTAT0 & 0x1)    //Receive data ready
       return RdURXH0();
   
}