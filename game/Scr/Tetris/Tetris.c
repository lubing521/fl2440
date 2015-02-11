
/*****************************************************/
/*                   ������Ϸ                        */
/*  ����:ADS1.2                                      */
/*  ���ߣ�Liuwei                                     */
/*  E-mail��liuweiele@163.com                        */
/*  QQ��304466616                                    */
/*  2009��02��21��                                   */
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
//�����ָ����λ���Ƿ���Է��¸÷���

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
																
				if(GetMapDot(pBuf,xgrd+xx,ygrd+yy))	return 0;	//�ø���ռ��			
			}
			
		}
	
	}
	
	return 1;

}

/*============================================================================*/

/*============================================================================*/

/*============================================================================*/
//��������ָ��λ�û�һ������.

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

//���һ�з���.

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
//��һ��

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

//дһ��

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

//���һ���Ƿ�����

int	MapLineIsFull(U8 *pMap,int Line)
{

	int xx,yy;
	////
	yy=Line;
	///
	for(xx=4;xx<(xGrids-4-1);xx++)
	{
		if(!GetMapDot(pMap,xx,yy))	return 0;	//�������������һ��Ϊ0,���ʾ����û����.
	
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
	for(yy=(yGrids-4-1);yy>0;yy--)	//�����һ�п�ʼ���
	{
		if(MapLineIsFull(pMap,yy))
		{
			//�����������.
			
			UserPoint+=(2*xGrids);
			
			states=1;
			for(i=yy;i>0;i--)
			{
				U8	buf[xGrids];
				////
				GetMapLine(pMap,i-1,buf);	//����һ��
				PutMapLine(pMap,i,buf);		//д������.	
				
			}
			
			goto st;					//���¿�ʼ���
			
		}

		
	}
	
	return states;
}

/*============================================================================*/

//��ʼ������

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
	
	block->Index	=(GUI_RandVal()%(16))*4;	//��������(�������)
	block->Rotate	=GUI_RandVal()%4;			//���鷽��(�������)
				
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
							
							{			0,0,0,0,	RGB565(00,00,00),RGB565(24,20,10),RGB565(05,10,05),		KEY_ID_ESC,			BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"�˳�"},
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,50,10),RGB565(05,10,05),		KEY_ID_UP,			BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"��"},
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,20,24),RGB565(05,10,05),		'p',				BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"��ͣ"},
						
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,50,10),RGB565(05,10,05),		KEY_ID_LEFT,		BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"<"},
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,50,10),RGB565(05,10,05),		KEY_ID_DOWN,		BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	"��"},
							{			0,0,0,0,	RGB565(00,00,00),RGB565(10,50,10),RGB565(05,10,05),		KEY_ID_RIGHT,		BN_3D,Up,	0,		FONT(ASCII8x16,HZ16x16),	">"},
						
								
																																								
							{			0,0,0,0,	0,0,0,	0,	0,	0,	0,	0,"--"},

					};
								
	////////

	struct	GUI_MATRIX_BN	m_button;

	////
	char	*s_Name[]={"Tetris","������Ϸ","???"};
	
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
	xoffset		=0;							//ˮƽƫ��
	game_states	=GAME_START;
	
	GetNewBlock(&CurBlock);		//��ǰ����
	GetNewBlock(&NextBlock);	//��һ������
	
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
			//��ǰλ�ÿ��Է��·���	
			
			GUI_Image(0,0,GUI_GetWallpaper());
			
			////
			
			GUI_PutMatrixButton(&m_button);
			DrawBlockInMap(TempMap,xgrd+xoffset,ygrd,CurBlock.Index+CurBlock.Rotate);
			ShowMap(TempMap,20,20,RGB565(00,00,00),RGB_TRANS,RGB565(00,00,00));	
			
			////////
			DrawBlock(LCD_XSIZE-96,40,RGB565(24,00,00),NextBlock.Index+NextBlock.Rotate);	//��ʾ��һ������	
			
			GUI_LabelPrintf(LCD_XSIZE-96,80,128,40,RGB565(31,50,00),RGB_TRANS,RGB_TRANS,LEFT,FONT(ASCII8x16,HZ16x16),"������:%d",BlockCount);
			GUI_LabelPrintf(LCD_XSIZE-96,120,128,40,RGB565(31,50,00),RGB_TRANS,RGB_TRANS,LEFT,FONT(ASCII8x16,HZ16x16),"�÷���:%d",UserPoint);
			
			////////	
					
					
			GUI_DisplayUpdate(0,0,LCD_XSIZE,LCD_YSIZE);
			////
			if(!CheckBlockInTray(TetrisMap,xgrd+xoffset,ygrd+1,CurBlock.Index+CurBlock.Rotate))
			{
			
				//�����һ�в��ܷ��¸÷���:
				
				if(CheckMap(TempMap))	//����Ƿ���������
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
			//��ǰλ�ò��ܷ��·���
				
			if(ygrd==0)
			{
			
				
				Delay_n15ms(500/15);
				////
				TS_Free();
				Key_Free();
				
				Tick1000msDecCnt=20;		//20���Ӻ��Զ���������
				while(Tick1000msDecCnt)
				{
					
					if(Tick250msCnt&0x01)
					{
						GUI_LabelPrintf(LCD_XSIZE-128,160,128,32,RGB565(00,00,00),RGB565(24,48,00),RGB565(10,20,10),CENTER,FONT(ASCII8x16,HZ16x16),"��Ϸ���� %d",Tick1000msDecCnt);
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
				//������û�а���.
				GUI_FreeButton(m_button.pBN);
				key=GetKeyValue();
				GUI_MatrixButtonEnable(&m_button);
				GUI_GetMatrixButton(&m_button);
				
			}
			else
			{
				//����������.						
			
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
								//�����ת����Է��¸÷���.
								goto st;
							}
							
							////���ܷ��¸÷���
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
								m_button.pBN[2].Text="��ʼ";
								game_states=GAME_PAUSE;
								
							}
							else
							{
								m_button.pBN[2].Text="��ͣ";
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
