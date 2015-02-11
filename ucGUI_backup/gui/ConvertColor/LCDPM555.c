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
* File    : LCD555.C
* Purpose : Color conversion routines for LCD-drivers
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
#include "LCD_Protected.h"    /* inter modul definitions */

/*********************************************************************
*
*       LCD_FIXEDPALETTE == -555
*
*         2^15 = 32768 basic colors
*
**********************************************************************
*/

int LCD_Color2Index_M555(LCD_COLOR Color) {
  int r,g,b;
  r = (Color>>3)  &0x1f;
  g = (Color>>11) &0x1f;
  b = (Color>>19) &0x1f;
  return b+(g<<5)+(r<<10);
}

LCD_COLOR LCD_Index2Color_M555(int Index) {
  int r,g,b;
  b = (Index&(0x1f<<0))  << (  3);
  g = (Index&(0x1f<<5))  >> (5-3);
  r = (Index&(0x1f<<10)) >>(10-3);
  return r+(g<<8)+(((U32)b)<<16);
}


