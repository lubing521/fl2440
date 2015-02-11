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
* File    : GUITASK.C
* Purpose : Saves/Restores task context with supported OSs.
*           It also uses a resource semaphore.
*
*
* Version-Date---Author-Explanation
*
* 3.00    010425 RS     Cleanup for emWin Version 3.00
*                       a) Dummy routines added to avoid link errors
*                         in case some emWin modules are compiled with
*                         different settings
* 1.00.01 990926 RS     Fix in order to make sure init task has no
*                       context of its own (CurrentTaskNo =-1)
* 1.00.00 990918 RS     Initial version for version control purposes.
*                       This is a generic version which can be used with
*                       both emWin b/w and emWin GSC; it is based on the
*                       Context saver of emWin b/w.
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
#include "GUIDEBUG.H"


/*********************************************************************
 *
 *       Configuration defaults
 *
 *********************************************************************
*/
#ifndef GUI_MAXTASK
  #define GUI_MAXTASK (4)
#endif

/*********************************************************************
 *
 *       externals
 *
 *********************************************************************

  The following externals are used and should typically be defined
  in GUI_X.c:

    U32 GUI_X_GetTaskId();
    void GUI_X_Unlock();
    int GUI_X_Lock();

*/




/*********************************************************************
 *
 *       Generic part  --- Identical for any kernel
 *
 *********************************************************************
*/

#if GUI_OS

/******************    Data     ********************/
static struct {
  U32  TaskID;
  GUI_CONTEXT Context;
} Save[GUI_MAXTASK];

static int CurrentTaskNo =-1;
static int EntranceCnt   = 0;


/******************    Kernel routines     ********************/

static int GetTaskNo(void) {
  int i;
  for (i=0; i< GUI_MAXTASK; i++) {
    U32 TaskId = GUI_X_GetTaskId();
    if (Save[i].TaskID == TaskId)
      return i;
    if (Save[i].TaskID == 0) {
      Save[i].TaskID = TaskId;
      return i;
    }
  }
  GUI_DEBUG_ERROROUT("No Context available for task ... (increase GUI_MAXTASK)");
  return 0;
}

void GUI_Unlock(void) {
  --EntranceCnt;
  GUI_X_Unlock();
}

void GUI_Lock(void) {
  GUI_X_Lock();
  if (++EntranceCnt==1) {
    int TaskNo = GetTaskNo();
    if (TaskNo != CurrentTaskNo) {
      /* Save data of current task */
      if (CurrentTaskNo>=0) {  /* Make sure CurrentTaskNo is valid */
        Save[CurrentTaskNo].Context = GUI_Context;
      }
      /* Load data of this task */
      GUI_Context = Save[TaskNo].Context;
      CurrentTaskNo = TaskNo;
    }
  }
}

void GUITASK_Init(void) {
  int i;
  CurrentTaskNo =-1;   /* Invalidate */
  GUI_X_InitOS();
  for (i=0; i<GUI_MAXTASK; i++) {
    Save[i].Context = GUI_Context;
  }
}



/*
  *****************    Dummy Kernel routines     *******************

The routines below are dummies in case configuration tells us not
to use any kernel. In this case the routines below should
not be required, but it can not hurt to have them. The linker
will eliminate them anyhow.
*/

#else

void GUI_Unlock(void) {}
void GUI_Lock(void) {}
void GUITASK_Init(void) {}
void GUITASK_StoreDefaultContext(void) {}

#endif
