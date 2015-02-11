#define	GLOBAL_CLK		1

//Task_Text Prio
#define NormalTaskPrio       5
#define MainTaskPrio 	NormalTaskPrio
#define Task0Prio 		NormalTaskPrio+1
#define Task1Prio  		NormalTaskPrio+2
#define Task2Prio  		NormalTaskPrio+3
#define TaskUartPrio  	NormalTaskPrio+4

#define	MainTaskStkLengh	1024*2   	// Define the MainTask stack length 
#define	TaskStkLeath		1024     	// Define the Task0 stack length 

//#include "GUI.h"
#include "includes.h"
#include "LCDConf.h"
//#include "2440addr.h"
//#include "2440lib.h"
//#include "2440slib.h"
//#include "Target.h"
//#include "Def.h"
//#include "option.h"
//#include "uc_os.h"
//#include "app_cfg.h"
//#include "FL2440_LCD_for_GUI.h"

volatile float speed=1;

OS_STK Task_Text_Stk[TaskStkLeath];
OS_STK Task_Title_Stk[TaskStkLeath];
OS_STK Task_SideText_Stk[TaskStkLeath];
OS_STK Task_Uart_Stk[TaskStkLeath];
OS_STK TaskStartSTK[TaskStkLeath];
OS_STK Task_Info_Stk[TaskStkLeath];
OS_STK Task_Bmp_Stk[TaskStkLeath];
OS_STK Task_LCD_Stk[TaskStkLeath];

extern const GUI_BITMAP bmslamdunk1_tiny;
extern const GUI_BITMAP bmslamdunk2_tiny;

extern void GUITASK_Init(void);
extern void  Init_Timer4(void);
extern void key_Eint(void);

OS_EVENT *Cls;
void 		Task_Text(void *pdata);			    // Task_Text 
void 		Task_Uart(void *pdata);			    // Task_Uart 
void 		Task_SideText(void *pdata);			    // Task_SideText 
void 		Task_Title(void *pdata);
void 		Task_Info(void *pdata);
void 		Task_Bmp(void *pdata);
void 		Task_LCD(void *pdata);


void TaskStart (void *pdata);
__inline void cpu_init(void);

void Main()
{
	
	cpu_init();
	FL_Lcd_Init();
	Uart_Init(0,115200);
	Uart_Printf("begin!\n");
	GUI_Init();
	GUITASK_Init();
	
	//Task_SideText(NULL);
	
	OSInit();
    OSTimeSet(0);
   
    OSTaskCreate(TaskStart, 0, &TaskStartSTK[TaskStkLeath-1], NormalTaskPrio+13);
    Cls=OSSemCreate(1); 
    
    OSStart();
	
		
	
}

void TaskStart (void *pdata)
{
   
    Init_Timer4();
    
    OSStatInit();
    //Uart_Printf("OSStatInit complete\n");
   
   	
   	//OSTaskCreate(Task_Text,0, &Task_Text_Stk[TaskStkLeath - 1], NormalTaskPrio);
   	 
  	
    //OSTaskCreate(Task_SideText, 0, &Task_SideText_Stk[TaskStkLeath - 1], NormalTaskPrio+3);
    
    //OSTaskCreate(Task_Info, 0, &Task_Info_Stk[TaskStkLeath - 1], NormalTaskPrio+5);
    
    //OSTaskCreate(Task_Title, 0, &Task_Title_Stk[TaskStkLeath - 1], NormalTaskPrio+6);
     
   // OSTaskCreate(Task_Uart, 0, &Task_Uart_Stk[TaskStkLeath - 1], NormalTaskPrio+8);
   //OSTaskCreate(Task_Bmp, 0, &Task_Bmp_Stk[TaskStkLeath - 1], NormalTaskPrio+7);
   OSTaskCreate(Task_LCD, 0, &Task_LCD_Stk[TaskStkLeath - 1], NormalTaskPrio+7);
    
   
   


    // Delete current task
    OSTaskDel(OS_PRIO_SELF);
    Uart_Printf("Exit TaskStart.\n");
}

#define LCD_MEM_X 0
#define LCD_MEM_Y 0


#define REBX 5
#define REBY 3
#define WIDTH 96
#define HIGH 72
#define TEXT_SIZE_Y	100
#define TEXT_SIZE_X	200
#define TEXT_X	100
#define	TEXT_Y	40
#define CH_WIDTH 10 //一个汉字的宽度
#define CH_HIGH 10 //一个汉字的高度

//侧边文本框原点及大小
#define SIDE_X 0
#define SIDE_Y 30
#define SIDE_SIZE_X 96
#define SIDE_SIZE_Y 170

#define SIDE_TEXT_STEP 10
#define SIDE_TEXT_STEP_ZOOM 5

#define TEXT_SPEED 300


#define TITLE_X 100
#define TITLE_Y 20
#define TITLE_SIZE_X 200
#define TITLE_SIZE_Y 10
#define TITLE_TEXT_STEP 5
#define TITLE_SPEED 200

#define INFO_X 100
#define INFO_Y 150
#define INFO_SIZE_X 200
#define INFO_SIZE_Y 90
#define INFO_STEP 10


