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
* File    : GUI_SetOrg.C
* Purpose : uC/GUI defines the GUI_SetOrg function
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

#include "GUI_Protected.H"
#include "GUIDebug.h"


/***********************************************************
*
*                 GUI_SetOrg
*
************************************************************

*/

void GUI_SetOrg(int x, int y) {
  GUI_LOCK();
  LCD_L0_SetOrg(x, y);
  GUI_UNLOCK();
}









