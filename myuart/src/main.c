#define GLOBAL_CLK 1
#include "app.h"

void UartMain()
{
	char buf;
	
	disable_watchdog();
	clock_init();
	uart_init();
		
	while(1){
		if(rUTRSTAT0 & 0x01){
			buf=rURXH0;
			while(!(rUTRSTAT0 & 0x04));
			rUTXH0=buf;
			//new add
			buf=0;
		}
	}

}