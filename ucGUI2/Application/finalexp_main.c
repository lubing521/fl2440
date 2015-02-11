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
   
   	
   	OSTaskCreate(Task_Text,0, &Task_Text_Stk[TaskStkLeath - 1], NormalTaskPrio);
   	 
  	
    OSTaskCreate(Task_SideText, 0, &Task_SideText_Stk[TaskStkLeath - 1], NormalTaskPrio+3);
    
    OSTaskCreate(Task_Info, 0, &Task_Info_Stk[TaskStkLeath - 1], NormalTaskPrio+5);
    
    OSTaskCreate(Task_Title, 0, &Task_Title_Stk[TaskStkLeath - 1], NormalTaskPrio+6);
     
    OSTaskCreate(Task_Uart, 0, &Task_Uart_Stk[TaskStkLeath - 1], NormalTaskPrio+8);
   //OSTaskCreate(Task_Bmp, 0, &Task_Bmp_Stk[TaskStkLeath - 1], NormalTaskPrio+7);
    
   
   


    // Delete current task
    OSTaskDel(OS_PRIO_SELF);
    Uart_Printf("Exit TaskStart.\n");
}

#define REBX 5
#define REBY 3
#define WIDTH 96
#define HIGH 72
#define TEXT_SIZE_Y	100
#define TEXT_SIZE_X	200
#define TEXT_X	100
#define	TEXT_Y	40
//#define CH_WIDTH 10 //一个汉字的宽度
//#define CH_HIGH 10 //一个汉字的高度

void Task_Text (void *pdata)
{
	U8 err;
	GUI_MEMDEV_Handle text_mem;
	text_mem=GUI_MEMDEV_Create(TEXT_X,TEXT_Y,TEXT_SIZE_X+10,TEXT_SIZE_Y+10);
	GUI_MEMDEV_Select(text_mem);
	while(1){
		
		OSSemPend(Cls,0,&err);
		Uart_Printf("CLS\n");
		//OS_ENTER_CRITICAL();

		//GUI_ClearRect(TEXT_X,TEXT_Y,TEXT_X+TEXT_SIZE_X,TEXT_Y+TEXT_SIZE_Y);
		GUI_Clear();
		GUI_DrawHLine(TEXT_Y,TEXT_X,TEXT_X+TEXT_SIZE_X);
		GUI_DrawHLine(TEXT_Y+TEXT_SIZE_Y,TEXT_X,TEXT_X+TEXT_SIZE_X);
		GUI_DrawVLine(TEXT_X,TEXT_Y,TEXT_Y+TEXT_SIZE_Y);
		GUI_DrawVLine(TEXT_X+TEXT_SIZE_X,TEXT_Y,TEXT_Y+TEXT_SIZE_Y);
		
		//GUI_DrawBitmap(&bmslamdunk1_tiny,i,j);
		//OS_EXIT_CRITICAL();
		//GUI_Delay(500);
		GUI_MEMDEV_CopyToLCD(text_mem);
		//OSSemPost(Disp);
		OSTimeDlyHMSM(0,0,0,200);
		
		
	}
		
}


