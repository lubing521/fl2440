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
* File    : LCDRLE4.C
* Purpose : Drawing routines for run length encoded bitmaps with 4bpp
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
#include "LCD.H"
#include "GUI_Private.h"

#ifndef __C51__ /* Avoid Keil C51 limitation */

void LCD_DrawBitmap_RLE4(int x0,int y0,int xsize, int ysize, const U8*pPixel, const LCD_LOGPALETTE* pLogPal) {
  const LCD_PIXELINDEX* pTrans =NULL;
  char NoTrans = !(GUI_Context.DrawMode & LCD_DRAWMODE_TRANS);
  LCD_PIXELINDEX aColorIndex[2];
  int xi,y, yMax;
  aColorIndex[0] = LCD_ACOLORINDEX[0];
  aColorIndex[1] = LCD_ACOLORINDEX[1];
  /* Handle color translation */
  if ((pLogPal) && (pLogPal->pPalEntries)) {
    if ((pTrans = LCD_GetpPalConvTable(pLogPal)) == NULL) {
      return;
    }
  }
 /* Check if we can limit the number of lines due to clipping) */
  yMax = y0+ysize-1;
  if (yMax > GUI_Context.ClipRect.y1)
    yMax = GUI_Context.ClipRect.y1;
  /* Repeat until we have reached bottom */
  for (xi=0, y=y0; y <= yMax; ) {
    U8 Cmd,Data;
    Cmd= *pPixel++;
    Data = *pPixel++;
    if (Cmd) {
      LCD_SetColorIndex(pTrans ? *(pTrans+Data) : Data);
      while (Cmd) {
        int xi1 = xi+Cmd;
        if (xi1>=xsize)
          xi1 = xsize;
        Cmd -= (xi1-xi);
        if (Data || NoTrans) {  /* Skip transparent pixels */
          LCD_HL_DrawHLine(x0+xi, y, xi1+x0-1);
        }
        xi =xi1;
        if (xi1==xsize) {
          y++;
          xi=0;
        }
      }
    } else {
      while (Data--) {
        U8 Index = *pPixel++;
        if ((Index>>4) || NoTrans) {  /* Skip transparent pixels */
          LCD_SetPixelIndex(x0+xi, y, pTrans ? *(pTrans+(Index>>4)) : (Index>>4));
        }
        if (++xi >= xsize) {
          xi=0; y++;
        }
        if (Data-- == 0)
          break;
        if ((Index&15) || NoTrans) {  /* Skip transparent pixels */
          LCD_SetPixelIndex(x0+xi, y, pTrans ? *(pTrans+(Index&15)) : (Index&15));
        }
        if (++xi >= xsize) {
          xi=0; y++;
        }
      }
    }
  }
  LCD_ACOLORINDEX[0] = aColorIndex[0];
  LCD_ACOLORINDEX[1] = aColorIndex[1];
}

#endif



