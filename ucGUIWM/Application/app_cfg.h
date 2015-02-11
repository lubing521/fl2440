#ifndef APP_CFG_H
#define APP_CFG_H


//Task Stk lengh

#define	MainTaskStkLengh	1024*2   	// Define the MainTask stack length 
#define	TaskStkLeath		1024     	// Define the Task0 stack length 




//Task function
void    MainTask(void *pdata);   		//MainTask task
void 		Task0(void *pdata);			    // Task0 
void 		Task1(void *pdata);			    // Task1 
void 		TaskTSICC(void *pdata);			    // Task2
void 		Task3(void *pdata);
void 		Task4(void *pdata);
void    TaskUart(void *pdata);          //Task Uart


//Task Prio
#define NormalTaskPrio       5
#define MainTaskPrio 	NormalTaskPrio
#define Task0Prio 		NormalTaskPrio+1
#define Task1Prio  		NormalTaskPrio+2
#define Task2Prio  		NormalTaskPrio+3
#define TaskUartPrio  	NormalTaskPrio+4


#endif

void cpu_init(void);
void FL_Lcd_Init(void);