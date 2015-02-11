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


OS_STK Task_Text_Stk[TaskStkLeath];
OS_STK Task_Title_Stk[TaskStkLeath];
OS_STK Task_SideText_Stk[TaskStkLeath];
OS_STK Task_Uart_Stk[TaskStkLeath];
OS_STK TaskStartSTK[TaskStkLeath];
OS_STK Task_Info_Stk[TaskStkLeath - 1];

extern const GUI_BITMAP bmslamdunk1_tiny;
extern const GUI_BITMAP bmslamdunk2_tiny;

extern void GUITASK_Init(void);
extern void  Init_Timer4(void);

OS_EVENT *Cls;
void 		Task_Text(void *pdata);			    // Task_Text 
void 		Task_Uart(void *pdata);			    // Task_Uart 
void 		Task_SideText(void *pdata);			    // Task_SideText 
void 		Task_Title(void *pdata);
void 		Task_Info(void *pdata);



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
    Uart_Printf("OSStatInit complete\n");
   
   	
   	OSTaskCreate(Task_Text,0, &Task_Text_Stk[TaskStkLeath - 1], NormalTaskPrio);
   	 
  	OSTaskCreate(Task_Title, 0, &Task_Title_Stk[TaskStkLeath - 1], NormalTaskPrio+4);
    OSTaskCreate(Task_SideText, 0, &Task_SideText_Stk[TaskStkLeath - 1], NormalTaskPrio+3);
    
    OSTaskCreate(Task_Info, 0, &Task_Info_Stk[TaskStkLeath - 1], NormalTaskPrio+2);
     
    OSTaskCreate(Task_Uart, 0, &Task_Uart_Stk[TaskStkLeath - 1], NormalTaskPrio+6);
    
   
   


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
#define CH_WIDTH 10 //Ò»¸öºº×ÖµÄ¿í¶È
#define CH_HIGH 10 //Ò»¸öºº×ÖµÄ¸ß¶È

void Task_Text (void *pdata)
{
	U8 err;

	while(1){
		
		OSSemPend(Cls,0,&err);
		Uart_Printf("CLS\n");
		OS_ENTER_CRITICAL();
		
		GUI_X_Lock();
		GUI_ClearRect(TEXT_X,TEXT_Y,TEXT_X+TEXT_SIZE_X,TEXT_Y+TEXT_SIZE_Y);
		
		GUI_DrawHLine(TEXT_Y,TEXT_X,TEXT_X+TEXT_SIZE_X);
		GUI_DrawHLine(TEXT_Y+TEXT_SIZE_Y,TEXT_X,TEXT_X+TEXT_SIZE_X);
		GUI_DrawVLine(TEXT_X,TEXT_Y,TEXT_Y+TEXT_SIZE_Y);
		GUI_DrawVLine(TEXT_X+TEXT_SIZE_X,TEXT_Y,TEXT_Y+TEXT_SIZE_Y);
		
		GUI_X_Unlock();
		//GUI_DrawBitmap(&bmslamdunk1_tiny,i,j);
		OS_EXIT_CRITICAL();
		//GUI_Delay(500);
		
		//OSSemPost(Disp);
		OSTimeDlyHMSM(0,0,0,200);
		
		
	}
		
}

