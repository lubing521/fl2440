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
* File    : GUI_GetClientRect.C
* Purpose : Core file
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
#include "GUIDebug.h"

/*********************************************************************
*
*            GUI_GetClientRect
*
**********************************************************************
*/

void GUI_GetClientRect   (GUI_RECT* pRect) {
  if (!pRect)
    return;
  #if GUI_WINSUPPORT
    WM_GetClientRect(pRect);
  #else
    pRect->x0 = 0;
    pRect->y0 = 0;
    pRect->x1 = LCD_GET_XSIZE();
    pRect->y1 = LCD_GET_YSIZE();
  #endif
}




