#define GLOBAL_CLK 1
#define MCLK	50000000
#define XD0	50
#define YD0	50
#define XD1	300
#define	YD1	150
#define	XD2	150
#define	YD2 200

#define	EEFADDR	0x1F
#define EEKADDR 0x20
#define EECADDR	0x30	//eeprom address
#define	EEFLAG	0x6A	

#define COLOR_MAX	 3		//颜色种类
#include "2440lib.h"
#include "2440addr.h"
#include "def.h"
#include "option.h"
#include "2440slib.h"
#include "lcd.h"
#include "touchpanel.h"
#include "iic.h"
//#include "uart.h"
void cpu_init(void);
static void __irq ADC_ISR(void) ;
void Touch_Screen_Off(void);
void Touch_Screen_Init(void) ;
static void Draw_cross(int x,int y,int color,int ch);
void TS_cal(void);
void TS_cal_test(void);

int color[COLOR_MAX]={0x1111,0x8000,0x1814a};
volatile int xdata,ydata;
volatile int flagTS=0;
int	a[6],k ;//a,b,c,d,e,f
U8 ts_cal_flag;
void Main(void) 
{ 
 	int Xlcd,Ylcd,i=0;
 	cpu_init();
 	Lcd_Init();
	Lcd_PowerEnable(0,1);
    Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0xffff);
	Uart_Init(MCLK,115200); 
 	Uart_Printf("the main is running\n");    
 	Touch_Screen_Init();
  	iic_init();
  	TS_cal_test();
  	Uart_Printf("complete!\n");
  	//for(i=0;i<6;i++){
  		//Uart_Printf("ABCDEFK is %d\n",a[i]);
  		//}
  	Lcd_ClearScr(0x1111);
 	while(1){
 		if(flagTS){
 			Xlcd=(a[0]*xdata+a[1]*ydata+a[2])/k;
 			Ylcd=(a[3]*xdata+a[4]*ydata+a[5])/k;
 			Uart_Printf("X=%d\tY=%d\n",Xlcd,Ylcd);
 			flagTS=0;
 			Lcd_ClearScr(color[i++]);
 			if(i==COLOR_MAX) i=0;
 			Lcd_printf(Xlcd,Ylcd,0,0x1111,1,"test\n");
 			}
 	} 
} 

void cpu_init(void)
{
	int i;
	U8 key;
	U32 mpll_val = 0 ;
	//U32 divn_upll = 0 ;

	Port_Init();
	
	Isr_Init();
	
	i = 2 ;	//don't use 100M!
		//boot_params.cpu_clk.val = 3;
	switch ( i ) {
	case 0:	//200
		key = 14;
		mpll_val = (100<<12)|(2<<4)|(2);
		break;
	case 1:	//300
		key = 14;
		mpll_val = (60<<12)|(1<<4)|(1);
		break;
	case 2:	//400
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	case 3:	//440!!!
		key = 14;
		mpll_val = (102<<12)|(1<<4)|(1);
		break;
	default:
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	}
	
	//init FCLK=400M, so change MPLL first
	ChangeMPllValue((mpll_val>>12)&0xff, (mpll_val>>4)&0x3f, mpll_val&3);
	ChangeClockDivider(key, 12);
	
	cal_cpu_bus_clk();
	
}
static void Draw_cross(int x,int y,int color,int ch) //画出校正十字
{
	int i;
	for(i=-10;i<=10;i++){
	 		PutPixel(x+i,y,color );
 			PutPixel(x,y+i,color);
 			}
 	Lcd_PutASCII(x+5,y+5,ch,0,0,1);//输出标号
}

