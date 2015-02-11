/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                           All Rights Reserved
*
*                                           MASTER INCLUDE FILE
*********************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    "os_cpu.h"
#include    "os_cfg.h"
#include    "ucos_ii.h"

#include "44blib.h"
#include "def.h"
#include "44b.h"

//add by san
//#ifndef  OS_MASTER_FILE
//#define  OS_GLOBALS
//#include <ucos_ii.h>
//#endif

#ifdef      EX3_GLOBALS
#define     EX3_EXT
#else
#define     EX3_EXT  extern
#endif

/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef struct {
    char    TaskName[30];
    INT16U  TaskCtr;
    INT16U  TaskExecTime;
    INT32U  TaskTotExecTime;
} TASK_USER_DATA;

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

EX3_EXT  TASK_USER_DATA  TaskUserData[10];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void   DispTaskStat(INT8U id);

#endif //__INCLUDES_H__ end
