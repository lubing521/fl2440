#define	GLOBAL_CLK		1
#include <stdlib.h>
//#include <time.h>
#include "GUI.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
//#include "Target.h"
#include "Def.h"
#include "option.h"
//#include "uc_os.h"
//#include "app_cfg.h"
#include "FL2440_LCD_for_GUI.h"

__inline void cpu_init(void);
void Main()
{
	
	U32 x,y;
	cpu_init();
	Lcd_Init();
	Lcd_PowerEnable(0,1);
	Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0xffff);
	Uart_Printf("begin!\n");
	GUI_Init();
	
	GUI_DispString("Hello world!");
	
	//GUI_Clear();
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	//GUI_FillCircle(120,64,40);
	//GUI_SetDrawMode(GUI_DRAWMODE_XOR);
	//GUI_FillCircle(140,84,40);
	srand(10);
	for(;;){
		
		x=(U32)rand()%320;
		y=(U32)rand()%240;
		Uart_Printf("x,y\t %d,%d MAX%d\n",x,y,RAND_MAX);
		GUI_FillCircle(x,y,4);
		Delay(x);
		GUI_Clear();
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

