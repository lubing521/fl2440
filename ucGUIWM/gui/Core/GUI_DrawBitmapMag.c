/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
* File    : GUI_DrawBitmapMag.C
* Purpose : Implementation file for GUI_DrawBitmapMag
*
*
* Version-Date---Author-Explanation
*
* 1.00.00 020517 RS     First release
*
*
* Known problems or limitations with current version
*
*    None.
*
*
* Open issues
*
*    None
*********************************************************************************************************
*/

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Protected.H"

void GUI_DrawBitmapMag (const GUI_BITMAP *pBitmap, int x0, int y0, int xMul, int yMul) {
  GUI_DRAWMODE PrevDraw;
  const GUI_LOGPALETTE* pPal;
  GUI_LOCK();
  pPal = pBitmap->pPal;
  PrevDraw = GUI_SetDrawMode((pPal && pPal->HasTrans) ? GUI_DRAWMODE_TRANS : 0);
  #if (GUI_WINSUPPORT)
    WM_ADDORG(x0,y0);
    WM_ITERATE_START(NULL) {
  #endif
  LCD_DrawBitmap(x0,y0
                  ,pBitmap->XSize, pBitmap->YSize
                  ,xMul, yMul
                  ,pBitmap->BitsPerPixel, pBitmap->BytesPerLine
                  ,pBitmap->pData, pBitmap->pPal);
  #if (GUI_WINSUPPORT)
    } WM_ITERATE_END();
  #endif
  GUI_SetDrawMode(PrevDraw);
  GUI_UNLOCK();
}





