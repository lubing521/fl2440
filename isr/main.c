#include "2440addr.h"
#include "lcd.h"

#define RGB(r,g,b)   (unsigned int)( (r << 16) + (g << 8) + b )
#define FROM_BCD(n)		((((n) >> 4) * 10) + ((n) & 0xf))
#define TO_BCD(n)		((((n) / 10) << 4) | ((n) % 10))



volatile unsigned char select=0;

extern unsigned char ok2440[];
extern unsigned char TE2440[];
extern unsigned char TE2440_ii[];

unsigned char *bmp[3]=
{
	ok2440,
	TE2440,
	TE2440_ii
	};

static void __irq Eint_isr(void);
static void __irq Eint0_isr(void);
static void __irq Eint2_isr(void);
void Eint_init(void);
void Eint_enable(void);
void Led_On(void);
void Led_Off(void);
void Beep_On(void);
void Beep_Off(void);
void Delay(unsigned int x );
void Main()
{

  Lcd_Init();
  Lcd_PowerEnable(0,1);
  Lcd_EnvidOnOff(1);
  Lcd_ClearScr(0x1111);
  
  
  Eint_init();
  Eint_enable();
  
  //Lcd_printf(40,100,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"Çë°´¼üÑ¡Ôñ£º\n");
  //Lcd_printf(50,120,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"1:·ÉÁè\n");
  //Lcd_printf(50,140,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"2:ÈýÐÇ\n");
  while(1)
  {
  	switch(select){
  		case 1:
  			Led_On();
  			Paint_Bmp(0,0,320,240,bmp[0]);
  			break;
  		case 2:
  			Beep_On();
  			Paint_Bmp(0,0,320,240,bmp[1]);
  			break;
  		default:
  			Led_Off();
  			Beep_Off();
  			//Lcd_ClearScr(0x1111);
  			Lcd_printf(40,100,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"Çë°´¼üÑ¡Ôñ£º\n");
 			Lcd_printf(50,120,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"1:·ÉÁè\n");
  			Lcd_printf(50,140,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"2:ÈýÐÇ\n");
  			break;
  		}
  	}
	
}
static void __irq Eint_isr(void)
{
	
	Delay(20);
	if((rINTPND & BIT_EINT0)){
		ClearPending(BIT_EINT0);
		if(select)
			select=0;
		else
			select=1;
			}
	if((rINTPND & BIT_EINT2)){
		ClearPending(BIT_EINT2);
		if(select)
			select=0;
		else
			select=2;
			}
			
}



static void __irq Eint0_isr(void)
{
	
	Delay(20);
	ClearPending(BIT_EINT0);
	if(select)
		select=0;
	else
		select=1;
}
static void __irq Eint2_isr(void)
{
	Delay(20);
	ClearPending(BIT_EINT2);
	if(select)
		select=0;
	else
		select=2;
}

void Eint_init(void)
{
	rGPFCON=rGPFCON & ~(3)|(1<<1);
    rGPFCON=rGPFCON & ~(3<<4)|(1<<5);
    
    rGPFUP|=(1<<0);
    rGPFUP|=(1<<2);
    
    rEXTINT0=(rEXTINT0 & ~(7<<0))|(2<<0);
    rEXTINT2=(rEXTINT2 & ~(7<<8))|(2<<8);
    
    rEINTPEND=0xffffff;
	rSRCPND|=BIT_EINT0|BIT_EINT2;
	rINTPND|=BIT_EINT0|BIT_EINT2;
       
    //pISR_EINT0=(unsigned)Eint0_isr;
    //pISR_EINT2=(unsigned)Eint2_isr;
    pISR_EINT0=(unsigned)Eint_isr;
    pISR_EINT2=(unsigned)Eint_isr;

}
void Eint_enable(void)
{
	
	rINTMSK=~(BIT_EINT0|BIT_EINT2);
}
void Led_On(void)
{
	rGPBCON=0x3ff7fd;
	rGPBUP=0xfff;
	rGPBDAT&=~(1<<5);
}
void Led_Off(void)
{
	rGPBCON=0x3ff7fd;
	rGPBUP=0xfff;
	rGPBDAT|=(1<<5);
}


void Beep_On(void)
{
	rGPBCON|=0x01;
	rGPBUP=0xfff;
	rGPBDAT|=0x01;
}
void Beep_Off(void)
{
	rGPBCON|=0x01;
	rGPBUP=0xfff;
	rGPBDAT &= 0xfffe;
}
void Delay(unsigned int x )
{
	unsigned int i,j,k;
	for(i=0;i<=x;i++)
		for(j=0;j<=0xff;j++)
			for(k=0;k<=0xff;k++);

}


