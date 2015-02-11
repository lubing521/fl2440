/*
*********************************************************************************************************
* 文件: Main.C
* 描述: 主程序文件.
* 编写: 
*********************************************************************************************************
*/
#define	GLOBAL_CLK		1

#include  "Includes.H"

//Task_Text Prio
#define NormalTaskPrio       5
#define MainTaskPrio 	NormalTaskPrio+11
#define Taskk_BK_Color_Prio 		NormalTaskPrio+10
#define Task_TS_ICC_Prio  		NormalTaskPrio+2
#define Task_BK_Bmp1_Prio  		NormalTaskPrio+3
#define Task_BK_Bmp1_Prio  		NormalTaskPrio+4
#define Task_LED_Prio  	NormalTaskPrio+5

#define	MainTaskStkLengh	1024*2   	// Define the MainTask stack length 
#define	TaskStkLeath		1024     	// Define the Task0 stack length 

#define LCD_X	320
#define LCD_Y	240

#define CH_X	LCD_X/5
#define CH_X_M	CH_X-CH_WIDTH
#define CH_Y_M	LCD_Y/4
#define CH_Y_1	CH_Y_M+3*CH_HIGH
#define CH_Y_2	CH_Y_1+3*CH_HIGH




OS_STK Task_Start_Stk[TaskStkLeath];
OS_STK Task_BK_Color_Stk[TaskStkLeath];
OS_STK Task_TS_ICC_Stk[TaskStkLeath];
OS_STK Task_BK_Bmp1_Stk[TaskStkLeath];
OS_STK Task_BK_Bmp2_Stk[TaskStkLeath];
OS_STK Task_LED_Stk[TaskStkLeath];


unsigned int Stack4[1024];
/* semaphores event control blocks */
OS_EVENT  *Touchpanel,*Convert,*Key0,*Key2,*TS_Input;
OS_TCB  *ptcb;

extern INT32S TS_LCD_COEF[8];
extern volatile INT32S TS_COORDINATE_X,TS_COORDINATE_Y;
volatile INT32S COORDINATE_X,COORDINATE_Y;
//extern U8 Image_house[];
extern U8 TE2440[];
extern const U8 slamdunk1[];
extern const U8 slamdunk2[];
extern Init_Timer4(void);
extern key_Eint(void);

__inline void cpu_init(void);

void 		Task_Start (void *pdata);
void 		Task_BK_Color(void *pdata);			    
void 		Task_TS_ICC(void *pdata);			   
void 		Task_BK_Bmp1(void *pdata);			    
void 		Task_BK_Bmp2(void *pdata);
void 		Task_LED(void *pdata);





/*
********************************************************************************************************
* 函数: int main (void).
* 描述: 主程序函数.
********************************************************************************************************
*/
void Main (void)
{
    cpu_init();
    Uart_Init(0,115200);  //串口初始化 波特率为115200
    Uart_Select(0);  //选者串口0
    
    Uart_Printf("start!\n");
    
    FL_Lcd_Init();//初始化LCD
    
    OSInit();
    OSTimeSet(0);
    
    /* 
     * 创建互斥信号量
     */
    Touchpanel = OSSemCreate(0);
    Convert=OSSemCreate(0);
    Key0=OSSemCreate(0);    
    Key2=OSSemCreate(0);
    TS_Input=OSSemCreate(0);
    /* create the start task */
    OSTaskCreate(Task_Start, 0, &Task_Start_Stk[TaskStkLeath - 1], MainTaskPrio);
    Touch_Screen_Init();
    IIC_Init();
    
    OSStart();
}

/*
********************************************************************************************************
* 函数: .
* 描述: .
********************************************************************************************************
*/

void Task_Start (void *i)
{
  
    /*使系统开始计时   */   
     
    Init_Timer4();
    OSStatInit();
  
    OSTaskCreate(Task_BK_Color, 0, &Task_BK_Color_Stk[TaskStkLeath - 1], Taskk_BK_Color_Prio);
    OSTaskCreate(Task_TS_ICC, 0, &Task_TS_ICC_Stk[TaskStkLeath - 1], Task_TS_ICC_Prio);
    OSTaskCreate(Task_BK_Bmp1, 0, &Task_BK_Bmp1_Stk[TaskStkLeath - 1], Task_BK_Bmp1_Prio);
    OSTaskCreate(Task_BK_Bmp2, 0, &Task_BK_Bmp2_Stk[TaskStkLeath - 1], Task_BK_Bmp2_Prio);


    // Delete current task
    OSTaskDel(OS_PRIO_SELF);
    Uart_Printf("Exit TaskStart.\n");
}

