/***********************************************
 * NAME    : 44BLIB.H                          *
 * Version : 17.Apr.00                         *
 ***********************************************/


#ifndef __44blib_h__
#define __44blib_h__

#define NULL 0

#define EnterPWDN(clkcon) ((void (*)(int))0xc0080e0)(clkcon)

/*44blib.c*/
void Delay(int time); //Watchdog Timer is used.
void *malloc(unsigned nbyte); 
void free(void *pt);

void Port_Init(void);
void Cache_Flush(void);
void ChangeMemCon(unsigned *pMemCfg);
void Uart_Select(int ch);
void Uart_TxEmpty(int ch);
void Uart_Init(int mclk,int baud);
char Uart_Getch(void);
char Uart_GetKey(void);
int  Uart_GetIntNum(void);
void Uart_SendByte(int data);
void Uart_Printf(char *fmt,...);
void Uart_SendString(char *pt);

void Timer_Start(int divider); //Watchdog Timer is used.
int Timer_Stop(void);          //Watchdog Timer is used.

void Led_Display(int data);

void ChangePllValue(int m,int p,int s);

#ifdef __cplusplus
}
#endif

#endif /*__44blib_h__*/
