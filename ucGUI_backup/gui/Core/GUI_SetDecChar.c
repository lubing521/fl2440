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
* File    : GUI_SetDecChar.C
* Purpose : Routines to set the character used for decimal point
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

#include "GUI_Protected.H"
#include "GUIDebug.h"

/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/


/*********************************************************************
*
*     GUI_SetDecChar
*/

char GUI_SetDecChar(char c) {
  char r = GUI_DecChar;
  GUI_DecChar = c;
  return r;
}

char GUI_GetDecChar(void) {
  return GUI_DecChar;
}