void Task_Uart (void *pdata)
{
	
	U8 c=0;
	S16 char_x,char_y;
	char_x=TEXT_X+CH_WIDTH;
	char_y=TEXT_Y+CH_HIGH;
	Uart_Printf("begin i,j=%d,%d\n",char_x,char_y);
	while(1){
		//while((c=Uart_Getch())!='\r')
		//Uart_Printf("in while i,j=%d,%d\n",char_x,char_y);
		
		c=Uart_GetKey();
		//GUI_DispCharAt(c,i,j);
		
		
		if(c=='\r'){	
			Uart_Printf("x,y=%d,%d\n",char_x,char_y);
			char_x=TEXT_X+CH_WIDTH;
			char_y=char_y+CH_HIGH;
		
			if(char_y>=(TEXT_Y+TEXT_SIZE_Y)){
				char_y=TEXT_Y+CH_HIGH;
				OSSemPost(Cls);
				//OSTimeDlyHMSM(0,0,0,10);
				//Uart_Printf("post 2.j=%d\n",char_x);
			}
		}
		else if(c!='\r' && c!=0)
		{
			Uart_Printf("else i,j=%d,%d\n",char_x,char_y);
			//GUI_X_Lock();
			GUI_DispCharAt(c,char_x,char_y);
			//GUI_X_Unlock();
			char_x=char_x+CH_WIDTH;
			if(char_x>=(TEXT_X+TEXT_SIZE_X)){
				char_x=TEXT_X+CH_WIDTH;
				char_y=char_y+CH_HIGH;				
				}
			if(char_y>=(TEXT_Y+TEXT_SIZE_Y-CH_HIGH)){
				char_y=TEXT_Y+CH_HIGH;;
				OSSemPost(Cls);
				//OSTimeDlyHMSM(0,0,0,10);
				//Uart_Printf("post 1.j=%d\n",char_y);
				}
			}
		
		OSTimeDlyHMSM(0,0,0,100);
		
		}

}

//²à±ßÎÄ±¾¿òÔ­µã¼°´óÐ¡
#define SIDE_X 0
#define SIDE_Y 30
#define SIDE_SIZE_X 96
#define SIDE_SIZE_Y 170

#define SIDE_TEXT_STEP 10
#define SIDE_TEXT_STEP_ZOOM 2

void Task_SideText(void *pdata)
{
	S16 i=SIDE_Y+SIDE_SIZE_Y;
	GUI_MEMDEV_Handle side_mem;
	side_mem=GUI_MEMDEV_Create(SIDE_X,SIDE_Y,SIDE_SIZE_X,SIDE_SIZE_Y);
	GUI_MEMDEV_Select(side_mem);
	
	while(1){	
	GUI_ClearRect(SIDE_X,SIDE_Y,SIDE_X+SIDE_SIZE_X,SIDE_Y+SIDE_SIZE_Y);
	//GUI_DrawBitmap(&bmslamdunk1_tiny,0,0);
	
	//GUI_DrawBitmap(&bmslamdunk1_tiny,0,i);
	
	//GUI_X_Lock();
	GUI_DispStringAt("uc/OS_II",SIDE_X,i);
	GUI_DispStringAt("uc/GUI",SIDE_X,i+SIDE_TEXT_STEP);
	GUI_DispStringAt("Version:",SIDE_X,i+SIDE_TEXT_STEP*2);
	GUI_DispDecMin(OSCPUUsage);
	//GUI_DispDecMin(OSVersion());
	
	
	i=i-SIDE_TEXT_STEP/SIDE_TEXT_STEP_ZOOM;
	//Uart_Printf("i=%d\n",i);
	if(i<=10){
		i=SIDE_Y+SIDE_SIZE_Y;
		Uart_Printf("Caution ! i=200\n");
		}
	
	GUI_MEMDEV_CopyToLCD(side_mem);
	
	//GUI_MEMDEV_Select(0);
	//GUI_X_Unlock();
	//GUI_MEMDEV_Delete(hMem);
	
	
	OSTimeDlyHMSM(0,0,0,500);
	
	}
}

#define TITLE_X 100
#define TITLE_Y 20
#define TITLE_SIZE_X 200
#define TITLE_SIZE_Y 10
#define TITLE_TEXT_STEP 5


