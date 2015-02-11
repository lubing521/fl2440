#include "touchpanel.h"
void cpu_init(void);
void TS_cal_init(void);
void __irq ADC_ISR(void) ;
void Touch_Screen_Off(void);
void Touch_Screen_Init(void) ;
void Draw_cross(int x,int y,int color,int ch);
void TS_cal(void);
void Coordinate_Lcd(U16 *Xlcd, U16 *Ylcd);

int color[COLOR_MAX]={0x1111,0x8000,0x1814a};//屏幕背景颜色
volatile int xdata,ydata;
volatile int flagTS=0;
int coef[8];//坐标转换系数，从eeprom读取

void Coordinate_Lcd(U16 *Xlcd, U16 *Ylcd)
{
		*Xlcd=(coef[2]*xdata+coef[3]*ydata+coef[4])/coef[1];
 		*Ylcd=(coef[5]*xdata+coef[6]*ydata+coef[7])/coef[1];		
}

void touchpanel(void) 
{ 
 	U16 Xlcd,Ylcd,i=0;
 	cpu_init();
 	Lcd_Init();
	Lcd_PowerEnable(0,1);
  Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0xffff);
	Uart_Init(MCLK,115200); 
 	Uart_Printf("the main is running\n");    
 	Touch_Screen_Init();
    iic_init();
    TS_cal_init();
  
    Lcd_ClearScr(0x1111);
 	while(1){
 		if(flagTS){
 			Coordinate_Lcd(&Xlcd, &Ylcd);
 			Uart_Printf("X=%d\tY=%d\n",Xlcd,Ylcd);
 			flagTS=0;
 			Lcd_ClearScr(color[i++]);
 			if(i==COLOR_MAX) i=0;
 			Lcd_printf(Xlcd,Ylcd,0,0x1111,1,"test\n");
 			}
 	} 
} 
void TS_cal_init(void)
{
	 int i,j;
	 U8 ts_cal_flag;
	 U8 buffer[SIZE_CONVERT];
	 U8 key;
	 Rdeeprom(EE_PAGE_ADDR,(U8)0,&ts_cal_flag);
	 Uart_Printf("TS Calibrate?<y/n>:");
	 Lcd_printf(50,120,0,0x0,1,"Waiting for selection...");
	 key=Uart_Getch();
	 if(key=='y')
	 		ts_cal_flag=0;
	 if(ts_cal_flag!=0x6A){
	 		TS_cal();
	 		Uart_Printf("Calibrate complete!\n");
	 	}
	 for(i=0;i<SIZE_CONVERT;i++){
	 		Rdeeprom(EE_PAGE_ADDR,(U8)i,&(buffer[i]));
	 		//Uart_Printf("rd %x\t",buffer[i]);
	 
	 }
	  Uart_Printf("\n");
	 for(i=0,j=0;j<SIZE;j++){
	 	coef[j]=(buffer[i]<<24)|(buffer[i+1]<<16)|(buffer[i+2]<<8)|(buffer[i+3]);
	 	i=i+4;
	 	//Uart_Printf("rbuffer %x\t%x\t%x\t%x\n",rbuffer[0],rbuffer[1],rbuffer[2],rbuffer[3]);
	 	//Uart_Printf("rresult[%d]=%d\n",j,coef[j]);
	 	}
	 
	  ClearSubPending(BIT_SUB_TC); 
    ClearSubPending(BIT_SUB_ADC); 
    ClearPending(BIT_ADC); 
    EnableSubIrq(BIT_SUB_TC); 
    EnableIrq(BIT_ADC);     
    Uart_Printf( "\nNow touchpanel controler is initial!\n" ) ; 
}



