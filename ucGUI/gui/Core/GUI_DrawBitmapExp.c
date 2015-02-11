/*
*********************************************************************************************************
*                                                µC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2000, SEGGER Microcontroller Systeme GmbH          
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed 
*              in any way. We appreciate your understanding and fairness.
*
* File        : GUI_DrawBitmapExp.C
* Purpose     : Implementation of GUI_DrawBitmapExp
*********************************************************************************************************
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Protected.H"

/*********************************************************************
*
*             GUI_DrawBitmapExp
*
**********************************************************************
*/

void GUI_DrawBitmapExp(int x0,    int y0,
                       int XSize, int YSize,
                       int XMul,  int YMul,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8* pData,
                       const GUI_LOGPALETTE* pPal)
{
  GUI_DRAWMODE PrevDraw;
  #if (GUI_WINSUPPORT)
  GUI_RECT r;
  #endif
  GUI_LOCK();
  #if (GUI_WINSUPPORT)
    WM_ADDORG(x0,y0);
    r.x0 = x0;
    r.x1 = x0+XSize-1;
    r.y0 = y0;
    r.y1 = y0+YSize-1;
    WM_ITERATE_START(&r); {
  #endif
  PrevDraw = GUI_SetDrawMode((pPal && pPal->HasTrans) ? GUI_DRAWMODE_TRANS : 0);
  LCD_DrawBitmap( x0,y0
                 ,XSize ,YSize
                 ,XMul,YMul
                 ,BitsPerPixel
                 ,BytesPerLine
                 ,pData
                 ,pPal
                 );
  GUI_SetDrawMode(PrevDraw);
  #if (GUI_WINSUPPORT)
    } WM_ITERATE_END();
  #endif
  GUI_UNLOCK();
}
