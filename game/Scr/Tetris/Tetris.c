
/*****************************************************/
/*                   方块游戏                        */
/*  环境:ADS1.2                                      */
/*  作者：Liuwei                                     */
/*  E-mail：liuweiele@163.com                        */
/*  QQ：304466616                                    */
/*  2009年02月21日                                   */
/*****************************************************/
	
/*****************************************************/

#include	"Include.h"
#include	"Tetris.h"

/*============================================================================*/
/*============================================================================*/

struct	BLOCK
{
	U16	Index;
	U16	Rotate;
};


/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
#define	xGrids	30
#define	yGrids	35

static	U8	TetrisMap[xGrids*yGrids];

static	U32	BlockCount,UserPoint;

static	struct	BLOCK	CurBlock,NextBlock;

/*============================================================================*/

/*============================================================================*/
/*============================================================================*/

	
			
/*============================================================================*/

/*============================================================================*/

char	BlockTab[]={
								
									0,0,0,0,
									0,1,1,0,
									0,1,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,1,0,
									0,1,1,0,
									0,0,0,0,
								
									0,0,0,0,
									0,1,1,0,
									0,1,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,1,0,
									0,1,1,0,
									0,0,0,0,
									/////////////////0
										
									0,1,0,0,
									0,1,0,0,
									0,1,0,0,
									0,1,1,1,
									
									0,0,0,1,
									0,0,0,1,
									1,1,1,1,
									0,0,0,0,
									
									1,1,1,0,
									0,0,1,0,
									0,0,1,0,
									0,0,1,0,
									
									0,0,0,0,
									1,1,1,1,
									1,0,0,0,
									1,0,0,0,
									////////////////1
									
									0,0,1,0,
									0,0,1,0,
									0,0,1,0,
									1,1,1,0,
									
									0,0,0,0,
									1,1,1,1,
									0,0,0,1,
									0,0,0,1,
									
									0,1,1,1,
									0,1,0,0,
									0,1,0,0,
									0,1,0,0,
									
									1,0,0,0,
									1,0,0,0,
									1,1,1,1,
									0,0,0,0,
									///////////////////2
									
									0,0,0,0,
									0,1,0,0,
									0,1,1,0,
									0,0,1,0,
									
									0,0,0,0,
									0,0,1,1,
									0,1,1,0,
									0,0,0,0,
									
									0,1,0,0,
									0,1,1,0,
									0,0,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,1,0,
									1,1,0,0,
									0,0,0,0,
									///////////////////3
									
									0,0,0,0,
									0,0,1,0,
									0,1,1,0,
									0,1,0,0,
									
									0,0,0,0,
									0,1,1,0,
									0,0,1,1,
									0,0,0,0,
									
									0,0,1,0,
									0,1,1,0,
									0,1,0,0,
									0,0,0,0,
									
									0,0,0,0,
									1,1,0,0,
									0,1,1,0,
									0,0,0,0,
									////////////////////4
									
									0,0,0,0,
									0,0,0,0,
									1,1,1,1,
									0,0,0,0,
									
									0,0,1,0,
									0,0,1,0,
									0,0,1,0,
									0,0,1,0,
									
									0,0,0,0,
									1,1,1,1,
									0,0,0,0,
									0,0,0,0,
									
									0,1,0,0,
									0,1,0,0,
									0,1,0,0,
									0,1,0,0,
									/////////////////////5
									
									0,0,0,0,
									0,1,0,0,
									0,1,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,0,1,0,
									0,1,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,1,0,
									0,1,0,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,1,0,
									0,0,1,0,
									0,0,0,0,
									/////////////////////6
									
									0,0,0,0,
									0,1,0,0,
									0,1,1,0,
									0,1,0,0,
									
									0,0,0,0,
									0,0,1,0,
									0,1,1,1,
									0,0,0,0,
									
									0,0,1,0,
									0,1,1,0,
									0,0,1,0,
									0,0,0,0,
									
									0,0,0,0,
									1,1,1,0,
									0,1,0,0,
									0,0,0,0,
									/////////////////////7
									
									0,0,0,0,
									0,1,0,0,
									1,1,1,0,
									0,1,0,0,
									
									0,0,0,0,
									0,1,0,0,
									1,1,1,0,
									0,1,0,0,
									
									0,0,0,0,
									0,1,0,0,
									1,1,1,0,
									0,1,0,0,
									
									0,0,0,0,
									0,1,0,0,
									1,1,1,0,
									0,1,0,0,
									/////////////////////8
									
									0,0,0,0,
									0,0,0,0,
									0,1,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,0,1,0,
									0,0,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,1,0,
									0,0,0,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,0,0,
									0,1,0,0,
									0,0,0,0,
									/////////////////////9
									
									0,0,0,0,
									1,1,1,1,
									1,0,0,1,
									0,0,0,0,
									
									0,1,1,0,
									0,1,0,0,
									0,1,0,0,
									0,1,1,0,
									
									0,0,0,0,
									1,0,0,1,
									1,1,1,1,
									0,0,0,0,
									
									0,1,1,0,
									0,0,1,0,
									0,0,1,0,
									0,1,1,0,
									/////////////////////10
									
									0,0,1,0,
									0,0,1,0,
									0,0,1,0,
									0,1,1,0,
									
									0,0,0,0,
									1,1,1,1,
									0,0,0,1,
									0,0,0,0,
									
									0,1,1,0,
									0,1,0,0,
									0,1,0,0,
									0,1,0,0,
									
									0,0,0,0,
									1,0,0,0,
									1,1,1,1,
									0,0,0,0,
									/////////////////////11
									
									0,0,0,0,
									0,1,0,0,
									0,1,1,1,
									0,1,0,0,
									
									0,0,1,0,
									0,0,1,0,
									0,1,1,1,
									0,0,0,0,
									
									0,0,1,0,
									1,1,1,0,
									0,0,1,0,
									0,0,0,0,
									
									0,0,0,0,
									1,1,1,0,
									0,1,0,0,
									0,1,0,0,
									/////////////////////12
									
									0,1,0,0,
									0,1,0,0,
									0,1,1,1,
									0,0,0,0,
									
									0,0,1,0,
									0,0,1,0,
									1,1,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,1,1,
									0,1,0,0,
									0,1,0,0,
									
									0,0,0,0,
									1,1,1,0,
									0,0,1,0,
									0,0,1,0,
									////////////////////13
									
									0,0,0,0,
									1,1,1,0,
									1,0,1,0,
									0,0,0,0,
									
									0,0,0,0,
									0,1,1,0,
									0,1,0,0,
									0,1,1,0,
									
									0,0,0,0,
									0,1,0,1,
									0,1,1,1,
									0,0,0,0,
									
									0,1,1,0,
									0,0,1,0,
									0,1,1,0,
									0,0,0,0,
									/////////////////////14
									
									0,0,0,0,
									0,0,1,0,
									0,0,1,0,
									0,1,1,0,
									
									0,0,0,0,
									0,1,1,1,
									0,0,0,1,
									0,0,0,0,
									
									0,1,1,0,
									0,1,0,0,
									0,1,0,0,
									0,0,0,0,
									
									0,0,0,0,
									1,0,0,0,
									1,1,1,0,
									0,0,0,0,
									///////////////////////15
									
													
																																																								
							};



