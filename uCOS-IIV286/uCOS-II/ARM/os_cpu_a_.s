;*
;* File: os_cpu_a.s
;*
;*            (c) Copyright ARM Limited 1999.  All rights reserved. 
;*
;*                               ARM Specific code
;*
;*
; 
;	Functions defined in this module:
;
;	void ARMDisableInt(void)	/* disable interrupts when in SVC */
;	void ARMEnableInt(void)		/* enable interrupts when in SVC */
;	void OS_TASK_SWAP(void)		/* context switch */
;	void OSStartHighRdy(void)	/* start highest priority task */

SwiV		EQU	0x08
IrqV		EQU	0x18
FiqV		EQU	0x1C
NoInt		EQU	0x80

SVC32Mode	EQU	0x13
IRQ32Mode	EQU	0x12
FIQ32Mode	EQU	0x11

OSEnterSWI	EQU	0x00


BIT_TIMER0	EQU		(0x1<<13)
I_ISPC		EQU		0x1e00024
INTMSK		EQU		0x1e0000c



	AREA	|subr|, CODE, READONLY

	; External symbols we need the addresses of
			IMPORT	OSTCBCur
addr_OSTCBCur		DCD	OSTCBCur
			IMPORT	OSTCBHighRdy
addr_OSTCBHighRdy	DCD	OSTCBHighRdy
			IMPORT	OSPrioCur
addr_OSPrioCur		DCD	OSPrioCur
			IMPORT	OSPrioHighRdy
addr_OSPrioHighRdy	DCD	OSPrioHighRdy

	IMPORT	need_to_swap_context
	IMPORT	IrqStart
	IMPORT	OSTimeTick
	IMPORT	IrqFinish

	IMPORT	OutDebug
	IMPORT	BreakPoint

	EXPORT	IRQContextSwap
	EXPORT	TickHandler
	
IRQContextSwap
	MOV	a1, #1
	MOV	lr, pc
	
