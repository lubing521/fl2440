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
* File    : GUI_AddHex.C
* Purpose : Converts hex value 2 string
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
*        GUI_AddHex
*
**********************************************************************
*/

static const char acHex[] = "0123456789ABCDEF";

void GUI_AddHex(U32 v, U8 Len, char**ps) {
  char *s = *ps;
  if (Len>8) return;
  (*ps) += Len;
  **ps     = '\0';     /* Make sure string is 0-terminated */
  while(Len--) {
    *(s+Len) = acHex[v&15];
    v>>=4;
  }
}

