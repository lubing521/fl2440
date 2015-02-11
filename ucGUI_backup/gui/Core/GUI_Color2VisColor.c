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
* File    : GUI_Color2VisColor.C
* Purpose : Implementation of GUI_Color2VisColor
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


/*********************************************************************
*
*        GUI_Color2VisColor
*
**********************************************************************
*/

GUI_COLOR GUI_Color2VisColor(GUI_COLOR color) {
  I16 Index;
  GUI_COLOR r;
  GUI_LOCK();
  Index = LCD_Color2Index(color);
  r = LCD_Index2Color(Index);  
  GUI_LOCK();
  return r;  
}

/*
  *********************************************************
  *
  *        GUI_ColorIsAvailable
  *
  *********************************************************
*/

char GUI_ColorIsAvailable(GUI_COLOR color) {
  return (GUI_Color2VisColor(color) == color) ? 1 : 0;
}