TickHandler
	STMDB    sp!,{r0-r11,lr}

	;interrupt disable(not nessary)
	mrs		r0, CPSR
	orr		r0, r0, #0x80		; and set IRQ disable flag
	msr		CPSR_cxsf, r0
	
	;End of interrupt
	;(Clear pending bit of INTPEND that don't accessed it.)
	;	rI_ISPC= BIT_TIMER0;
	LDR	r0, =I_ISPC
	LDR	r1, =BIT_TIMER0
	STR	r1, [r0]

	BL	IrqStart
	
	BL	OSTimeTick

	BL	IrqFinish		;a1= return value	0:not context switch, otherwise:context switch

	CMP		a1, #0
	LDRNE	pc, =_CON_SWAP


_NOT_CON_SWAP
	;not context switching
	LDMIA    sp!,{r0-r11, lr}
	SUBS	pc, lr, #4


_CON_SWAP
	;now context switching
	LDMIA    sp!,{r0-r11,lr}
	SUB		lr, lr, #4
	STR		lr, SAVED_LR		;STR lr, [pc, #SAVED_LR-.-8]


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Change Supervisor mode
	;!!!r12 register don't preserved. (r12 that PC of task)

	MRS             lr, SPSR
	AND				lr, lr, #0xFFFFFFE0
	ORR				lr, lr, #0x13
	MSR             CPSR_cxsf, lr


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Now  Supervisor mode
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	STR		r12, [sp, #-8]	; saved r12
	LDR		r12, SAVED_LR	;LDR r12, [pc, #SAVED_LR-.-8]
	STMFD	sp!, {r12}		; r12 that PC of task
	SUB		sp, sp, #4		; inclease stack point
	LDMIA	sp!, {r12}		; restore r12
	STMFD	sp!, {lr}		; save lr
	STMFD	sp!, {r0-r12}	; save register file and ret address
	MRS		r4, CPSR
	STMFD	sp!, {r4}		; save current PSR
	MRS		r4, SPSR		; YYY+
	STMFD	sp!, {r4}		; YYY+ save SPSR


	; OSPrioCur = OSPrioHighRdy
	LDR	r4, addr_OSPrioCur
	LDR	r5, addr_OSPrioHighRdy
	LDRB	r6, [r5]
	STRB	r6, [r4]
	
	; Get current task TCB address
	LDR	r4, addr_OSTCBCur
	LDR	r5, [r4]
	STR	sp, [r5]		; store sp in preempted tasks's TCB

	; Get highest priority task TCB address
	LDR	r6, addr_OSTCBHighRdy
	LDR	r6, [r6]
	LDR	sp, [r6]		; get new task's stack pointer

	; OSTCBCur = OSTCBHighRdy
	STR	r6, [r4]		; set new current task TCB address

	LDMFD	sp!, {r4}		; YYY+
;	AND		r4, r4, #0xFFFFFF20
;	ORR		r4, r4, #0x13
	MSR	SPSR_cxsf, r4		; YYY+
	LDMFD	sp!, {r4}		; YYY+
;	AND		r4, r4, #0xFFFFFF20
;	ORR		r4, r4, #0x13
	MSR	CPSR_cxsf, r4		; YYY+
	LDMFD	sp!, {r0-r12, lr, pc}	; YYY+


SAVED_LR		DCD		0


;	void DisableInt(void)
;	void EnableInt(void)
;
;	Disable and enable IRQ and FIQ preserving current CPU mode.
;
	EXPORT	ARMDisableInt
ARMDisableInt
	STMDB	sp!, {r0}
	MRS		r0, CPSR
	ORR		r0, r0, #NoInt
	MSR		CPSR_cxsf, r0
	LDMIA	sp!, {r0}
	MOV	pc, lr


	EXPORT	ARMEnableInt
ARMEnableInt
	STMDB	sp!, {r0}
	MRS	r0, CPSR
	BIC	r0, r0, #NoInt
	MSR	CPSR_cxsf, r0
	LDMIA	sp!, {r0}
	MOV	pc, lr

	EXPORT	ARMIsDisableInt
ARMIsDisableInt		;return value [disable: 1      enable: 0]
	MRS	a1, CPSR
	AND	a1, a1, #NoInt
	MOV	pc, lr		


;	void OS_TASK_SW(void)
;	
;	Perform a context switch.
;
;	On entry, OSTCBCur and OSPrioCur hold the current TCB and priority
;	and OSTCBHighRdy and OSPrioHighRdy contain the same for the task
;	to be switched to.
;
;	The following code assumes that the virtual memory is directly
;	mapped into  physical memory. If this is not true, the cache must 
;	be flushed at context switch to avoid address aliasing.
	EXPORT	OS_TASK_SW
OS_TASK_SW
	STMFD	sp!, {lr}		; save pc
	STMFD	sp!, {lr}		; save lr
	STMFD	sp!, {r0-r12}	; save register file and ret address
	MRS	r4, CPSR
	STMFD	sp!, {r4}		; save current PSR
	MRS	r4, SPSR		; YYY+
	STMFD	sp!, {r4}		; YYY+ save SPSR

	; OSPrioCur = OSPrioHighRdy
	LDR	r4, addr_OSPrioCur
	LDR	r5, addr_OSPrioHighRdy
	LDRB	r6, [r5]
	STRB	r6, [r4]
	
	; Get current task TCB address
	LDR	r4, addr_OSTCBCur
	LDR	r5, [r4]
	STR	sp, [r5]		; store sp in preempted tasks's TCB

	; Get highest priority task TCB address
	LDR	r6, addr_OSTCBHighRdy
	LDR	r6, [r6]
	LDR	sp, [r6]		; get new task's stack pointer

	; OSTCBCur = OSTCBHighRdy
	STR	r6, [r4]		; set new current task TCB address

	LDMFD	sp!, {r4}		; YYY+
	MSR	SPSR_cxsf, r4		; YYY+
	LDMFD	sp!, {r4}		; YYY+
	MSR	CPSR_cxsf, r4		; YYY+
	LDMFD	sp!, {r0-r12, lr, pc}	; YYY+



;	void OSStartHighRdy(void)
;	
;	Start the task with the highest priority;
;
	EXPORT	OSStartHighRdy
OSStartHighRdy
	LDR	r4, addr_OSTCBCur	; Get current task TCB address
	LDR	r5, addr_OSTCBHighRdy	; Get highest priority task TCB address

	LDR	r5, [r5]		; get stack pointer
	LDR	sp, [r5]		; switch to the new stack

	STR	r5, [r4]		; set new current task TCB address

	LDMFD	sp!, {r4}		; YYY
	MSR	SPSR_cxsf, r4
	LDMFD	sp!, {r4}		; get new state from top of the stack
	MSR	CPSR_cxsf, r4		; CPSR should be SVC32Mode
	LDMFD	sp!, {r0-r12, lr, pc }	; start the new task

	END