void Task_Title(void *pdata)
{
	
	GUI_MEMDEV_Handle title_mem;
	S16 i=TITLE_X+TITLE_SIZE_X;
	title_mem=GUI_MEMDEV_Create(TITLE_X,TITLE_Y,TITLE_SIZE_X,TITLE_SIZE_Y);	
	GUI_MEMDEV_Select(title_mem);
	while(1){
	//Uart_Printf("Task_Title\n");
	GUI_ClearRect(TITLE_X,TITLE_Y,TITLE_X+TITLE_SIZE_X,TITLE_Y+TITLE_SIZE_Y);
	//GUI_X_Lock();
	GUI_DispStringAt("Uart Test,Please Input Characters!",i,20);
	i=i-TITLE_TEXT_STEP;
	if(i<=0)
		i=TITLE_X+TITLE_SIZE_X;
	GUI_MEMDEV_CopyToLCD(title_mem);
	//GUI_X_Unlock();
	//GUI_MEMDEV_Delete(title_mem);
	
	OSTimeDlyHMSM(0,0,0,300);
	
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
	info_mem=GUI_MEMDEV_Create(INFO_X,INFO_Y,INFO_SIZE_X,INFO_SIZE_Y);
	GUI_MEMDEV_Select(info_mem);
	
	while(1){
		//Uart_Printf("Information!\n");
		GUI_ClearRect(INFO_X,INFO_Y,INFO_X+INFO_SIZE_X,INFO_Y+INFO_SIZE_Y);
		GUI_DispStringAt("Static informaion:",INFO_X,INFO_Y);
		GUI_DispStringAt("CPU Usage:",INFO_X,INFO_Y+INFO_STEP);
		GUI_DispDecMin(OSCPUUsage);
		GUI_DispString("%");
		GUI_DispStringAt("Task Switch Times:",INFO_X,INFO_Y+INFO_STEP*2);
		GUI_DispDecMin(OSIdleCtr);
		GUI_DispString("/s");
		
		//GUI_DispString();
		GUI_MEMDEV_CopyToLCD(info_mem);
		
		OSTimeDlyHMSM(0,0,1,0);
		
	}
	
}


__inline void cpu_init(void)
{
	U16 i;
    U8 key;
	U32 mpll_val = 0 ;
    
    Port_Init();  //¶Ë¿Ú³õÊ¼»¯
    Isr_Init();   //ÖÐ¶Ï³õÊ¼»¯
    //key_Eint();
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
/*
#include "GUI.h"

extern const GUI_BITMAP bmMicriumLogo;
extern const GUI_BITMAP bmMicriumLogo_1bpp;

void Main(void) {
  int Cnt =0;
  int i,YPos;
  int LCDXSize = LCD_GET_XSIZE();
  int LCDYSize = LCD_GET_YSIZE();
  const GUI_BITMAP *pBitmap;
  
  Lcd_Init();
	Lcd_PowerEnable(0,1);
	Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0xffff);
	Uart_Printf("begin!\n");
  
  GUI_Init();
  GUI_SetBkColor(GUI_RED); GUI_Clear();
  GUI_Delay(1000);
  GUI_SetBkColor(GUI_BLUE); GUI_Clear();
  GUI_Delay(1000);
  GUI_SetColor(GUI_WHITE);
  for (i=0; i<1000; i+=10) {
    GUI_DrawHLine(i,0,100);
    GUI_DispStringAt("Line ",0,i);
    GUI_DispDecMin(i);
  }
  GUI_Delay(1000);
  GUI_SetColor(0x0);
  GUI_SetBkColor(0xffffff);
  for (i=0; i<160; i++) {
    int len = (i<80) ? i : 160-i;
    GUI_DrawHLine(i,20,len+20);
  }
  GUI_Delay(1000);
  GUI_Clear();
  if (LCD_GET_YSIZE()>(100+bmMicriumLogo_1bpp.YSize)) {
    pBitmap=&bmMicriumLogo;
  } else {
    GUI_SetColor(GUI_BLUE);
    pBitmap=&bmMicriumLogo_1bpp;
  }
  GUI_DrawBitmap(pBitmap,(LCDXSize-pBitmap->XSize)/2,10);
  YPos=20+pBitmap->YSize;
  GUI_SetFont(&GUI_FontComic24B_1);
  GUI_DispStringHCenterAt("www.micrium.com",LCDXSize/2,YPos);
  GUI_Delay(1000);
  GUI_SetColor(GUI_RED);
  GUI_DispStringHCenterAt("© 2000\n", LCDXSize/2,YPos+30);
  GUI_SetFont(&GUI_Font10S_1);
  GUI_DispStringHCenterAt("Micriµm Inc.",LCDXSize/2,YPos+60);;
  GUI_Delay(1000);
}
*/

