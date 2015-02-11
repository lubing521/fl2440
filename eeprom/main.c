#define GLOBAL_CLK 1
#define MCLK	50000000
#define XD0	50
#define YD0	50
#define XD1	300
#define	YD1	150
#define	XD2	150
#define	YD2 200
#define	EE_PAGE_ADDR	0xa0
#define EEFLAG	0x6a
#define	SIZE	8
#define	SIZE_CONVERT	32


#define COLOR_MAX	 3		//颜色种类
#include "2440lib.h"
#include "2440addr.h"
#include "def.h"
#include "option.h"
#include "2440slib.h"
#include "lcd.h"
#include "iic.h"

void cpu_init(void);
void TS_cal_init(void);

void TS_cal(void);


int color[COLOR_MAX]={0x1111,0x8000,0x1814a};
volatile int xdata,ydata;
volatile int flagTS=0;
int	a[6]={100,200,300,400,500,600};
int k=1000;
int coef[8];
void Main(void) 
{ 
 	//int Xlcd,Ylcd;
 	int i=0;
 	
 	
 	cpu_init();
	Uart_Init(MCLK,115200); 
 	Uart_Printf("the main is running\n");    
  	iic_init();
  	TS_cal_init();
  	Uart_Printf("Write to eeprom!\n");
  	
  	
  	Uart_Printf("Write to eeprom done!\n");	
  	Uart_Printf("Read from eeprom!\n");
  	
  	for(i=0;i<6;i++){
  		Uart_Printf("%d\t",coef[i]);
  		}
 	
}



 
void TS_cal_init(void)
{
	 int i,j;
	 U8 ts_cal_flag;
	 U8 buffer[SIZE_CONVERT];
 	 //S32 coef[SIZE];
	 Rdeeprom(EE_PAGE_ADDR,(U8)0,&ts_cal_flag);
	 Uart_Printf("ts_cal_flag=%d\n",ts_cal_flag);
	 if(ts_cal_flag!=0x6A){
	 		TS_cal();
	 	}
	 for(i=0;i<SIZE_CONVERT;i++){
	 		Rdeeprom(EE_PAGE_ADDR,(U8)i,&(buffer[i]));
	 		Uart_Printf("rd %x\t",buffer[i]);
	 
	 }
	  Uart_Printf("\n");
	 for(i=0,j=0;j<SIZE;j++){
	 	coef[j]=(buffer[i]<<24)|(buffer[i+1]<<16)|(buffer[i+2]<<8)|(buffer[i+3]);
	 	i=i+4;
	 	//Uart_Printf("rbuffer %x\t%x\t%x\t%x\n",rbuffer[0],rbuffer[1],rbuffer[2],rbuffer[3]);
	 	Uart_Printf("rresult[%d]=%d\n",j,coef[j]);
	 	}
	 
}



void TS_cal(void) //触摸屏矫正
{
	S32 i,j;
	S32 xt[3],yt[3];
 	//S32 coef[SIZE];
 	U8 buffer_1[SIZE_CONVERT];
 	U8 buffer[SIZE][4];
 	 	
	//Draw_cross(XD0,YD0,0,'1');
	//Draw_cross(XD1,YD1,0,'2');
	//Draw_cross(XD2,YD2,0,'3');
	Uart_Printf("click the cross!\n");
	/*for(i=0;i<3;i++){
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
     */
     coef[0]=EEFLAG;
     coef[1]=k;
     for(i=2;i<8;i++)
     	coef[i]=a[i-2];
     
    
	  for(i=0;i<SIZE;i++){
		  buffer[i][0]=(unsigned char)((coef[i]&0xFF000000)>>24);
     	buffer[i][1]=(unsigned char)((coef[i]&0x00FF0000)>>16);
     	buffer[i][2]=(unsigned char)((coef[i]&0x0000FF00)>>8);
     	buffer[i][3]=(unsigned char)(coef[i]&0x000000FF);
     	Uart_Printf("buffer %x\t%x\t%x\t%x\n",buffer[i][0],buffer[i][1],buffer[i][2],buffer[i][3]);
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
       Uart_Printf("wr %x\t",buffer_1[i]);
       }
       Uart_Printf("\n");
             
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

