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
* File    : GUITime.C
* Purpose : Time related routines
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


/*********************************************************
*
*                 GUI_GetTime()
*
**********************************************************
*/
int GUI_GetTime(void) {
  return GUI_X_GetTime();
}

/*********************************************************
*
*                 GUI_Delay
*
*     Executes background (timer and WM) for specified
*     time
*
**********************************************************
*/

void GUI_Delay(int Period) {
  int EndTime = GUI_GetTime()+Period;
  int tRem; /* remaining Time */
  while (tRem = EndTime- GUI_GetTime(), tRem>0) {
    /* Execute background jobs */
    if (GUI_pfTimerExec) {
      (*GUI_pfTimerExec)();
    }
    if (WM_pfExecIdle) {
      (*WM_pfExecIdle)();
    }
    GUI_X_Delay((tRem >5) ? 5 : tRem);
  }
}









