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
* File    : GUI_MoveRect.C
* Purpose : Move rectangles
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

#include "GUI_Protected.h"


/*********************************************************************
*
*                    GUI_MoveRect
*
***********************************************************************
*/

void GUI_MoveRect(GUI_RECT *pRect, int x, int y) {
  if (pRect) {
    pRect->x0 += x;
    pRect->x1 += x;
    pRect->y0 += y;
    pRect->y1 += y;
  }
}




