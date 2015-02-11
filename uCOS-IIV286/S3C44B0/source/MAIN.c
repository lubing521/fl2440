
#include "../include/includes.h"               // uC/OS interface
//#include "../include/44blib.h"
//#include "../include/def.h"
//#include "../include/44b.h"

//Add By San
void Isr_Init(void);
void Beep(unsigned int time);

// allocate memory for tasks' stacks 
#ifdef SEMIHOSTED
#define	STACKSIZE	(64+SEMIHOSTED_STACK_NEEDS)
#else
#define	STACKSIZE	256
#endif
OS_STK Stack1[STACKSIZE]= {0, };
OS_STK Stack2[STACKSIZE]= {0, };
OS_STK Stack3[STACKSIZE]= {0, };

// mailbox event control blocks 
OS_EVENT *Mbox1;
OS_EVENT *Mbox2;
OS_EVENT *Mbox3;

char PassMsg[] = "SW44B0";

void Task1(void *Id)
{
	char *Msg;
	INT8U err;

	Uart_Printf("Task1() called\r\n");

	for(;;){
		// wait for a message from the input mailbox 
		Msg = (char *)OSMboxPend(Mbox1, 0, &err);

		Uart_Printf("\nTask1() called\r\n");
		// print task's id 
		Uart_Printf("%c", *(char *)Id);
		//OSTimeDly(5);
		
		//test led
		//Led_Display(0);
    	//Delay(1000);
    	//Led_Display(3);
    	//Delay(10000);

		// post the input message to the output mailbox 
		OSMboxPost(Mbox2, Msg);
	}
}

void Task2(void *Id)
{
	char *Msg;
	INT8U err;

	Uart_Printf("Task2() called\r\n");

	for(;;){

		// wait for a message from the input mailbox 
		Msg = (char *)OSMboxPend(Mbox2, 0, &err);

		Uart_Printf("\nTask2() called\r\n");
		// print task's id 
		Uart_Printf("%c", *(char *)Id);

		//test led
		//Led_Display(0);
    	//Delay(1000);
    	//Led_Display(3);
    	//Delay(10000);

		// post the input message to the output mailbox 
		OSMboxPost(Mbox3, Msg);
	}
}


void Task3(void *Id)
{
	char *Msg;
	INT8U err;

	Uart_Printf("Task3() called\r\n");


	for(;;){
		// wait for a message from the input mailbox 
		Msg = (char *)OSMboxPend(Mbox3, 0, &err);

		Uart_Printf("\nTask3() called\r\n");
		// print task's id 
		Uart_Printf("%c", *(char *)Id);
		
		//test led
		//Led_Display(0);
    	//Delay(1000);
    	//Led_Display(3);
    	//Delay(10000);
    	
    	//Beep(500);		//蜂鸣器鸣叫若干个100us

		// post the input message to the output mailbox 
		OSMboxPost(Mbox1, Msg);
	}
}

int Main(int argc, char **argv)
//int main(void)
{
	char Id1 = '1';
	char Id2 = '2';
	char Id3 = 't';
	
	//Isr_Init();
	
	Port_Init();
	Uart_Init(0,115200);
	Uart_Select(0);
	Delay(0);
	Delay(5000);
	
	Uart_Printf("\n###Let's begin\n");
	//test uart
	//while(1)
	//{
	//	Uart_Printf("\n###Let's begin\n");
	//	Uart_Printf("\n00000000000000\n");
	//}

	OSInit();

	Mbox1 = OSMboxCreate(PassMsg);
	Mbox2 = OSMboxCreate((void *)0);
	Mbox3 = OSMboxCreate((void *)0);

	OSTaskCreate(Task1, (void *)&Id1, (OS_STK *)&Stack1[STACKSIZE - 1], 3);
	OSTaskCreate(Task2, (void *)&Id2, (OS_STK *)&Stack2[STACKSIZE - 1], 2);
	OSTaskCreate(Task3, (void *)&Id3, (OS_STK *)&Stack3[STACKSIZE - 1], 1);

	OSStart();

	return 0;
}	// main

void Isr_Init(void)
{
    U32 i;
    
    for(i=_RAM_STARTADDRESS;i<(_RAM_STARTADDRESS+0x20);i+=4)
    {
	  *((volatile unsigned *)i)=0xEA000000+0x1FFE;
    }

    //rINTCON=0x1;	  // Vectored Int. IRQ enable,FIQ disable    
    rINTCON=0x5;	  // Non-vectored,IRQ enable,FIQ disable    

    rINTMOD=0x0;	  // All=IRQ mode
    rINTMSK|=BIT_GLOBAL|BIT_EINT4567;	  // All interrupt is masked.
}

/****************************************************************************
【功能说明】蜂鸣器鸣叫time个100us
****************************************************************************/
void Beep(unsigned int time)
{
	rPDATE = (rPDATE | 0x08);
	Delay(time);		//延时若干个100us
	rPDATE = (rPDATE & 0x1f7);
}
//***************************************************************************