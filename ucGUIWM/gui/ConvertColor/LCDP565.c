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
* File    : LCD565.C
* Purpose : Color conversion routines for uC/GUI
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
*       LCD_FIXEDPALETTE == 565
*
*         2^16 = 65536 colors
*
**********************************************************************
*/

int LCD_Color2Index_565(LCD_COLOR Color) {
  int r,g,b;
  r = (Color>> (0+3))  &0x1f;
  g = (Color>> (8+2)) &0x3f;
  b = (Color>>(16+3)) &0x1f;
  return r+(g<<5)+(b<<11);
}

LCD_COLOR LCD_Index2Color_565(int Index) {
  int r,g,b;
  r = (Index&(0x1f<<0))  << (  3);
  g = (Index&(0x3f<<5))  >> (5-2);
  b = (Index&(0x1f<<11)) >>(11-3);
  return r+(g<<8)+(((U32)b)<<16);
}



