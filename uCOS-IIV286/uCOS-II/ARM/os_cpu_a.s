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

	AREA	|subr|, CODE, READONLY

	; Improper use of locations within a READONLY area
SavedIRQ	DCD	0x0
SavedFIQ	DCD	0x0
SavedSWI	DCD	0x0

	; External symbols we need the addresses of
			IMPORT	OSTCBCur
addr_OSTCBCur		DCD	OSTCBCur
			IMPORT	OSTCBHighRdy
addr_OSTCBHighRdy	DCD	OSTCBHighRdy
			IMPORT	OSPrioCur
addr_OSPrioCur		DCD	OSPrioCur
			IMPORT	OSPrioHighRdy
addr_OSPrioHighRdy	DCD	OSPrioHighRdy
	
	EXPORT IRQContextSwap
IRQContextSwap
	; NOTE: The following code assumes that all threads use r13 as
	; the stack-pointer, and that it is a APCS conformant stack.
	; i.e. there is never any data stored beneath the current
	; stack-pointer.
	; The above needs to be true to enable the context switches
	; started as a return from an interrupt to use the current
	; threads stack as the state save area.
	;
	LDMFD	sp!,{r12}		; recover SPSR value from stack
	; if NE then we need to check if we were a nested interrupt
	AND	r11,r12,#0x1F		; mask out all but the mode bits
	TEQNE	r11,#IRQ32Mode		; check for interrupted IRQ thread
	; if EQ then we can return immediately
	MSREQ	SPSR_cxsf,r12		; restore the SPSR
	LDMEQFD	sp!,{r0-r12,pc}^	; and return to the interrupted thread

	; We now need to perform a context switch.
	; r12 = SPSR describing the interrupted thread.
	; r11 = interrupted thread processor mode

	; We need to protect the SPSR before we actually perform the
	; return to the interrupted thread, since we don't want to
	; lose the value by another interrupt occuring between the
	; SPSR load and the PC+CPSR load. Similarly we need to protect
	; the IRQ stack and threading code while we setup the state
	; required to enter the context switch code from an interrupt
	; routine. We rely on the interrupted thread having IRQs
	; enabled (since we would never have reached this point
	; otherwise).
	; We have recovered the SPSR value, so only r0-r12,lr are on the stack.

	LDR	r4,[sp,#(13 * 4)]	; load return address: nasty use of
					; a constant

	; r11 contains the mode bits describing the interrupted thread
	MRS	r0,CPSR			; get current mode info.
	ORR	r0,r0,#0x80		; and set IRQ disable flag
	BIC	r1,r0,#0x1F		; clear mode bits
	ORR	r1,r1,r11		; insert interrupted thread mode bits
	MSR	CPSR_cxsf,r1	; and change to that mode

	; We are now in the interrupted thread mode with IRQs
	; disabled.
	MOV	r3,lr			; copy the current lr
	MRS	r1,SPSR			; copy current SPSR
	MRS	r2,CPSR			; copy current CPSR
	STMFD	sp!,{r1,r2,r3,r4}	; and construct return stack
	MSR	CPSR_cxsf,r0			; return to IRQ mode
	; IRQ mode; IRQs disabled
	; r12 = SPSR describing interrupted thread
	MSR	SPSR_cxsf,r12			; restore SPSR_irq ready for return
	LDMFD	sp!,{r0-r12,lr}		; restore all the registers
	SUBS	pc,pc,#0			; and return to the interrupted mode
	NOP							; flush the pipeline
	NOP
	NOP

	; we are now executing in the interrupted mode with IRQs enabled
	BL	OS_TASK_SW		; perform the context switch
	LDMFD	sp!,{lr}
	MSR	SPSR_cxsf,lr	; recover the SPSR when the thread 
						; was interrupted
	LDMFD	sp!,{lr}
	MSR	CPSR_cxsf,lr	; recover the CPSR when the thread 
						; was interrupted
	LDMFD	sp!,{lr,pc}	; return to the interrupted thread


;	void DisableInt(void)
;	void EnableInt(void)
;
;	Disable and enable IRQ and FIQ preserving current CPU mode.
;
	EXPORT	ARMDisableInt
ARMDisableInt
	MRS	r12, CPSR
	ORR	r12, r12, #NoInt
	MSR	CPSR_cxsf, r12
	MOV	pc, lr

	EXPORT	ARMEnableInt
ARMEnableInt
	MRS	r12, CPSR
	BIC	r12, r12, #NoInt
	MSR	CPSR_cxsf, r12
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
	STMFD	sp!, {r0-r12, lr}	; save register file and ret address
	MRS	r4, CPSR
	STMFD	sp!, {r4}			; save current PSR
	MRS	r4, SPSR				; YYY+
	STMFD	sp!, {r4}			; YYY+ save SPSR

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
	LDMFD	sp!, {r0-r12, pc}	; YYY+

;	EXPORT OSIntCtxSw
;OSIntCtxSw
;	add	r7,sp,#16
;	LDR	sp,=IRQStack
;	MRS	r1,spsr
;	ORR	r1,r1,#0xc0
;	MSR	CPSR_cxsf,r1
;	LDR	r0,[r7,#52]
;	SUB	r0,r0,#4
;	STMFD	sp!{r0}
;	STMFD	sp!{lr}
;	MOV	lr,r7
;	LDMFD	lr!{r0-r12}
;	STMFD	sp!{r0-r12}
;	MRS	r4,CPSR
;	BIC	r4,r4,#0xC0
;	STMD	sp!,{r4}
;	MRS	r4,SPSR
;	STMFD	sp!{r4}
;	
;	LDR 	r4,addr_OSTCBCur

;	void OSStartHighRdy(void)
;	
;	Start the task with the highest priority;
;
	EXPORT	OSStartHighRdy
OSStartHighRdy
	; Get current task TCB address
	LDR	r4, addr_OSTCBCur
	; Get highest priority task TCB address
	LDR	r5, addr_OSTCBHighRdy
	LDR	r5, [r5]		; get stack pointer
	LDR	sp, [r5]		; switch to the new stack

	STR	r5, [r4]		; set new current task TCB address

	LDMFD	sp!, {r4}		; YYY
	LDMFD	sp!, {r4}		; get new state from top of the stack
	MSR	CPSR_cxsf, r4		; CPSR should be SVC32Mode
	LDMFD	sp!, {r0-r12, pc}	; start the new task

	END