void TS_cal_test(void)
{
	 int i;
	 Rdeeprom(EEFADDR,(U8)0,&ts_cal_flag);
	 Uart_Printf("ts_cal_flag=%d\n",ts_cal_flag);
	 if(ts_cal_flag!=0x6A){
	 		TS_cal();
	 	}
	 k=rdTSfromIIC(0x20);
	 Delay(100);
	 for(i=0;i<6;i++){
	 	a[i]=rdTSfromIIC(EECADDR+4*i);
	 	Delay(100);
	}
	 	
}
void TS_cal(void) //触摸屏矫正
{
	int i;
	int xt[3],yt[3];
	Draw_cross(XD0,YD0,0,'1');
	Draw_cross(XD1,YD1,0,'2');
	Draw_cross(XD2,YD2,0,'3');
	Uart_Printf("click the cross!\n");
	for(i=0;i<3;i++){
		Uart_Printf("Point %d\n",i+1);
		while(flagTS==0){
			Delay(100);
		}
		xt[i]=xdata;
		yt[i]=ydata;
		flagTS=0;
		//Uart_Printf("Point %d <%d,%d>\n",i,xt[i],yt[i]);
		}
	//坐标转换	
	 k=(xt[0]-xt[2])*(yt[1]-yt[2])-(xt[1]-xt[2])*(yt[0]-yt[2]);
     a[0]=(XD0-XD2)*(yt[1]-yt[2])-(XD1-XD2)*(yt[0]-yt[2]);
     a[1]=(xt[0]-xt[2])*(XD1-XD2)-(XD0-XD2)*(xt[1]-xt[2]);
     a[2]=yt[0]*(xt[2]*XD1-xt[1]*XD2)+yt[1]*(xt[0]*XD2-xt[2]*XD0)+yt[2]*(xt[1]*XD0-xt[0]*XD1);
     a[3]=(YD0-YD2)*(yt[1]-yt[2])-(YD1-YD2)*(yt[0]-yt[2]);
     a[4]=(xt[0]-xt[2])*(YD1-YD2)-(YD0-YD2)*(xt[1]-xt[2]);
     a[5]=yt[0]*(xt[2]*YD1-xt[1]*YD2)+yt[1]*(xt[0]*YD2-xt[2]*YD0)+yt[2]*(xt[1]*YD0-xt[0]*YD1);
     ts_cal_flag=EEFLAG;
    
    // Wreeprom(EEFADDR,(U8)0,ts_cal_flag);                     //置“触摸屏校正参数计算并保持好”的标志信息
     Uart_Printf("in TS_cal ts_cal_flag=%d\n",ts_cal_flag);
     Delay(100); 											//等待一段时间，一定要有，否则EEPROM不能正确读写
     	
     wrTStoIIC(k,EEKADDR);
     Delay(100);
     for(i=0;i<6;i++){
     		wrTStoIIC(a[i],EECADDR+4*i);
     		Delay(100);
     	}
}


/************触摸屏初始化 ********************/
void Touch_Screen_Init(void) 
{ 
    rADCDLY=50000;    //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms 
    // Enable Prescaler=39,Prescaler A/D converter freq. = 50 MHz/(39+1) = 1.25MHz 
    //Conversion time = 1/(1.25MHz / 5cycles) = 1/250 kHz = 4 us   
    //AIN0,Normal,Disable read start,No operation 
    
    rADCCON = (1<<14)|(39<<6)|(0<<3)|(0<<2)|(0<<1)|(0);  
    //YM=GND,YP=AIN5,XM=Hi-z,XP=AIN7,XP pullup En,Normal ADC,Waiting for interrupt mode   
      
    rADCTSC = (0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3); 
    pISR_ADC    = (unsigned)ADC_ISR; 
    ClearSubPending(BIT_SUB_TC); 
    ClearSubPending(BIT_SUB_ADC); 
    ClearPending(BIT_ADC); 
    EnableSubIrq(BIT_SUB_TC); 
    EnableIrq(BIT_ADC);     
    Uart_Printf( "\nNow touchpanel controler is initial!\n" ) ; 
} 
/********* 触摸屏中断响应程序 ***********************/
static void __irq ADC_ISR(void) 
{ 
   DisableIrq(BIT_ADC); 
   DisableSubIrq(BIT_SUB_TC); 
   rADCTSC=(1<<3)|(1<<2);         //Pull-up disable, Seq. X,Y postion measure.
   rADCCON|=0x1;                   //start ADC 
  
   while(rADCCON & 0x1);    //check if Enable_start is low 
   while(!(rADCCON & 0x8000));        //check if EC(End of Conversion) flag is high, This line is necessary~!! 
   
   xdata=(0x3ff&rADCDAT0);
   ydata=(0x3ff&rADCDAT1);
   flagTS=1;
   
   ClearSubPending(BIT_SUB_TC);//
   
   rADCTSC =0xd3;    //Waiting for interrupt 
   rADCTSC=rADCTSC|(1<<8); // Detect stylus up interrupt signal. 
   while(1){ //等待抬起中断
   		if(rSUBSRCPND & ((unsigned)0x1<<9))
   			break;
   	}
   rADCTSC=rADCTSC&~(1<<8); // Detect stylus Down interrupt signal. 
   ClearPending(BIT_ADC); 
   ClearSubPending(BIT_SUB_TC); 
   ClearSubPending(BIT_SUB_ADC);
   EnableSubIrq(BIT_SUB_TC); 
   EnableIrq(BIT_ADC); 
   
   //Uart_Printf("the next touch\n"); 
   
}
/*******************触摸屏关闭 *****************/
void Touch_Screen_Off(void) 
{ 
   DisableIrq(BIT_ADC); 
   DisableSubIrq(BIT_SUB_TC); 
   rADCCON |= (1<<2); //ADC standby mode 
   rADCCON |= (1); //ADC no operation 
}



