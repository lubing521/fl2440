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
* File    : GUI_Private.H
* Purpose : GUI internal declarations
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
*
*
* Attention : Do not modify this file ! If you do, you will not
*             be able do update to a later version of uC/GUI!
*
*********************************************************************************************************
*/

#ifndef  GUI_PRIVATE_H
#define  GUI_PRIVATE_H
//added by weicz
//#include "GUI.h"

//added

#include "GUI_Protected.h"
#include "LCD_Private.h"   /* Required because of LCD_PIXELINDEX, which depends on LCDConf */

#if GUI_SUPPORT_MEMDEV
  #define LCDDEV_L0_Color2Index         GUI_Context.pDeviceAPI->pfColor2Index
  #define LCDDEV_L0_DrawBitmap          GUI_Context.pDeviceAPI->pfDrawBitmap
  #define LCDDEV_L0_DrawHLine           GUI_Context.pDeviceAPI->pfDrawHLine
  #define LCDDEV_L0_DrawVLine           GUI_Context.pDeviceAPI->pfDrawVLine
  #define LCDDEV_L0_DrawPixel           GUI_Context.pDeviceAPI->pfDrawPixel
  #define LCDDEV_L0_FillRect            GUI_Context.pDeviceAPI->pfFillRect
  #define LCDDEV_L0_GetPixel            GUI_Context.pDeviceAPI->pfGetPixel
  #define LCDDEV_L0_GetRect             GUI_Context.pDeviceAPI->pfGetRect
  #define LCDDEV_L0_GetPixelIndex       GUI_Context.pDeviceAPI->pfGetPixelIndex
  #define LCDDEV_L0_Index2Color         GUI_Context.pDeviceAPI->pfIndex2Color
  #define LCDDEV_L0_SetPixelIndex       GUI_Context.pDeviceAPI->pfSetPixelIndex
  #define LCDDEV_L0_XorPixel            GUI_Context.pDeviceAPI->pfXorPixel
#else
  #define LCDDEV_L0_Color2Index         LCD_L0_Color2Index
  #define LCDDEV_L0_DrawBitmap          LCD_L0_DrawBitmap
  #define LCDDEV_L0_DrawHLine           LCD_L0_DrawHLine
  #define LCDDEV_L0_DrawVLine           LCD_L0_DrawVLine
  #define LCDDEV_L0_DrawPixel           LCD_L0_DrawPixel
  #define LCDDEV_L0_FillRect            LCD_L0_FillRect
  #define LCDDEV_L0_GetPixel            LCD_L0_GetPixel
  #define LCDDEV_L0_GetRect             LCD_L0_GetRect
  #define LCDDEV_L0_GetPixelIndex       LCD_L0_GetPixelIndex
  #define LCDDEV_L0_Index2Color         LCD_L0_Index2Color
  #define LCDDEV_L0_SetPixelIndex       LCD_L0_SetPixelIndex
  #define LCDDEV_L0_XorPixel            LCD_L0_XorPixel
#endif




tLCDDEV_DrawBitmap           LCD_L0_DrawBitmap;
tLCDDEV_DrawBitmap           LCD_L0_1_DrawBitmap;
tLCDDEV_DrawBitmap           LCD_L0_MAG_DrawBitmap;

LCD_PIXELINDEX*  LCD_GetpPalConvTable(const LCD_LOGPALETTE*  pLogPal);
LCD_PIXELINDEX*  LCD_GetpPalConvTableUncached(const LCD_LOGPALETTE*  pLogPal);
LCD_PIXELINDEX*  GUI_MEMDEV_XY2PTR(int x,int y);



#endif   /* ifdef GUI_H */
