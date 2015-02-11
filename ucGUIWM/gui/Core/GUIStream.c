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
* File    : GUIStream.C
* Purpose : Support for streamed bitmaps
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
#include "GUI.H"



void GUI_DrawStreamedBitmap(const GUI_BITMAP_STREAM *pBitmapHeader, int x, int y) {
  GUI_BITMAP Bitmap = {0};
  GUI_LOGPALETTE Palette;
  Bitmap.BitsPerPixel = pBitmapHeader->BitsPerPixel;
  Bitmap.BytesPerLine = pBitmapHeader->BytesPerLine;
  Bitmap.pData        = ((U8*)pBitmapHeader)+16+4*pBitmapHeader->NumColors;
  Bitmap.pPal         = &Palette;
  Bitmap.XSize        = pBitmapHeader->XSize;
  Bitmap.YSize        = pBitmapHeader->YSize;
  Palette.HasTrans    = pBitmapHeader->HasTrans;
  Palette.NumEntries  = pBitmapHeader->NumColors;
  Palette.pPalEntries = (const LCD_COLOR*)((U8*)pBitmapHeader+16);
  GUI_DrawBitmap(&Bitmap, x, y);
}