#define BMP_SIZE_X 96
#define BMP_SIZE_Y 72
#define BMP_MEM_SIZE_X 198
#define BMP_MEM_SIZE_Y 98
#define BMP_MEM_X 102
#define BMP_MEM_Y 42
#define BMP_STEP_X 5
#define BMP_STEP_Y 3

void Task_LCD(void *pdata)
{
	GUI_MEMDEV_Handle lcd_mem;
	S16 bmp_x,bmp_y,Titlei,Systemi;
	int reb_x=BMP_STEP_X,reb_y=BMP_STEP_Y;

	Titlei=TITLE_X+TITLE_SIZE_X;
	Systemi=SIDE_Y+SIDE_SIZE_Y;
	bmp_x=BMP_MEM_X+1;
	bmp_y=BMP_MEM_Y+1;
	lcd_mem=GUI_MEMDEV_Create(LCD_MEM_X,LCD_MEM_Y,LCD_XSIZE,LCD_YSIZE);
	GUI_MEMDEV_Select(lcd_mem);
	
	while(1){
	
		GUI_Clear();
		//Draw Rect
		GUI_ClearRect(TEXT_X,TEXT_Y,TEXT_X+TEXT_SIZE_X,TEXT_Y+TEXT_SIZE_Y);
		
		GUI_DrawHLine(TEXT_Y,TEXT_X,TEXT_X+TEXT_SIZE_X);
		GUI_DrawHLine(TEXT_Y+TEXT_SIZE_Y,TEXT_X,TEXT_X+TEXT_SIZE_X);
		GUI_DrawVLine(TEXT_X,TEXT_Y,TEXT_Y+TEXT_SIZE_Y);
		GUI_DrawVLine(TEXT_X+TEXT_SIZE_X,TEXT_Y,TEXT_Y+TEXT_SIZE_Y);
		
		//System Info
		GUI_DispStringAt("uc/OS_II",SIDE_X,Systemi);
		GUI_DispStringAt("uc/GUI",SIDE_X,Systemi+SIDE_TEXT_STEP);
		GUI_DispStringAt("Version:",SIDE_X,Systemi+SIDE_TEXT_STEP*2);
		GUI_DispDecMin(OSVersion());
		
		Systemi=(S16)(Systemi-SIDE_TEXT_STEP/SIDE_TEXT_STEP_ZOOM);
			//Uart_Printf("i=%d\n",i);
		if(Systemi<=10){
			Systemi=SIDE_Y+SIDE_SIZE_Y;
			//Uart_Printf("Caution ! i=200\n");
		}
		
		//
		GUI_DispStringAt("Uart Test,Please Input Characters!",Titlei,20);
		Titlei=Titlei-TITLE_TEXT_STEP;
		if(Titlei<=0)
			Titlei=TITLE_X+TITLE_SIZE_X;
		
		
		//Static Info
		GUI_DispStringAt("Static informaion:",INFO_X,INFO_Y);
		GUI_DispStringAt("CPU Usage:",INFO_X,INFO_Y+INFO_STEP);
		GUI_DispDecMin(OSCPUUsage);
		GUI_DispString("%");
		GUI_DispStringAt("Task Switch Times:",INFO_X,INFO_Y+INFO_STEP*2);
		GUI_DispDecMin(OSTaskCtr);
		GUI_DispString("/s");
		
		//Ball
		//
		GUI_DrawBitmap(&bmslamdunk1_tiny,bmp_x,bmp_y);
		//GUI_FillCircle(bmp_x,bmp_y,2);
		bmp_x+=reb_x;
		bmp_y+=reb_y;
		Uart_Printf("x=%d\t",bmp_x);
		Uart_Printf("reb_x=%d\t",reb_x);
		Uart_Printf("y=%d\t",bmp_y);
		Uart_Printf("reb_y=%d\n",reb_y);
		if((bmp_x+BMP_SIZE_X)>=(BMP_MEM_X+BMP_MEM_SIZE_X)){
			reb_x=-BMP_STEP_X;
			bmp_x+=reb_x;
			bmp_x+=reb_x;
			Uart_Printf("reb_x=%d\n",reb_x);
			}
		if(bmp_x<=BMP_MEM_X){
			reb_x=BMP_STEP_X;
			bmp_x+=reb_x;
			bmp_x+=reb_x;
			}
		if((bmp_y+BMP_SIZE_Y)>=(BMP_MEM_Y+BMP_MEM_SIZE_Y)){
			reb_y=-BMP_STEP_Y;
			bmp_y+=reb_y;
			bmp_y+=reb_y;
			Uart_Printf("reb_y=%d\n",reb_y);
			}
		if(bmp_y<=BMP_MEM_Y){
			reb_y=BMP_STEP_Y;
			bmp_y+=reb_y;
			bmp_y+=reb_y;
			}
			
		GUI_MEMDEV_CopyToLCD(lcd_mem);
		OSTimeDlyHMSM(0,0,0,100);	
		}
			
		
}



__inline void cpu_init(void)
{
	U16 i;
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