/*============================================================================*/
//
void	DrawBlockDot(int x,int y,int Color)
{
	GUI_Rectangle(x,y,8,8,Color);
	GUI_FillRectangle(x+2,y+2,8-4,8-4,Color);
}

/*============================================================================*/

void	SetMapDot(U8 *pBuf,int xgrd,int ygrd)
{
	pBuf[(ygrd*xGrids)+xgrd]=1;
}

/*============================================================================*/

void	ClrMapDot(U8 *pBuf,int xgrd,int ygrd)
{
	pBuf[(ygrd*xGrids)+xgrd]=0;
}

/*============================================================================*/

int		GetMapDot(U8 *pBuf,int xgrd,int ygrd)
{
	if(xgrd<0)	xgrd=0;
	return	pBuf[(ygrd*xGrids)+xgrd];
}

/*============================================================================*/

void	DrawBlock(int x,int y,int Color,int BlockID)
{
	int	xx,yy,i,offset;
	////
	
	offset=BlockID*(16);
	xx	=x;
	yy	=y;
	
	for(i=0;i<16;i++)
	{
		xx=(i%4)*7;
		yy=(i/4)*7;
		
		if(BlockTab[offset+i])
		{
			DrawBlockDot(x+xx,y+yy,Color);
		}
	}
	
}

