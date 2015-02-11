/**************************************************************
The initial and control for 320��240 16Bpp TFT LCD----LCD_LTV350QV-F04
**************************************************************/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//#include "option.h"
#include "2440addr.h"
//#include "2440lib.h"
//#include "2440slib.h" 
#include "lcd.h"

#define MVAL		(13)
#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define INVVDEN		(1)		//0=normal       1=inverted
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control

#define M5D(n) ((n) & 0x1fffff)	// To get lower 21bits

//TFT 240320


#define SCR_XSIZE_TFT_240320 	(LCD_XSIZE_TFT_240320)
#define SCR_YSIZE_TFT_240320 	(LCD_YSIZE_TFT_240320)

//TFT240320
#define HOZVAL_TFT_240320	(LCD_XSIZE_TFT_240320-1)
#define LINEVAL_TFT_240320	(LCD_YSIZE_TFT_240320-1)

//Timing parameter for LCD LQ035Q7DB02
#define VBPD_240320		(3)		//��ֱͬ���źŵĺ��1
#define VFPD_240320		(5)		//��ֱͬ���źŵ�ǰ��5
#define VSPW_240320		(15)		//��ֱͬ���źŵ�����1

#define HBPD_240320		(58)		//ˮƽͬ���źŵĺ��35
#define HFPD_240320		(15)		//ˮƽͬ���źŵ�ǰ��19
#define HSPW_240320		(8)		//ˮƽͬ���źŵ�����5

#define CLKVAL_TFT_240320	(4) 	
//FCLK=180MHz,HCLK=90MHz,VCLK=6.5MHz

// GPB1/TOUT1 for Backlight control(PWM)
#define GPB1_TO_OUT()   (rGPBUP &= 0xfffd, rGPBCON &= 0xfffffff3, rGPBCON |= 0x00000004)
#define GPB1_TO_1()     (rGPBDAT |= 0x0002)
#define GPB1_TO_0()     (rGPBDAT &= 0xfffd)


unsigned short LCD_BUFFER[SCR_YSIZE_TFT_240320][SCR_XSIZE_TFT_240320];//240*320

extern unsigned char __CHS[];
extern unsigned char __VGA[];

extern int SET_IF(void );
extern int CLR_IF(void );

/**************************************************************
320��240 16Bpp TFT LCD����ģ���ʼ��
**************************************************************/

void Lcd_Init(void)
{
    rGPCUP  = 0x00000000;
	 rGPCCON = 0xaaaa02a9; 
	 
//    rGPDUP=0xffffffff; // Disable Pull-up register
     rGPDUP  = 0x00000000;
   rGPDCON=0xaaaaaaaa; //Initialize VD[15:8]

	rLCDCON1=(CLKVAL_TFT_240320<<8)|(MVAL_USED<<7)|(3<<5)|(12<<1)|0;
    	// TFT LCD panel,16bpp TFT,ENVID=off
	rLCDCON2=(VBPD_240320<<24)|(LINEVAL_TFT_240320<<14)|(VFPD_240320<<6)|(VSPW_240320);
	rLCDCON3=(HBPD_240320<<19)|(HOZVAL_TFT_240320<<8)|(HFPD_240320);
	rLCDCON4=(MVAL<<8)|(HSPW_240320);
    rLCDCON5 = (1<<11) | (1<<10) | (1<<9) | (1<<8) | (0<<7) | (0<<6)
             | (1<<3)  |(BSWP<<1) | (HWSWP);
             //5:6:5 VCLK posedge BSWP=0,HWSWP=1;

	rLCDSADDR1=(((U32)LCD_BUFFER>>22)<<21)|M5D((U32)LCD_BUFFER>>1);
	rLCDSADDR2=M5D( ((U32)LCD_BUFFER+(SCR_XSIZE_TFT_240320*LCD_YSIZE_TFT_240320*2))>>1 );
	rLCDSADDR3=(((SCR_XSIZE_TFT_240320-LCD_XSIZE_TFT_240320)/1)<<11)|(LCD_XSIZE_TFT_240320/1);
	rLCDINTMSK|=(3); // MASK LCD Sub Interrupt
    rTCONSEL &= (~7) ;     // Disable LPC3480
	rTPAL=0; // Disable Temp Palette
}

/**************************************************************
LCD��Ƶ�Ϳ����ź��������ֹͣ��1������Ƶ���
**************************************************************/
void Lcd_EnvidOnOff(int onoff)
{
    if(onoff==1)
	rLCDCON1|=1; // ENVID=ON
    else
	rLCDCON1 =rLCDCON1 & 0x3fffe; // ENVID Off
}

/**************************************************************
320��240 16Bpp TFT LCD ��Դ��������ʹ��
**************************************************************/
void Lcd_PowerEnable(int invpwren,int pwren)
{
    //GPG4 is setted as LCD_PWREN
    rGPGUP=rGPGUP&(~(1<<4))|(1<<4); // Pull-up disable
    rGPGCON=rGPGCON&(~(3<<8))|(3<<8); //GPG4=LCD_PWREN
    rGPGDAT = rGPGDAT | (1<<4) ;
//	invpwren=pwren;
    //Enable LCD POWER ENABLE Function
    rLCDCON5=rLCDCON5&(~(1<<3))|(pwren<<3);   // PWREN
    rLCDCON5=rLCDCON5&(~(1<<5))|(invpwren<<5);   // INVPWREN
}

