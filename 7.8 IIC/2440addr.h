#define rCLKDIVN  (*(volatile unsigned *)0x4c000014)
//ISR
#define _IRQ_BASEADDRESS 	0x33ffff00
#define pISR_RESET		(*(unsigned *)(_IRQ_BASEADDRESS+0x0))
#define pISR_UNDEF		(*(unsigned *)(_IRQ_BASEADDRESS+0x4))
#define pISR_SWI		(*(unsigned *)(_IRQ_BASEADDRESS+0x8))
#define pISR_PABORT		(*(unsigned *)(_IRQ_BASEADDRESS+0xc))
#define pISR_DABORT		(*(unsigned *)(_IRQ_BASEADDRESS+0x10))
#define pISR_RESERVED	(*(unsigned *)(_IRQ_BASEADDRESS+0x14))
#define pISR_IRQ		(*(unsigned *)(_IRQ_BASEADDRESS+0x18))
#define pISR_FIQ		(*(unsigned *)(_IRQ_BASEADDRESS+0x1c))

#define pISR_WDT  ((*(unsigned *)(_IRQ_BASEADDRESS+0x44))//0x50))


//UART0
#define rUTRSTAT0   (*(volatile unsigned *)0x50000010)	//UART 0 Tx/Rx status
#define rULCON0     (*(volatile unsigned *)0x50000000)	//UART 0 Line control
#define rUCON0      (*(volatile unsigned *)0x50000004)	//UART 0 Control
#define rUFCON0     (*(volatile unsigned *)0x50000008)	//UART 0 FIFO control
#define rUBRDIV0    (*(volatile unsigned *)0x50000028)	//UART 0 Baud rate divisor
#define WrUTXH0(ch) (*(volatile unsigned char *)0x50000020)=(unsigned char)(ch)


//WDT
#define rWTCON  (*(volatile unsigned *)0x53000000)
#define rWTDAT  (*(volatile unsigned *)0x53000004)
#define rWTCNT  (*(volatile unsigned *)0x53000008)



//INTRRUPT
#define rSRCPND        (*(volatile unsigned *)0x4A000000)
#define rINTMOD        (*(volatile unsigned *)0x4A000004)
#define rINTMSK        (*(volatile unsigned *)0x4A000008)
#define rPRIORITY      (*(volatile unsigned *)0x4A00000c)
#define rINTPND        (*(volatile unsigned *)0x4A000010)
#define rSUBSRCPND     (*(volatile unsigned *)0x4A000018)
#define rINTSUBMSK     (*(volatile unsigned *)0x4A00001C)

//IIC
#define rIICCON        (*(volatile unsigned *)0x54000000)
#define rIICSTAT       (*(volatile unsigned *)0x54000004)
#define rIICADD        (*(volatile unsigned *)0x54000008)
#define rIICDS         (*(volatile unsigned *)0x5400000C)
#define rIICLC         (*(volatile unsigned *)0x54000010)

//I/O PORT
#define rGPEUP         (*(volatile unsigned *)0x56000048)
#define rGPEDAT        (*(volatile unsigned *)0x56000044)
#define rGPECON        (*(volatile unsigned *)0x56000040)

#define prescaler_value  100
#define clock_select     3
#define  BIT_WDT_AC97 (1<<9)
#define  BIT_SUB_WDT (1<<13)
#define PCLK 50000000
#define IICBUFSIZE 0x20

#define U8 unsigned char
#define U32 unsigned int

#define WRDATA      (1)
#define POLLACK     (2)
#define RDDATA      (3)
#define SETRDADDR   (4)