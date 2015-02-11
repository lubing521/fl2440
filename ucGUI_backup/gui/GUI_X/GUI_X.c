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
* File        : GUI_X.C
* Purpose     : Config / System dependent externals for GUI
*
* Depending on which parts of µC/GUI software are used,
* some of the routines in this file may have to be modified.
*
* For simple applications, none of these externals are required.
*********************************************************************************************************
*/


#include "GUI.h"

/*********************************************************************
*
*      Timing:
*                 GUI_GetTime()
*                 GUI_Delay(int)
*
**********************************************************************

  Some timing dependent routines of emWin require a GetTime
  and delay funtion. Default time unit (tick), normally is
  1 ms.
  If you use emWin as a simple graphic library without blinking or
  similar functionality, these routines are not required.
*/

int GUI_X_GetTime(void)  { return 0; }
void GUI_X_Delay(int ms) { }

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_Lock()
*                 GUI_X_Unlock()
*                 GUI_X_GetTaskId()
*
**********************************************************************

Note:
  The following routines are required only if emWin is used in a
  true multi task environment, which means you have more than one
  thread using the emWin API.
  In this case the
                      #define GUI_OS 1
  needs to be in GUIConf.h
*/

void GUI_X_InitOS(void)    { }
void GUI_X_Unlock(void)    { }
void GUI_X_Lock(void)      { }
U32  GUI_X_GetTaskId(void) { return 0; }

/*********************************************************************
*
*      Keyboard:
*
*                   GUI_X_GetKey
*                   GUI_X_WaitKey
*                   GUI_X_StoreKey
*
**********************************************************************

  The keyboard routines are required only by some widgets.
  Most smaller applications will not required this functionality
  at all.
*/

static char Key;

int GUI_X_GetKey(void) {
  int r = Key;
  Key = 0;
  return r;
}

int GUI_X_WaitKey(void) {
  while (Key == 0);
  return Key;
}

void GUI_X_StoreKey(int k) {
  Key = k;
}

/******************************************************************************
*
*                    Touch panel
*
*******************************************************************************

  The routines below are dummies because the hardware offers no touch support !
  These routines are used only if configuration says so ... (GUI_SUPPORT_TOUCH)
*/

void TOUCH_X_ActivateX(void) {}
void TOUCH_X_ActivateY(void) {}
int  TOUCH_X_MeasureX(void)  { return 0;}
int  TOUCH_X_MeasureY(void)  { return 0;}

/*********************************************************************
*
*      Logging: OS dependent
*
**********************************************************************

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

*/

void GUI_X_Log(const char *s) { s = s; }
