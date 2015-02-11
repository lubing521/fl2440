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
* File    : LCDGetP.C
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

#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"
#include "GUIDebug.h"



/*********************************************************************
*
*           LCD_GetPixelIndex
*
**********************************************************************

Note: We can not use the standard clipping which we use for drawing
operations as it is perfectly legal to read pixels outside of
the clipping area. We therefor get the bounding rectangle of the
device and use it for clipping.
*/

unsigned LCD_GetPixelIndex(int x, int y)  {
  LCD_RECT r;  
  LCDDEV_L0_GetRect(&r);
  if (x < r.x0)
    return 0;
  if (x > r.x1)
    return 0;
  if (y < r.y0)
    return 0;
  if (y > r.y1)
    return 0;
  return LCDDEV_L0_GetPixelIndex(x,y);
}





