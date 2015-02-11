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
* File    : GUIKey.C
* Purpose     : Keyboard manager
*               This module is a supplement to uC/GUI.
*               It is required (and will be linked if in you are using
*               a library) if the touch module is being used.
*
*
* Version-Date---Author-Explanation
*
* 1.00    010409 RS     First release
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

/*
          *****************************************************************
          *                                                               *
          *                  Check configuration                          *
          *                                                               *
          *****************************************************************
*/


/*
          *****************************************************************
          *                                                               *
          *              Internal types and declarations                  *
          *                                                               *
          *****************************************************************
*/




/*******************************************************************
*
*                  GUI_GetKey
*
********************************************************************
*/

int GUI_GetKey(void) {
  return GUI_X_GetKey();
}

/*******************************************************************
*
*                  GUI_WaitKey
*
********************************************************************
*/

int GUI_WaitKey(void) {
  return GUI_X_WaitKey();
}

/*******************************************************************
*
*                  GUI_StoreKey
*
********************************************************************
*/

void GUI_StoreKey(int Key) {
  GUI_X_StoreKey(Key);
}

/*******************************************************************
*
*                  GUI_ClearKeyBuffer
*
********************************************************************
*/

void GUI_ClearKeyBuffer(void) {
  while (GUI_GetKey());
}


