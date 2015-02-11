#define	GLOBAL_CLK		1

#include  "Includes.H"

//任务优先级定义
#define NormalTaskPrio       5
#define MainTaskPrio 	NormalTaskPrio+11
#define Taskk_BK_Color_Prio 		NormalTaskPrio+10
#define Task_TS_ICC_Prio  		NormalTaskPrio+2
#define Task_BK_Bmp1_Prio  		NormalTaskPrio+3
#define Task_BK_Bmp2_Prio  		NormalTaskPrio+4
#define Task_LED_Prio  	NormalTaskPrio+5

//任务堆栈长度定义
#define	TaskStkLeath		1024     	

//LCD参数及显示菜单位置常量定义
#define LCD_X	LCD_XSIZE_TFT_240320
#define LCD_Y	LCD_YSIZE_TFT_240320

#define CH_X	LCD_X/5
#define CH_X_M	CH_X-CH_WIDTH
#define CH_Y_M	LCD_Y/4
#define CH_Y_1	CH_Y_M+3*CH_HIGH
#define CH_Y_2	CH_Y_1+3*CH_HIGH



//任务堆栈定义
OS_STK Task_Start_Stk[TaskStkLeath];
OS_STK Task_BK_Color_Stk[TaskStkLeath];
OS_STK Task_TS_ICC_Stk[TaskStkLeath];
OS_STK Task_BK_Bmp1_Stk[TaskStkLeath];
OS_STK Task_BK_Bmp2_Stk[TaskStkLeath];
OS_STK Task_LED_Stk[TaskStkLeath];


//信号量定义
OS_EVENT  *Touchpanel,*Convert,*Key0,*Key2,*TS_Input;
OS_TCB  *ptcb;

//触摸屏坐标与LCD坐标转换系数
extern INT32S TS_LCD_COEF[8];
//触摸屏坐标输出
extern volatile INT32S TS_COORDINATE_X,TS_COORDINATE_Y;
//转换后坐标输出
volatile INT32S COORDINATE_X,COORDINATE_Y;

//BMP图像数据
extern const U8 slamdunk1[];
extern const U8 slamdunk2[];
//时钟初始化
extern Init_Timer4(void);
//按键中断初始化
extern key_Eint(void);

__inline void cpu_init(void);

void 		Task_Start (void *pdata);			//起始任务，创建其他任务
void 		Task_BK_Color(void *pdata);			//背景颜色切换任务
void 		Task_TS_ICC(void *pdata);			//触摸屏校正任务，将转换系数存入eeprom
void 		Task_BK_Bmp1(void *pdata);			//显示背景图片1任务
void 		Task_BK_Bmp2(void *pdata);			//显示背景图片2任务
void 		Task_LED(void *pdata);				//LED闪烁任务


void Main (void)
{

	//cpu初始化，设置时钟频率等
    cpu_init();
    Uart_Init(0,115200);  //串口初始化 波特率为115200
    Uart_Select(0);  //选者串口0
    
    Uart_Printf("uC/OS Start!\n");
    
    FL_Lcd_Init();//初始化LCD
    //ucos操作系统初始化
    OSInit();
    //操作系统时钟初始化
    OSTimeSet(0);
     
    //创建互斥信号量
    Touchpanel = OSSemCreate(0);
    Convert=OSSemCreate(0);
    Key0=OSSemCreate(0);    
    Key2=OSSemCreate(0);
    TS_Input=OSSemCreate(0);
    //创建起始任务
    OSTaskCreate(Task_Start, 0, &Task_Start_Stk[TaskStkLeath - 1], MainTaskPrio);
    //触摸屏初始化
    Touch_Screen_Init();
    //IIC总线初始化
    IIC_Init();
    //启动任务调度
    OSStart();
}

//起始任务

void Task_Start (void *i)
{
  
    /*使系统开始计时   */   
     
    Init_Timer4();
    OSStatInit();
  
    OSTaskCreate(Task_BK_Color, 0, &Task_BK_Color_Stk[TaskStkLeath - 1], Taskk_BK_Color_Prio);
    OSTaskCreate(Task_TS_ICC, 0, &Task_TS_ICC_Stk[TaskStkLeath - 1], Task_TS_ICC_Prio);
    

    //退出任务
    OSTaskDel(OS_PRIO_SELF);
    Uart_Printf("Exit TaskStart.\n");
}

