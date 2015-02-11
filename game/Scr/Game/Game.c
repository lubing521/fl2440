
#include	"Include.h"
#include	"Game.h"


/*============================================================================*/

static	struct	GUI_MENU_DATA	GameMenuTab[]={
										//ID,	Status,		X0_Pst,Y0_Pst,X1_Pst,Y1_Pst,	Run_Addr,	ICO,	Desc
										
										
										{	0,		1,		0,0,	0,0,		Tetris,	},
										{	0,		1,		0,0,	0,0,		Race,	},
										
			
												
										{	0,		0,	0,0,	0,0,		0,		}
										
										};
										

/*============================================================================*/
/*============================================================================*/


/*============================================================================*/

static	int	Game_MenuWinProc(HWND wnd,int msg,int param0,void *pdata)
{
	
	int	val;
	struct	GUI_WINDOW	*pWin;
	struct	GUI_MENU	*pMenu;
	////
	
	pWin	=(void*)wnd;
	pMenu	=(void*)pWin->ExData;
	////
	
	pMenu->x	=pWin->x;
	pMenu->y	=pWin->y;
	pMenu->dx	=pWin->dx;
	pMenu->dy	=pWin->dy;
	////
	
	switch(msg)
	{
	
		case	MSG_PAINT:
		case	MSG_LBUTTON_DOWN:
		case	MSG_KEY:
				
				
				GUI_MemdevEnable();
				
				if(!GUI_ListMenu(wnd,msg,param0,pdata,pMenu))
				{
					PostQuitMessage(wnd);
				}
				GUI_ShowWindow(wnd);
				break;
				////////

		default:
					
				if(TS_PRESS_100MS())	SendMessage(wnd,MSG_LBUTTON_DOWN,0,0);
				val=GetKeyValue();
				if(val)					SendMessage(wnd,MSG_KEY,val,0);
			
				break;
				////////
	
	}	

	
	
}
/*============================================================================*/

int	GameMenu(void *pdata)
{
	
	int	wnd;
	int	key;
	int a;
	struct	MESSAGE	msg;
	struct	GUI_MENU		Menu;
	char	*s_Name[]={"Game","сно╥","???"};
	
	/////////////	
	if(pdata)
	{
		struct	PGM_INFO *pInfo	=pdata;
		pInfo->Name			=(char*)GUI_Language(s_Name);
		pInfo->pIcon		=0;
		pInfo->pSmallIcon	=0;
		pInfo->ExtraInfo	="Version:1.0";
		return	1;
	
	}
	/////////////
	TS_Free();
	Key_Free();

	///////////////////
	
	Menu.x			=0;
	Menu.y			=0;
	Menu.dx			=LCD_XSIZE;
	Menu.dy			=LCD_YSIZE;
	Menu.FrColor	=RGB565(31,63,31);
	Menu.BkColor	=RGB_TRANS;
	Menu.ActColor	=RGB565(00,63,00);
	Menu.Mode		=LIST_MENU;
	Menu.ActObj		=0;
	Menu.pMenuData	=GameMenuTab;
	Menu.Name		=(char*)GUI_Language(s_Name);
	////
	
	wnd=GUI_CreateWindow(	0,0,LCD_XSIZE,LCD_YSIZE,
							0,RGB565(0,0,0),GUI_GetWallpaper(),
							(U32)&Menu,
							Game_MenuWinProc,
							(char*)GUI_Language(s_Name));
	GUI_MemdevEnable();
	
	SendMessage(wnd,MSG_PAINT,0,0);
	////
	
	while(GetMessage(wnd,&msg))
	{
		DispatchMessage(wnd,&msg);				
	}
		
	////
	//GUI_DestroyWindow(wnd);
	TS_Free();
	Key_Free();
	
}

/*============================================================================*/

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
