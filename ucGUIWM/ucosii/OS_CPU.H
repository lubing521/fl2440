/*
*********************************************************
* Copyright (c) 2004 上海双实科技有限公司
* All rights reserved.				            
*
* 文件名称：OS_CPU.h
* 文件标识：头文件
* 摘    要：本文件是UCOS-II特性定义头文件，用户应根据实际需要
*           修改此文件。
* 当前版本：1.0
* 作    者：刘征
* 完成日期：2004.7.31
*
* 取代版本：
* 作    者：
* 完成日期：
*********************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif
/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                          All Rights Reserved
*                        (c) Copyright ARM Limited 1999.  All rights reserved.
*
*                                          ARM Specific code
*
* File : OS_CPU.H
*********************************************************************************************************
*/

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */


//chy 2003-01-22 int is 32bit  short is 16bit
typedef unsigned short   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short   INT16S;                   /* Signed   16 bit quantity                           */
//------------------------------------------------------------------------

typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef INT16U         OS_FLAGS;                 /* Date type for event flag bits (8, 16 or 32 bits)             */

//#define BYTE           INT8S                     /* Define data types for backward compatibility ...   */
//#define UBYTE          INT8U                     /* ... to uC/OS V1.xx.  Not actually needed for ...   */
//#define WORD           INT16S                    /* ... uC/OS-II.                                      */
//#define UWORD          INT16U
//#define LONG           INT32S
//#define ULONG          INT32U

/* 
*********************************************************************************************************
*                              ARM, various architectures
*
*********************************************************************************************************
*/

#define OS_CRITICAL_METHOD 2
#define	OS_ENTER_CRITICAL()	__asm{bl ARMDisableInt}//ARMDisableInt()
#define	OS_EXIT_CRITICAL()	__asm{bl ARMEnableInt}//ARMEnableInt()

/*
 * Definitions specific to ARM/uHAL
 */
#define	SVC32MODE	0x13

/* stack stuff */
#define OS_STK_GROWTH    1

/* angel takes up stack */
#define SEMIHOSTED_STACK_NEEDS 1024

/* idle task stack size (words) */
#ifdef SEMIHOSTED
#define OS_IDLE_STK_SIZE        (32+SEMIHOSTED_STACK_NEEDS)
#else
#define OS_IDLE_STK_SIZE        32
#endif

#define  OS_TASK_SW()         OSCtxSw()

/* defined in os_cpu_a.s */
void OSCtxSw(void);
void OSIntCtxSw(void);

extern void ARMDisableInt(void);
extern void ARMEnableInt(void);

#ifdef __cplusplus
}
#endif
