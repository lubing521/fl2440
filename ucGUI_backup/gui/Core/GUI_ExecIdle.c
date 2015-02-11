/*
*********************************************************************************************************
*                                                µC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2000, SEGGER Microcontroller Systeme GmbH          
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed 
*              in any way. We appreciate your understanding and fairness.
*
* File        : GUI_ExecIdle.c
* Purpose     : 
*********************************************************************************************************
*/


#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Protected.H"


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*     GUI_ExecIdle
*/
int GUI_ExecIdle(void) {
  #if GUI_WINSUPPORT    /* If 0, WM will not generate any code */
    WM_ExecIdle();
  #endif
  /* Execute background jobs */
  if (GUI_pfTimerExec) {
    (*GUI_pfTimerExec)();
  }
  return 0;
}
