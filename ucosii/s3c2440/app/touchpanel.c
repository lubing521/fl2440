#include "includes.h"

static void __irq ADC_ISR(void) ;
static void Draw_cross(int x,int y,int color,int ch);

INT32S TS_LCD_COEF[8];//触摸屏校正转换系数    
volatile INT32S TS_COORDINATE_X,TS_COORDINATE_Y;//触摸屏坐标输出
extern OS_EVENT *Touchpanel,*TS_Input; 


void TS_cal_init(void)
{
	 INT32S i,j;
	 INT8U ts_cal_flag;
	 INT8U buffer[SIZE_CONVERT];
	 INT8U key;
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
	 	TS_LCD_COEF[j]=(buffer[i]<<24)|(buffer[i+1]<<16)|(buffer[i+2]<<8)|(buffer[i+3]);
	 	i=i+4;
	 	//Uart_Printf("rbuffer %x\t%x\t%x\t%x\n",rbuffer[0],rbuffer[1],rbuffer[2],rbuffer[3]);
	 	//Uart_Printf("rresult[%d]=%d\n",j,TS_LCD_COEF[j]);
	 	}
	  Lcd_ClearScr(0x1111);
	  ClearSubPending(BIT_SUB_TC); 
    ClearSubPending(BIT_SUB_ADC); 
    ClearPending(BIT_ADC); 
    EnableSubIrq(BIT_SUB_TC); 
    EnableIrq(BIT_ADC);
         
    Uart_Printf( "Now touchpanel controler is initial!\n" ) ; 
}



void TS_cal(void) //触摸屏矫正
{
	INT32S i,j;
	INT32S xt[3],yt[3];
 	INT8U buffer_1[SIZE_CONVERT];
 	INT8U buffer[SIZE][4];
 	INT8U err;
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
		OSSemPend(Touchpanel,0,&err);
		/*switch(err){
			case OS_NO_ERR:
				Uart_Printf("OS_NO_ERR\n");
				break;
			case OS_ERR_PEND_ISR:
				Uart_Printf("OS_ERR_PEND_ISR\n");
				break;
			case OS_ERR_PEVENT_NULL:
				Uart_Printf("OS_ERR_PEVENT_NULL\n");
				break;
			case OS_ERR_EVENT_TYPE:
				Uart_Printf("OS_ERR_EVENT_TYPE\n");
				break;
			default:
				Uart_Printf("default\n");
				break;
			}
			*/
		xt[i]=TS_COORDINATE_X;
		yt[i]=TS_COORDINATE_Y;
		//OSSemPost(Touchpanel);
		//Uart_Printf("Point %d <%d,%d>\n",i+1,xt[i],yt[i]);
		}
	//坐标转换	
	 	 TS_LCD_COEF[0]=EEFLAG;
	 	 TS_LCD_COEF[1]=(xt[0]-xt[2])*(yt[1]-yt[2])-(xt[1]-xt[2])*(yt[0]-yt[2]);
     TS_LCD_COEF[2]=(XD0-XD2)*(yt[1]-yt[2])-(XD1-XD2)*(yt[0]-yt[2]);
     TS_LCD_COEF[3]=(xt[0]-xt[2])*(XD1-XD2)-(XD0-XD2)*(xt[1]-xt[2]);
     TS_LCD_COEF[4]=yt[0]*(xt[2]*XD1-xt[1]*XD2)+yt[1]*(xt[0]*XD2-xt[2]*XD0)+yt[2]*(xt[1]*XD0-xt[0]*XD1);
     TS_LCD_COEF[5]=(YD0-YD2)*(yt[1]-yt[2])-(YD1-YD2)*(yt[0]-yt[2]);
     TS_LCD_COEF[6]=(xt[0]-xt[2])*(YD1-YD2)-(YD0-YD2)*(xt[1]-xt[2]);
     TS_LCD_COEF[7]=yt[0]*(xt[2]*YD1-xt[1]*YD2)+yt[1]*(xt[0]*YD2-xt[2]*YD0)+yt[2]*(xt[1]*YD0-xt[0]*YD1);
			
			/*for(i=0;i<8;i++){
				Uart_Printf("TS_LCD_COEF[%d]=%d\n",i,TS_LCD_COEF[i]);
			}
			*/
     
    
	  for(i=0;i<SIZE;i++){
		  buffer[i][0]=(unsigned char)((TS_LCD_COEF[i]&0xFF000000)>>24);
     	buffer[i][1]=(unsigned char)((TS_LCD_COEF[i]&0x00FF0000)>>16);
     	buffer[i][2]=(unsigned char)((TS_LCD_COEF[i]&0x0000FF00)>>8);
     	buffer[i][3]=(unsigned char)(TS_LCD_COEF[i]&0x000000FF);
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

static void Draw_cross(int x,int y,int color,int ch) //画出校正十字
{
	INT32S i;
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
    /*
    ClearSubPending(BIT_SUB_TC); 
    ClearSubPending(BIT_SUB_ADC); 
    ClearPending(BIT_ADC); 
    EnableSubIrq(BIT_SUB_TC); 
    EnableIrq(BIT_ADC);
    */
} 

static void __irq ADC_ISR(void) 
{ 
   OSIntEnter();
   DisableIrq(BIT_ADC); 
   DisableSubIrq(BIT_SUB_TC); 
   rADCTSC=(1<<3)|(1<<2);         //Pull-up disable, Seq. X,Y postion measure.
   rADCCON|=0x1;                   //start ADC 
  
   while(rADCCON & 0x1);    //check if Enable_start is low 
   while(!(rADCCON & 0x8000));        //check if EC(End of Conversion) flag is high, This line is necessary~!! 
   
   TS_COORDINATE_X=(0x3ff&rADCDAT0);
   TS_COORDINATE_Y=(0x3ff&rADCDAT1);
   //Uart_Printf("x=%d,y=%d\n",TS_COORDINATE_X,TS_COORDINATE_Y);
	
   	
   OSSemPost(TS_Input);
   OSSemPost(Touchpanel);
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
   OSIntExit();
}