void TS_cal(void) //触摸屏矫正
{
	S32 i,j;
	S32 xt[3],yt[3];
 	U8 buffer_1[SIZE_CONVERT];
 	U8 buffer[SIZE][4];
 	
 	Lcd_ClearScr(0xffff);
	Draw_cross(XD0,YD0,0,'1');
	Draw_cross(XD1,YD1,0,'2');
	Draw_cross(XD2,YD2,0,'3');
	Uart_Printf("click the cross!\n");
	Lcd_printf(80,120,0,0x0,1,"Calibrate TS");
	ClearSubPending(BIT_SUB_TC); 
    ClearSubPending(BIT_SUB_ADC); 
    ClearPending(BIT_ADC); 
    EnableSubIrq(BIT_SUB_TC); 
    EnableIrq(BIT_ADC);
	
	
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
	 	 coef[0]=EEFLAG;
	 	 coef[1]=(xt[0]-xt[2])*(yt[1]-yt[2])-(xt[1]-xt[2])*(yt[0]-yt[2]);
     coef[2]=(XD0-XD2)*(yt[1]-yt[2])-(XD1-XD2)*(yt[0]-yt[2]);
     coef[3]=(xt[0]-xt[2])*(XD1-XD2)-(XD0-XD2)*(xt[1]-xt[2]);
     coef[4]=yt[0]*(xt[2]*XD1-xt[1]*XD2)+yt[1]*(xt[0]*XD2-xt[2]*XD0)+yt[2]*(xt[1]*XD0-xt[0]*XD1);
     coef[5]=(YD0-YD2)*(yt[1]-yt[2])-(YD1-YD2)*(yt[0]-yt[2]);
     coef[6]=(xt[0]-xt[2])*(YD1-YD2)-(YD0-YD2)*(xt[1]-xt[2]);
     coef[7]=yt[0]*(xt[2]*YD1-xt[1]*YD2)+yt[1]*(xt[0]*YD2-xt[2]*YD0)+yt[2]*(xt[1]*YD0-xt[0]*YD1);
	
	for(i=0;i<8;i++){
		Uart_Printf("coef[%d]=%d\n",i,coef[i]);
		}
     
    
	  for(i=0;i<SIZE;i++){
		  buffer[i][0]=(unsigned char)((coef[i]&0xFF000000)>>24);
     	buffer[i][1]=(unsigned char)((coef[i]&0x00FF0000)>>16);
     	buffer[i][2]=(unsigned char)((coef[i]&0x0000FF00)>>8);
     	buffer[i][3]=(unsigned char)(coef[i]&0x000000FF);
     	//Uart_Printf("buffer %x\t%x\t%x\t%x\n",buffer[i][0],buffer[i][1],buffer[i][2],buffer[i][3]);
     	}
    for(i=0,j=0;j<SIZE_CONVERT;i++){ //二维数组转化为一维数组
    	buffer_1[j++]=buffer[i][0];
    	buffer_1[j++]=buffer[i][1];
    	buffer_1[j++]=buffer[i][2];
    	buffer_1[j++]=buffer[i][3];
    	//Uart_Printf("buffer before write %x\t%x\t%x\t%x\n",buffer[i][0],buffer[i][1],buffer[i][2],buffer[i][3]);
    	
    	}
        
    for(i=0;i<SIZE_CONVERT;i++){ 
       Wreeprom(EE_PAGE_ADDR,(U8)i,buffer_1[i]);//U32 slvAddr,U32 addr,U8 data
       //Uart_Printf("wr %x\t",buffer_1[i]);
       }
       //Uart_Printf("\n");
             
}

void Draw_cross(int x,int y,int color,int ch) //画出校正十字
{
	int i;
	for(i=-10;i<=10;i++){
	 		PutPixel(x+i,y,color );
 			PutPixel(x,y+i,color);
 			}
 	Lcd_PutASCII(x+5,y+5,ch,0,0,1);//输出标号
}


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
void __irq ADC_ISR(void) 
{ 
   DisableIrq(BIT_ADC); 
   DisableSubIrq(BIT_SUB_TC); 
   rADCTSC=(1<<3)|(1<<2);         //Pull-up disable, Seq. X,Y postion measure.
   rADCCON|=0x1;                   //start ADC 
  
   while(rADCCON & 0x1);    //check if Enable_start is low 
   while(!(rADCCON & 0x8000));        //check if EC(End of Conversion) flag is high, This line is necessary~!! 
   
   xdata=(0x3ff&rADCDAT0);
   ydata=(0x3ff&rADCDAT1);
   Uart_Printf("x=%d,y=%d\n",xdata,ydata);
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