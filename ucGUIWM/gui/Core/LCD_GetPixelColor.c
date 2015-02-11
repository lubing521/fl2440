/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
* File    : LCD_GetPixelColor.C
* Purpose : Get Pixel routines
*			Note: These routines are in a module of their own
*			      because they are mostly not required to link
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

#include "LCD.H"              /* interface definitions */
#include "LCD_Protected.h"    /* inter modul definitions */



/*********************************************************************
*
*           LCD_GetPixelColor
*
**********************************************************************
*/

LCD_COLOR LCD_GetPixelColor(int x, int y) {
  return LCD_Index2Color(LCD_GetPixelIndex(x,y));
}