void Task_Uart (void *pdata)
{
	
	U8 c=0;
	S16 char_x,char_y;
	S16 ch_width,ch_high;
	ch_width=GUI_GetCharDistX('a');
	ch_high=GUI_GetFontDistY();
	char_x=TEXT_X+ch_width;
	char_y=TEXT_Y+ch_high;
	//Uart_Printf("begin i,j=%d,%d\n",char_x,char_y);
	while(1){
		//while((c=Uart_Getch())!='\r')
		//Uart_Printf("in while i,j=%d,%d\n",char_x,char_y);
		
		c=Uart_GetKey();
		//GUI_DispCharAt(c,i,j);
		
		
		if(c=='\r'){	
			Uart_Printf("x,y=%d,%d\n",char_x,char_y);
			char_x=TEXT_X+ch_width;
			char_y=char_y+ch_high;
		
			if(char_y>=(TEXT_Y+TEXT_SIZE_Y)){
				char_y=TEXT_Y+ch_high;
				OSSemPost(Cls);
				//OSTimeDlyHMSM(0,0,0,10);
				//Uart_Printf("post 2.j=%d\n",char_x);
			}
		}
		else if(c!='\r' && c!=0)
		{
			//Uart_Printf("else i,j=%d,%d\n",char_x,char_y);
			//GUI_X_Lock();
			GUI_DispCharAt(c,char_x,char_y);
			//GUI_X_Unlock();
			char_x=char_x+ch_width;
			if(char_x>=(TEXT_X+TEXT_SIZE_X)){
				char_x=TEXT_X+ch_width;
				char_y=char_y+ch_high;				
				}
			if(char_y>=(TEXT_Y+TEXT_SIZE_Y-ch_high)){
				char_y=TEXT_Y+ch_high;;
				OSSemPost(Cls);
				//OSTimeDlyHMSM(0,0,0,10);
				//Uart_Printf("post 1.j=%d\n",char_y);
				}
			}
		
		OSTimeDlyHMSM(0,0,0,100);
		
		}

}

//侧边文本框原点及大小
#define SIDE_X 0
#define SIDE_Y 30
#define SIDE_SIZE_X 96
#define SIDE_SIZE_Y 170

#define SIDE_TEXT_STEP 10
#define SIDE_TEXT_STEP_ZOOM 5

#define TEXT_SPEED 300

void Task_SideText(void *pdata)
{
	S16 i=SIDE_Y+SIDE_SIZE_Y;
	S16 uCOS_Version;
	const GUI_FONT *oldfont;
	GUI_MEMDEV_Handle side_mem;
	side_mem=GUI_MEMDEV_Create(SIDE_X,SIDE_Y,SIDE_SIZE_X,SIDE_SIZE_Y);
	GUI_MEMDEV_Select(side_mem);
	
	uCOS_Version=OSVersion();
	
	//Uart_Printf("version=%d\n",uCOS_Version);
	
	while(1){	
	//GUI_ClearRect(SIDE_X,SIDE_Y,SIDE_X+SIDE_SIZE_X,SIDE_Y+SIDE_SIZE_Y);
	//GUI_DrawBitmap(&bmslamdunk1_tiny,0,0);
	
	//GUI_DrawBitmap(&bmslamdunk1_tiny,0,i);
	
	//GUI_X_Lock();
	GUI_Clear();
	oldfont=GUI_SetFont(&GUI_Font8x8);
	GUI_DispStringAt("uc/OS_II",SIDE_X,i);
	GUI_DispStringAt("uc/GUI",SIDE_X,i+SIDE_TEXT_STEP);
	GUI_DispStringAt("Version:",SIDE_X,i+SIDE_TEXT_STEP*2);
	//GUI_DispDecMin(OSCPUUsage);
	GUI_DispDecMin(uCOS_Version);
	
	
	i=(S16)(i-SIDE_TEXT_STEP/SIDE_TEXT_STEP_ZOOM);
	//Uart_Printf("i=%d\n",i);
	if(i<=10){
		i=SIDE_Y+SIDE_SIZE_Y;
		//Uart_Printf("Caution ! i=200\n");
		}
	
	GUI_MEMDEV_CopyToLCD(side_mem);
	
	//GUI_MEMDEV_Select(0);
	//GUI_X_Unlock();
	//GUI_MEMDEV_Delete(hMem);
	
	
	OSTimeDlyHMSM(0,0,0,TEXT_SPEED);
	
	}
}

#define TITLE_X 100
#define TITLE_Y 20
#define TITLE_SIZE_X 200
#define TITLE_SIZE_Y 10
#define TITLE_TEXT_STEP 5
#define TITLE_SPEED 200