void Task_BK_Color(void *pdata)
{
   INT8U Reply;
   INT32S x,y;
  
   OSSemPend(Convert,0, &Reply);
   OSTaskCreate(Task_BK_Bmp1, 0, &Task_BK_Bmp1_Stk[TaskStkLeath - 1], Task_BK_Bmp1_Prio);
   OSTaskCreate(Task_BK_Bmp2, 0, &Task_BK_Bmp2_Stk[TaskStkLeath - 1], Task_BK_Bmp2_Prio);
   OSTaskCreate(Task_LED, 0, &Task_LED_Stk[TaskStkLeath - 1], Task_LED_Prio);
    for (;;)
    {
        
         Lcd_printf(CH_X_M,CH_Y_M,0,0,1,"请选择背景颜色：\n");
         Lcd_printf(CH_X,CH_Y_1,0,0,1,"1.蓝色\n");
         Lcd_printf(CH_X,CH_Y_2,0,0,1,"2.红色\n");
        //请求信号量，等待触摸屏输入中断
        OSSemPend(Touchpanel,0,&Reply);
        
        x=COORDINATE_X;	
 		y=COORDINATE_Y;	
        OS_ENTER_CRITICAL();
        Uart_Printf("Task_BK_Color  "); 
        //在调试主机端显示输入点坐标
        Uart_Printf("x_lcd=%d,y_lcd=%d\n",x,y);
       
         OS_EXIT_CRITICAL();
         
         
         //判断触摸屏输入位置对应的菜单项
         if(x>CH_X && x<CH_X+3*CH_WIDTH && y>CH_Y_1 && y<CH_Y_1+CH_HIGH){
         	//背景色置蓝色
         	Lcd_ClearScr(0x1111);
         	
         	OSTimeDlyHMSM(0,0,0,100);
        }
         if(x>CH_X && x<CH_X+3*CH_WIDTH && y>CH_Y_2 && y<CH_Y_2+CH_HIGH){
     
         	OSTimeDlyHMSM(0,0,0,10);
         	//背景色置红色
         	Lcd_ClearScr(0x8000);
        }
         
    	
    }
}



void Task_TS_ICC (void *pdata) //初始化触摸屏，校正，坐标转换
{
     U8 err;
     Uart_Printf("Task_TS_ICC\n");
     Uart_Printf("***** Calibrate TS ****** \n");
    
     TS_cal_init();
     OSSemPost(Convert);
     
  	 while(1){
  	 	OSSemPend(TS_Input,0,&err);
  	 	
  	 	//坐标转换
  	 	COORDINATE_X=(TS_LCD_COEF[2]*TS_COORDINATE_X+TS_LCD_COEF[3]*TS_COORDINATE_Y+TS_LCD_COEF[4])/TS_LCD_COEF[1];	
   		COORDINATE_Y=(TS_LCD_COEF[5]*TS_COORDINATE_X+TS_LCD_COEF[6]*TS_COORDINATE_Y+TS_LCD_COEF[7])/TS_LCD_COEF[1];
   		}
    
}



void Task_BK_Bmp1 (void *pdata)
{
	U8 err;
	
	while(1){
		Uart_Printf("Task_BK_Bmp1\n");
		//请求信号量，等待中断，任务挂起
		OSSemPend(Key0,0,&err);
		Lcd_ClearScr(0xffff);
		//显示位图
		Paint_Bmp(0,0,LCD_X,LCD_Y,slamdunk1);
		
	}
}

void Task_BK_Bmp2 (void *pdata)
{
	U8 err;
	while(1){
		Uart_Printf("Task_BK_Bmp2\n");
		//请求信号量，等待中断，任务挂起
		OSSemPend(Key2,0,&err);
		Lcd_ClearScr(0xffff);
		//显示位图
		Paint_Bmp(0,0,LCD_X,LCD_Y,slamdunk2);
		
	}

}

void Task_LED(void *pdata)
{
	while(1){
		Uart_Printf("Task_LED\n");
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