/**************************************************************
320��240 16Bpp TFT LCD�������ص���ʾ�������
**************************************************************/
void PutPixel(U32 x,U32 y, U16 c )
{
	if ( (x < SCR_XSIZE_TFT_240320) && (y < SCR_YSIZE_TFT_240320) )
		LCD_BUFFER[(y)][(x)] = c;
}
 
void GUI_Point(U32 x,U32 y, U16 c )
{
	if ( (x < SCR_XSIZE_TFT_240320) && (y < SCR_YSIZE_TFT_240320) )
		LCD_BUFFER[(y)][(x)] = c;
}

/**************************************************************
320��240 16Bpp TFT LCDȫ������ض���ɫ��Ԫ������
**************************************************************/
void Lcd_ClearScr( U16 c)
{
	unsigned int x,y ;
		
    for( y = 0 ; y < SCR_YSIZE_TFT_240320 ; y++ )
    {
    	for( x = 0 ; x < SCR_XSIZE_TFT_240320 ; x++ )
    	{
			LCD_BUFFER[y][x] = c ;
    	}
    }
}

/**************************************************************
LCD��Ļ��ʾ��ֱ��ת
// LCD display is flipped vertically
// But, think the algorithm by mathematics point.
//   3I2
//   4 I 1
//  --+--   <-8 octants  mathematical cordinate
//   5 I 8
//   6I7
**************************************************************/
 void Glib_Line(int x1,int y1,int x2,int y2,int color)
{
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}

/**************************************************************
��LCD��Ļ�ϻ�һ������
**************************************************************/
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color)
{
    Glib_Line(x1,y1,x2,y1,color);
    Glib_Line(x2,y1,x2,y2,color);
    Glib_Line(x1,y2,x2,y2,color);
    Glib_Line(x1,y1,x1,y2,color);
}

/**************************************************************
��LCD��Ļ������ɫ���һ������
**************************************************************/
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color)
{
    int i;

    for(i=y1;i<=y2;i++)
	Glib_Line(x1,i,x2,i,color);
}

/**************************************************************
��LCD��Ļ��ָ������㻭һ��ָ����С��ͼƬ
**************************************************************/
void Paint_Bmp(int x0,int y0,int h,int l,unsigned char bmp[])
{
	int x,y;
	U32 c;
	int p = 0;
	
    for( y = y0 ; y < l ; y++ )
    {
    	for( x = x0 ; x < h ; x++ )
    	{
    		c = bmp[p+1] | (bmp[p]<<8) ;

			if ( ( (x0+x) < SCR_XSIZE_TFT_240320) && ( (y0+y) < SCR_YSIZE_TFT_240320) )
				LCD_BUFFER[y0+y][x0+x] = c ;
			
    		p = p + 2 ;
    	}
    }
}

/**************************************************************
320��240 16Bpp TFT LCD�ƶ��۲촰��
**************************************************************/
void Lcd_MoveViewPort(int vx,int vy)
{
    U32 addr;

    SET_IF(); 
	#if (LCD_XSIZE_TFT_240320<32)
    	    while((rLCDCON1>>18)<=1); // if x<32
	#else	
    	    while((rLCDCON1>>18)==0); // if x>32
	#endif
    
    addr=(U32)LCD_BUFFER+(vx*2)+vy*(SCR_XSIZE_TFT_240320*2);
	rLCDSADDR1= ( (addr>>22)<<21 ) | M5D(addr>>1);
	rLCDSADDR2= M5D(((addr+(SCR_XSIZE_TFT_240320*LCD_YSIZE_TFT_240320*2))>>1));
	CLR_IF();
}

/**************************************************************
��LCD��Ļ��ָ�������дASCII��
**************************************************************/
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	unsigned char *pZK,mask,buf;
	
	

	pZK = &__VGA[ch*16];
	for( i = 0 ; i < 16 ; i++ )
	{
		mask = 0x80;
		buf = pZK[i];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j,y+i,bk_c);
                }
            }
            
            mask = mask >> 1;
        }
	}
}

/**************************************************************
��LCD��Ļ��ָ�������д����
**************************************************************/
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	unsigned char *pZK,mask,buf;

	pZK = &__CHS[ (  ( (QW >> 8) - 1 )*94 + (QW & 0x00FF)- 1 )*32 ];
	for( i = 0 ; i < 16 ; i++ )
	{
		//��
		mask = 0x80;
        buf = pZK[i*2];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j,y+i,bk_c);
                }
            }
            mask = mask >> 1;
        } 
        
		//��
		mask = 0x80;
        buf = pZK[i*2 + 1];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                PutPixel(x+j + 8,y+i,c);
            }else
            {
                if( !st )
                {
                    PutPixel(x+j + 8,y+i,bk_c);
                }
            }
            mask = mask >> 1;
        }                 
	}
}

//----------------------
void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...)
{
    char __LCD_Printf_Buf[256];
    va_list ap;
	unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;

    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
	 
    while(*pStr != 0 )
	{
		switch(*pStr)
		{
			case '\n' :
				{
			
                    break;
				}

			default:
				{
					if( *pStr > 0xA0 & *(pStr+1) > 0xA0 )  //�������
                    {
                        Lcd_PutHZ( x , y , (*pStr - 0xA0)*0x0100 + *(pStr+1) - 0xA0 , c , bk_c , st);

                        pStr++;
                        i++;

                        x += 16;
                    }else               //Ӣ�����
                    {
                        Lcd_PutASCII( x , y , *pStr , c , bk_c , st );

                        x += 8;

                    }

                    break;
				}
		}
		
		pStr++;
        i++;		

        if( i > 256 ) break;
	}
   
}

    