/*============================================================================*/
//检查在指定的位置是否可以放下该方块

int	CheckBlockInTray(U8 *pBuf,int xgrd,int ygrd,int BlockID)
{
	int	offset,xx,yy;
	/////
	
	//if(xgrd<0)	xgrd=0;
	//if(ygrd<0)	ygrd=0;
	////
	
	offset=BlockID*16;
	
	for(yy=0;yy<4;yy++)
	{
		for(xx=0;xx<4;xx++)
		{
					
			if(BlockTab[offset+(yy*4)+xx])
			{	
																
				if(GetMapDot(pBuf,xgrd+xx,ygrd+yy))	return 0;	//该格已占用			
			}
			
		}
	
	}
	
	return 1;

}

/*============================================================================*/

/*============================================================================*/

/*============================================================================*/
//在托盘里指定位置画一个方块.

int	DrawBlockInMap(U8 *pBuf,int xgrd,int ygrd,int BlockID)
{
	int	offset,xx,yy;
	/////
	
//	if(!CheckBlockInTray(pBuf,xgrd,ygrd,BlockID))	return 0;
	////
	
	offset=BlockID*16;
	
	for(yy=0;yy<4;yy++)
	{
		for(xx=0;xx<4;xx++)
		{
					
			if(BlockTab[offset+(yy*4)+xx])
			{								
				SetMapDot(pBuf,xgrd+xx,ygrd+yy);			
			}
			
		}
	
	}

}

/*============================================================================*/

//清除一行方块.

void	ClrMapLine(U8 *pBuf,int Line)
{

	int	xx,yy;
	////
	
	yy=Line;
	for(xx=0;xx<xGrids;xx++)
	{
	
		if(xx<4)				continue;
		if(xx>(xGrids-4-1))		continue;
		
		ClrMapDot(pBuf,xx,yy);
		
	}
	
}

/*============================================================================*/
//读一行

void	GetMapLine(U8 *pMap,int Line,U8 *pBuf)
{
	int	i,xx,yy;
	////
	
	i=0;
	yy=Line;
	for(xx=4;xx<(xGrids-4);xx++)
	{
			
		pBuf[i++]=GetMapDot(pMap,xx,yy);
	
	}
	
}

//写一行

void	PutMapLine(U8 *pMap,int Line,U8 *pBuf)
{

	int	i,xx,yy;
	////
	
	i=0;
	yy=Line;
	for(xx=4;xx<(xGrids-4);xx++)
	{
			
			if(pBuf[i++])
			{
				SetMapDot(pMap,xx,yy);
			}
			else
			{
				ClrMapDot(pMap,xx,yy);
			}
	
	}

}

//检测一行是否已满

int	MapLineIsFull(U8 *pMap,int Line)
{

	int xx,yy;
	////
	yy=Line;
	///
	for(xx=4;xx<(xGrids-4-1);xx++)
	{
		if(!GetMapDot(pMap,xx,yy))	return 0;	//如果此行有任意一点为0,则表示此行没有满.
	
	}
	return 1;

}

/*============================================================================*/

