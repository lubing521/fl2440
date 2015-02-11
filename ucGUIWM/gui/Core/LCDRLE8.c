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
* File    : LCDRLE8.C
* Purpose : Drawing routines for run length encoded bitmaps with 8 bpp
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
#include "GUI_Private.H"
#include "LCD_Private.H"

static struct {
  int x,y;
  const U8* pPixel;
  const U8* pPixelStart;
} Cache;

void LCD_DrawBitmap_RLE8 (int x0,int y0,int xsize, int ysize, const U8*pPixel, const LCD_LOGPALETTE* pLogPal) {
  LCD_PIXELINDEX aColorIndex[2];
  int xi,y, yMax;
  const U8* pPixelOrg = pPixel;
  char NoTrans = !(GUI_Context.DrawMode & LCD_DRAWMODE_TRANS);
  const LCD_PIXELINDEX* pTrans =NULL;
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
  /* Init variables for looping */
  xi=0;
  y =0;
  /* Check if we can use the cache to save some unnecessary iterations */
  {
    int yDiff = GUI_Context.ClipRect.y0 - y0;
    if ((Cache.pPixelStart == pPixel) && (yDiff > Cache.y)) {
      /* Accept cache values */
      y = Cache.y;
      xi = Cache.x;
      pPixel = Cache.pPixel;
    }
  }
  /* Init values for caching */
  Cache.pPixel = Cache.pPixelStart = pPixelOrg;
  Cache.x = Cache.y = 0;
  y += y0;
  /* Repeat until we have reached bottom */
  for (; y <= yMax; ) {
    U8 Cmd  = *pPixel++;
    U8 Data = *pPixel++;
    if (Cmd) {
      /* Save cache info */
      Cache.pPixel = pPixel-2;
      Cache.x = xi;
      Cache.y = y-y0;
      LCD_ACOLORINDEX[1] = pTrans ? *(pTrans+Data) : Data;
      while (Cmd) {
        int xi1 = xi+Cmd;
        if (xi1>=xsize)
          xi1 = xsize;
        Cmd -= (xi1-xi);
        if (Data || NoTrans) {  /* Skip transparent pixels */
          LCD_DrawHLine(x0+xi, y, xi1+x0-1);
        }
        xi =xi1;
        if (xi1==xsize) {
          y++;
          xi=0;
        }
      }
    } else {
      do {
        U8 Index = *pPixel++;
        if (Index || NoTrans) {  /* Skip transparent pixels */
          int x = x0+xi;
          #if 1 /* High speed variant */
            if (y >= GUI_Context.ClipRect.y0)
              if (x >= GUI_Context.ClipRect.x0)
                if (x <= GUI_Context.ClipRect.x1)
                  LCDDEV_L0_SetPixelIndex(x, y, pTrans ? *(pTrans+Index) : Index);
          #else
            LCD_SetPixelIndex(x, y, pTrans ? *(pTrans+Index) : Index);
          #endif
        }
        if (++xi > xsize) {
          xi=0; y++;
          if (y > GUI_Context.ClipRect.y1)
            break;
        }
      } while (--Data);
    }
  }
  LCD_ACOLORINDEX[0] = aColorIndex[0];
  LCD_ACOLORINDEX[1] = aColorIndex[1];
}