void Task_Title(void *pdata)
{
	
	GUI_MEMDEV_Handle title_mem;
	S16 i=TITLE_X+TITLE_SIZE_X;
	title_mem=GUI_MEMDEV_Create(TITLE_X,TITLE_Y,TITLE_SIZE_X,TITLE_SIZE_Y);	
	GUI_MEMDEV_Select(title_mem);
	while(1){
	//Uart_Printf("Task_Title\n");
	//GUI_ClearRect(TITLE_X,TITLE_Y,TITLE_X+TITLE_SIZE_X,TITLE_Y+TITLE_SIZE_Y);
	//GUI_X_Lock();
	GUI_Clear();
	GUI_DispStringAt("Uart Test,Please Input Characters!",i,20);
	i=i-TITLE_TEXT_STEP;
	if(i<=0)
		i=TITLE_X+TITLE_SIZE_X;
	GUI_MEMDEV_CopyToLCD(title_mem);
	//GUI_X_Unlock();
	//GUI_MEMDEV_Delete(title_mem);
	
	OSTimeDlyHMSM(0,0,0,TITLE_SPEED);
	
	}
}

#define INFO_X 100
#define INFO_Y 150
#define INFO_SIZE_X 200
#define INFO_SIZE_Y 90
#define INFO_STEP 10

void Task_Info(void *pdata)
{
	GUI_MEMDEV_Handle info_mem;
	S32 Time,Seconds,Minutes,Hours;
	info_mem=GUI_MEMDEV_Create(INFO_X,INFO_Y,INFO_SIZE_X,INFO_SIZE_Y);
	GUI_MEMDEV_Select(info_mem);
	
	while(1){
		//Uart_Printf("Information!\n");
		GUI_Clear();
		//GUI_ClearRect(INFO_X,INFO_Y,INFO_X+INFO_SIZE_X,INFO_Y+INFO_SIZE_Y);
		GUI_DispStringAt("Static informaion:",INFO_X,INFO_Y);
		GUI_DispStringAt("CPU Usage:",INFO_X,INFO_Y+INFO_STEP);
		GUI_DispDecMin(OSCPUUsage);
		GUI_DispString("%");
		GUI_DispStringAt("Task Switch Times:",INFO_X,INFO_Y+INFO_STEP*2);
		GUI_DispDecMin(OSTaskCtr);
		GUI_DispString("/s");
		GUI_DispStringAt("System Time:",INFO_X,INFO_Y+INFO_STEP*3);
		Time=OSTimeGet()/OS_TICKS_PER_SEC;
		Hours=(Time/3600)%60;
		Minutes=(Time/60)%60;
		Seconds=Time % 60;
		
		GUI_DispDec(Hours,2);
		GUI_DispString(":");
		GUI_DispDec(Minutes,2);
		GUI_DispString(":");
		GUI_DispDec(Seconds,2);
		
		
		
		//GUI_DispString();
		GUI_MEMDEV_CopyToLCD(info_mem);
		
		OSTimeDlyHMSM(0,0,1,0);
		
	}
	
}

#define BMP_SIZE_X 2
#define BMP_SIZE_Y 2
#define BMP_MEM_SIZE_X 198
#define BMP_MEM_SIZE_Y 98
#define BMP_MEM_X 102
#define BMP_MEM_Y 42
#define BMP_STEP_X 5
#define BMP_STEP_Y 3

void Task_Bmp(void *pdata)
{
	S16 bmp_x,bmp_y;
	int reb_x=BMP_STEP_X,reb_y=BMP_STEP_Y;

	GUI_MEMDEV_Handle bmp_mem;
	
	bmp_mem=GUI_MEMDEV_Create(BMP_MEM_X,BMP_MEM_Y,BMP_MEM_SIZE_X,BMP_MEM_SIZE_Y);
	GUI_MEMDEV_Select(bmp_mem);
	
	bmp_x=BMP_MEM_X+1;
	bmp_y=BMP_MEM_Y+1;
	while(1){
		//Uart_Printf("reb_x=%d",reb_x);
		GUI_Clear();
		//GUI_DrawBitmap(&bmslamdunk1_tiny,bmp_x,bmp_y);
		GUI_FillCircle(bmp_x,bmp_y,2);
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
		GUI_MEMDEV_CopyToLCD(bmp_mem);
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