int	CheckMap(U8 *pMap)
{
	int	i,j,xx,yy;
	int	states;
	////
	states=0;
st:
	for(yy=(yGrids-4-1);yy>0;yy--)	//从最后一行开始检测
	{
		if(MapLineIsFull(pMap,yy))
		{
			//如果此行已满.
			
			UserPoint+=(2*xGrids);
			
			states=1;
			for(i=yy;i>0;i--)
			{
				U8	buf[xGrids];
				////
				GetMapLine(pMap,i-1,buf);	//读上一行
				PutMapLine(pMap,i,buf);		//写到本行.	
				
			}
			
			goto st;					//重新开始检测
			
		}

		
	}
	
	return states;
}

/*============================================================================*/

//初始化背景

void	TetrisMapInit(U8 *pBuf)
{
	int xx,yy;
	////
	
	for(yy=0;yy<yGrids;yy++)
	{
		for(xx=0;xx<xGrids;xx++)
		{
			
			if(xx<4)
			{
				SetMapDot(pBuf,xx,yy);
				continue;
			}
			////
			if(xx>(xGrids-4-1))
			{
				SetMapDot(pBuf,xx,yy);
				continue;
			}
			////
			if(yy>(yGrids-4-1))
			{
				SetMapDot(pBuf,xx,yy);
				continue;
			}
			////
			
			ClrMapDot(pBuf,xx,yy);
			
			
			
		}
	
	}

}

/*============================================================================*/

/*============================================================================*/


/*============================================================================*/

void	ShowMap(U8 *pMap,int x,int y,int FrColor,int BkColor,int BorderColor)
{
	int xx,yy,dx,dy,x_offset,y_step;
	////
	
	dx			=((xGrids-(4*2))*7)+1;
	dy			=((yGrids-(1*4))*7)+1;
	x_offset	=((1*4)*7);
	y_step		=dy>>2;
	////
	
	if(BkColor!=RGB_TRANS)
	{
		GUI_FillRectangle(x,y,dx,dy,BkColor);
	}
	else
	{
		GUI_SetTrans(128);
		GUI_FillRectangle(x,y,dx,dy,RGB565(24,48,24));
		GUI_SetTrans(0);
	}
	////////
	
	for(yy=0;yy<(yGrids-4);yy++)
	{
		for(xx=4;xx<(xGrids-4);xx++)
		{
			
 			if(GetMapDot(pMap,xx,yy))
			{
				DrawBlockDot(x+(xx*7)-x_offset,y+(yy*7),FrColor);
			
			}
		
		}
	
	}
	////////
	
	GUI_Rectangle(x,y,dx,dy,BorderColor);
	////
	
}


/*============================================================================*/

/*============================================================================*/

void	GetNewBlock(struct BLOCK *block)
{
	
	block->Index	=(GUI_RandVal()%(16))*4;	//方块索引(随机生成)
	block->Rotate	=GUI_RandVal()%4;			//方块方向(随机生成)
				
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/


int	Tetris(void *pdata)
{

	#define	GAME_START	USER_MSG+0
	#define	GAME_PAUSE	USER_MSG+1
	////
	
	HWND	wnd;
	int	i,key,game_states,xgrd,ygrd,xoffset;
	U8	TempMap[xGrids*yGrids];
	struct	GUI_BUTTON		button[]={
							
							{			0,0,0,0,	RGB565(00,00,00),RGB565(24,20,10),RGB565(05,10,05),		KEY_ID_ESC,			BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"退出"},
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,50,10),RGB565(05,10,05),		KEY_ID_UP,			BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"∧"},
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,20,24),RGB565(05,10,05),		'p',				BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"暂停"},
						
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,50,10),RGB565(05,10,05),		KEY_ID_LEFT,		BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"<"},
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,50,10),RGB565(05,10,05),		KEY_ID_DOWN,		BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"∨"},
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,50,10),RGB565(05,10,05),		KEY_ID_RIGHT,		BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	">"},
						
								
																																								
							{			0,0,0,0,	0,0,0,	0,	0,	0,	0,	0,"--"},

					};
								
	////////

	struct	GUI_MATRIX_BN	m_button;

	////
	char	*s_Name[]={"Tetris","方块游戏","???"};
	
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
	
	m_button.x	=(LCD_XSIZE-160);
	m_button.y	=LCD_YSIZE-80;
	m_button.dx	=160;
	m_button.dy	=72;
	
	m_button.xQty	=3;
	m_button.yQty	=2;
	m_button.xSpace	=8;
	m_button.ySpace	=8;
	m_button.ActObj	=-1;
	m_button.Enable	=1;
	m_button.pBN	=button;
	////
	
	GUI_MemdevEnable();
	TS_Free();
	Key_Free();
	/////
	