void Task_BK_Color(void *pdata)
{
   INT8U Reply;
   INT32S x,y;
   OSTaskCreate(Task_LED, 0, &Task_LED_Stk[TaskStkLeath - 1], Task_LED_Prio);
   OSSemPend(Convert,0, &Reply);
    for (;;)
    {
        
         Lcd_printf(CH_X_M,CH_Y_M,0,0,1,"请选择背景颜色：\n");
         Lcd_printf(CH_X,CH_Y_1,0,0,1,"1.蓝色\n");
         Lcd_printf(CH_X,CH_Y_2,0,0,1,"2.红色\n");
         //Uart_Printf("OSCPUUsage=%f,OSIdleCtr=%d\n",OSCPUUsage,OSIdleCtr);
         //Uart_Printf("OSIdleCtr=%d\n",OSIdleCtr);
        OSSemPend(Touchpanel,0,&Reply);
        
        x=COORDINATE_X;	
 		y=COORDINATE_Y;	
        OS_ENTER_CRITICAL();
        Uart_Printf("\n    Task1  "); 
        Uart_Printf("xm=%d,ym=%d\n",x,y);
        //Uart_Printf("rec %s\n",ss);
        /* wait a short while */
         OS_EXIT_CRITICAL();
         if(x>CH_X && x<CH_X+3*CH_WIDTH && y>CH_Y_1 && y<CH_Y_1+CH_HIGH){
         	Lcd_ClearScr(0x1111);
         	//Lcd_printf(50,50,0,0,1,"CPU Usage: %6.1f% \n",OSCPUUsage*100);
         	//OSTaskCreate(TaskDrawR, 0, &TaskDrawRStk[TaskStkLeath - 1], 11);
         	OSTimeDlyHMSM(0,0,0,100);
        }
         if(x>CH_X && x<CH_X+3*CH_WIDTH && y>CH_Y_2 && y<CH_Y_2+CH_HIGH){
         	OSTaskDelReq(11);
         	OSTimeDlyHMSM(0,0,0,10);
         	Lcd_ClearScr(0x8000);
        }
         
    	//OSTimeDly(500);
    	// OSMutexPost(Sem1);
    }
}

/*
********************************************************************************************************
* 函数: .
* 描述: .
********************************************************************************************************
*/


void Task_TS_ICC (void *pdata) //初始化触摸屏，校正，坐标转换
{
     U8 err;
     Uart_Printf("***** Calibrate TS ****** \n");
    
     TS_cal_init();
     OSSemPost(Convert);
     
  	 while(1){
  	 	OSSemPend(TS_Input,0,&err);
  	 	COORDINATE_X=(TS_LCD_COEF[2]*TS_COORDINATE_X+TS_LCD_COEF[3]*TS_COORDINATE_Y+TS_LCD_COEF[4])/TS_LCD_COEF[1];	
   		COORDINATE_Y=(TS_LCD_COEF[5]*TS_COORDINATE_X+TS_LCD_COEF[6]*TS_COORDINATE_Y+TS_LCD_COEF[7])/TS_LCD_COEF[1];
   		}
    
}

/*
********************************************************************************************************
* End.
********************************************************************************************************
*/

void Task_BK_Bmp1 (void *pdata)
{
	U8 err;
	//OSSemPend(Convert);
	while(1){
		//Uart_Printf("\n========= Task3 ==========\n");
		
		OSSemPend(Key0,0,&err);
		Lcd_ClearScr(0xffff);
		Paint_Bmp(0,0,LCD_X,LCD_Y,slamdunk1);
		//OSTimeDly(500);
		
		//Uart_Printf("\n========= LED ==========\n");
	}
}

void Task_BK_Bmp2 (void *pdata)
{
	U8 err;
	while(1){
		
		OSSemPend(Key2,0,&err);
		Lcd_ClearScr(0xffff);
		Paint_Bmp(0,0,LCD_X,LCD_Y,slamdunk2);
		
	}

}

void Task_LED(void *pdata)
{
	while(1){
		Led_On();
		OSTimeDly(3000);
		Led_Off();
		OSTimeDly(3000);
		}
}

__inline void cpu_init(void)
{
	
    int i;
    U8 key;
	U32 mpll_val = 0 ;
    
    Port_Init();  //端口初始化
    Isr_Init();   //中断初始化
    key_Eint();
    i = 2 ;	//don't use 100M!
		//boot_params.cpu_clk.val = 3;
	switch ( i ) {
	case 0:	//200
		key = 12;
		mpll_val = (92<<12)|(4<<4)|(1);
		break;
	case 1:	//300
		key = 13;
		mpll_val = (67<<12)|(1<<4)|(1);
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