ReStart:

	TetrisMapInit(TetrisMap);
	
	UserPoint	=0;
	BlockCount	=0;
	
	key			=0;
	i			=0;
	xgrd		=xGrids>>1;					//x
	ygrd		=0;							//y
	xoffset		=0;							//水平偏移
	game_states	=GAME_START;
	
	GetNewBlock(&CurBlock);		//当前方块
	GetNewBlock(&NextBlock);	//下一个方块
	
	////
	
	while(1)
	{
		int a,block_states;
		
		////
st:	
		memcpy(TempMap,TetrisMap,(xGrids*yGrids));	
		
		block_states=CheckBlockInTray(TetrisMap,xgrd+xoffset,ygrd,CurBlock.Index+CurBlock.Rotate);
		////
		
		if(block_states)
		{
			//当前位置可以放下方块	
			
			GUI_Image(0,0,GUI_GetWallpaper());
			
			////
			
			GUI_PutMatrixButton(&m_button);
			DrawBlockInMap(TempMap,xgrd+xoffset,ygrd,CurBlock.Index+CurBlock.Rotate);
			ShowMap(TempMap,20,20,RGB565(00,00,00),RGB_TRANS,RGB565(00,00,00));	
			
			////////
			DrawBlock(LCD_XSIZE-96,40,RGB565(24,00,00),NextBlock.Index+NextBlock.Rotate);	//显示下一个方块	
			
			GUI_LabelPrintf(LCD_XSIZE-96,80,128,40,RGB565(31,50,00),RGB_TRANS,RGB_TRANS,LEFT,FONT(ASCII8x16,HZ16x16),"方块数:%d",BlockCount);
			GUI_LabelPrintf(LCD_XSIZE-96,120,128,40,RGB565(31,50,00),RGB_TRANS,RGB_TRANS,LEFT,FONT(ASCII8x16,HZ16x16),"得分数:%d",UserPoint);
			
			////////	
					
					
			GUI_DisplayUpdate(0,0,LCD_XSIZE,LCD_YSIZE);
			////
			if(!CheckBlockInTray(TetrisMap,xgrd+xoffset,ygrd+1,CurBlock.Index+CurBlock.Rotate))
			{
			
				//如果下一行不能放下该方块:
				
				if(CheckMap(TempMap))	//检测是否有满的行
				{
					Uart_Printf(0,"User Point=%d\n",UserPoint);
				}
				////////					
				
GoNextBlock:
				xgrd	=(GUI_RandVal()%(xGrids-(2*4)));
				if(xgrd<4)	xgrd=4;
					
				ygrd	=0;
				xoffset	=0;
				
			
				BlockCount++;
				
				CurBlock.Index	=NextBlock.Index;
				CurBlock.Rotate	=NextBlock.Rotate;
				
				GetNewBlock(&NextBlock);
				
				memcpy(TetrisMap,TempMap,(xGrids*yGrids));			
				////////
				Uart_Puts(0,"Next Block!\n");
				////
				goto st;

			}
			
		
		}
		else
		{
			//当前位置不能放下方块
				
			if(ygrd==0)
			{
			
				
				Delay_n15ms(500/15);
				////
				TS_Free();
				Key_Free();
				
				Tick1000msDecCnt=20;		//20秒钟后将自动重新运行
				while(Tick1000msDecCnt)
				{
					
					if(Tick250msCnt&0x01)
					{
						GUI_LabelPrintf(LCD_XSIZE-128,160,128,32,RGB565(00,00,00),RGB565(24,48,00),RGB565(10,20,10),CENTER,FONT(ASCII8x16,HZ16x16),"游戏结束 %d",Tick1000msDecCnt);
						GUI_DisplayUpdate(LCD_XSIZE-128,160,128,32);
					}
					////
					if(TS_Press_100ms())	break;
					if(GetKeyValue())		break;
				}
				/////
				
				Uart_Puts(0,"Tetris ReStart!\n");
				goto ReStart;
				////////
			}
			else
			{
				goto GoNextBlock;
			}
			

		}
		
		////////////////////////////////
				
		a=Tick500msCnt;
		while(1)
		{
			
			
			if(!pGUI_TS->IsDown())
			{
				//触摸屏没有按下.
				GUI_FreeButton(m_button.pBN);
				key=GetKeyValue();
				GUI_MatrixButtonEnable(&m_button);
				GUI_GetMatrixButton(&m_button);
				
			}
			else
			{
				//触摸屏按下.						
			
				key=GUI_GetMatrixButton(&m_button);
															
				if(TS_GetDownTime()>500)
				{
					GUI_MatrixButtonEnable(&m_button);
				}
				////
				
				if(key)
				{
					GUI_MatrixButtonDisable(&m_button);
				}
				
				////
				
			}
		
			GUI_DisplayUpdate(m_button.x,m_button.y,m_button.dx,m_button.dy);			
			///////////////////
			
			if(key)
			{
				int	i;
				////
				
				switch(key)
				{
				
					case	'a':
					case	KEY_ID_LEFT:
							
							if(CheckBlockInTray(TetrisMap,xgrd+xoffset-1,ygrd,CurBlock.Index+CurBlock.Rotate))
							{
								xoffset--;
								goto	st;
							}
							
							break;
							////////////////////
							
					case	'd':
					case	KEY_ID_RIGHT:
					
							if(CheckBlockInTray(TetrisMap,xgrd+xoffset+1,ygrd,CurBlock.Index+CurBlock.Rotate))
							{
								xoffset++;
								goto st;
							}
							
							break;
							/////////////////////
							
					case	's':
					case	KEY_ID_DOWN:
					
							ygrd+=1;
							goto	st;
							break;
							//////////////////////
					
					case	KEY_ID_ENTER:
					case	KEY_ID_UP:
					
							TS_Free();
							
							i=CurBlock.Rotate;
							
							CurBlock.Rotate++;
							if(CurBlock.Rotate>=4)
							{
								CurBlock.Rotate=0;
							}
							////

							if(CheckBlockInTray(TetrisMap,xgrd+xoffset,ygrd,CurBlock.Index+CurBlock.Rotate))
							{
								//如果旋转后可以放下该方块.
								goto st;
							}
							
							////不能放下该方块
							CurBlock.Rotate=i;
							break;
							///////////////////////
							
					case	KEY_ID_ESC:
							goto	ret;
							break;
							///////////////////////
							
					case	'p':
					
							if(game_states==GAME_START)
							{
								m_button.pBN[2].Text="开始";
								game_states=GAME_PAUSE;
								
							}
							else
							{
								m_button.pBN[2].Text="暂停";
								game_states=GAME_START;
							}
							
							TS_Free();
							goto st;
							break;
							///////////////////
							
							
				}
				
			}
			////
			
			if((a!=Tick500msCnt) && (game_states!=GAME_PAUSE))	
			{
				ygrd+=1;
				goto	st;
			}
			////
			
		}
		
		////////////////////////////////

		
		
	}
	////
	
ret:
	TS_Free();
	Key_Free();


}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
