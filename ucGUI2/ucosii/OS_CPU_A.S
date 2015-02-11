;**********************************************************************
; BASED ON
;***************************************************
; $Workfile:   Os_cpu_a.s  $
; $Revision: 1.1.1.1 $
; $Author: liuzheng $
; $Date: 2004/07/31  $
; **************************************************

;********************************************************************/
        AREA  |subr|, CODE, READONLY 
 IMPORT OSTCBCur
 IMPORT OSTCBHighRdy
 IMPORT OSPrioCur
 IMPORT OSPrioHighRdy
 IMPORT OSRunning
 IMPORT OSIntNesting
 IMPORT OSTimeTick
 IMPORT OSIntEnter
 IMPORT OSIntExit
 IMPORT OSTaskSwHook
 IMPORT OSIntCtxSwFlag
 
 IMPORT SAVED_LR_IRQ
 IMPORT SAVED_LR_SVC
 
 IMPORT TmrSignalTmr
           
 EXPORT OSTickISR
 EXPORT ARMDisableInt
 EXPORT ARMEnableInt
 EXPORT OSIntCtxSw
 EXPORT OSCtxSw
 EXPORT OSStartHighRdy

;/***********************************************************************
;
; Function: OSStartHighRdy
;
; Purpose:
; 	   To start the task with the highest priority during OS startup
;
; Processing:
;    See uC/OS-II Task Level Context Switch flow chart
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Notes:
;   Called once during OSStart()
;
;*********************************************************************/
OSStartHighRdy
        BL 	OSTaskSwHook             ; Call user-defined hook function

        LDR 	r4, =OSRunning            ; Indicate that multitasking has started
        MOV 	r5, #1                   
        STRB 	r5, [r4]                 ; OSRunning = true

        LDR 	r4, =OSTCBHighRdy        ; Get highest priority task TCB address
        LDR 	r4, [r4]                 ; get stack pointer
        LDR 	sp, [r4]                 ; switch to the new stack

        LDMFD 	sp!, {r4}                ; pop new task s spsr
        MSR 	spsr_c, r4
        LDMFD 	sp!, {r4}                ; pop new task s psr
        MSR 	cpsr_c, r4
        LDMFD 	sp!, {r0-r12,lr,pc}      ; pop new task s r0-r12,lr & pc

 	
;/***********************************************************************
;
; Function: OS_TASK_SW 
;
; Purpose:
; 	To perform a context switch from the Task Level.
;
; Processing:
;    See uC/OS-II Task Level Context Switch flow chart
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Notes:
;   On entry, OSTCBCur and OSPrioCur hold the current TCB and priority
;   and OSTCBHighRdy and OSPrioHighRdy contain the same for the task
;   to be switched to.
; 
;   The following code assumes that the virtual memory is directly
;   mapped into  physical memory. If this is not true, the cache must 
;   be flushed at context switch to avoid address aliasing.
;
;*********************************************************************/
OSCtxSw
        STMFD 	sp!, {lr}                ; push pc (lr is actually be pushed in place of PC)
        STMFD 	sp!, {r0-r12,lr}         ; push lr & register file
        MRS 	r4, cpsr                 ; copy CPSR to R4 
        STMFD 	sp!, {r4}                ; push current psr
        MRS 	r4, spsr                 ; copy SPSR to R4
        STMFD 	sp!, {r4}                ; push current spsr

_OSCtxSw
        LDR 	r4, =OSPrioCur           ; OSPrioCur = OSPrioHighRdy
        LDR 	r5, =OSPrioHighRdy
        LDRB 	r6, [r5]
        STRB 	r6, [r4]
        
        LDR 	r4, =OSTCBCur            ; Get current task TCB address
        LDR 	r5, [r4]
        STR 	sp, [r5]                 ; store sp in preempted tasks's TCB

        BL 	OSTaskSwHook             ; call Task Switch Hook

        LDR 	r6, =OSTCBHighRdy        ; Get highest priority task TCB address
        LDR 	r6, [r6]
        LDR 	sp, [r6]                 ; get new task s stack pointer

        STR 	r6, [r4]                 ; set new current task TCB address

        LDMFD 	sp!, {r4}                ; pop new task spsr
        MSR 	spsr_c, r4
        LDMFD 	sp!, {r4}                ; pop new task cpsr
        MSR 	cpsr_c, r4
        LDMFD 	sp!, {r0-r12,lr,pc}      ; pop new task r0-r12,lr & pc


;***********************************************************************
;
; Function: OSIntCtxSw
;
; Purpose:
; 	To perform a context switch from the interrupt level.
;
; Processing:
;    See uC/OS-II Interrupt Level Context Switch flow chart
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Notes:
;   Sets up the stacks and registers to call the task level
;   context switch
;
;*********************************************************************/
OSIntCtxSw	
        LDR 	r0, =OSIntCtxSwFlag      ; OSIntCtxSwFlag = true
        MOV 	r1, #1
        STR 	r1, [r0]                 
        MOV 	pc, lr                   ;This is only change flag,return to OSIntExit

;/***********************************************************************
;
; Function: OSTickISR
;
; Purpose:
; 	   The IRQ interrupt handler
;
; Processing:
;    Saves context
;    Calls the IRQ dispatcher
;    Checks if context switch necessary
;    If not, restores context and returns from interrupt
;    If switch required, branches without link to IRQContextSwap
;       which performs context switch if interrupts not nested
;       and returns from interrupt to new context
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Notes:
;   (1) here we use OSIntCtxSwFlag to indicate a request for int-level 
;       context switch 
;   (2) _IntCtxSw is used to perform a real switch operation
;
;*********************************************************************/	
OSTickISR
		SUB	lr, lr, #4            
		STMFD	sp!, {r0-r12, lr}    ; push r0-r12 register file and lr( pc return address )
	 
	    MRS 	r4, spsr        
		STMFD 	sp!, {r4}           ; push current spsr_irq ( =cpsr_svc )

;rI_ISPC = BIT_TIMER4
	
		mov      r0,#0x4000
		mov      r1,#0x4a000000
		str      r0,[r1,#0]
		mov      r0,#0x4000
		mov      r1,#0x4a000000
		str      r0,[r1,#0x10]

;-------------------------------------------------------------------------

		BL	OSIntEnter
		;liuzheng 2003-2-14
		BL 	OSTimeTick
		
		BL TmrSignalTmr
		;if task change, OSIntCtxSwFlag set 1
		BL	OSIntExit
		
		LDR	r0, =OSIntCtxSwFlag      ; check if OSIntCtxFlag is marked as true
		LDR 	r1, [r0]
		CMP	r1, #1
		
		BEQ	_IntCtxSw	         ; if OSIntCtxFlag = true, then jump to _IntCtxSw

		LDMFD 	sp!, {r4}                ; get cpsr_svc from stack
		MSR     spsr_c, r4                 ; prepare spsr to return svc mode	
		LDMFD	sp!, {r0-r12, pc}^       ; recover r0-r12 and pc from stack, cpsr also
  
_IntCtxSw
        MOV 	r1, #0                   ; clear OSIntCtxSwFlag = flase
        STR 	r1, [r0]
	
        LDMFD 	sp!, {r4}                ; restore spsr_irq 
		MSR 	spsr_c, r4 	
		LDMFD	sp!, {r0-r12, lr}        ; recover the irq stack pointer
	
	    LDR r5, =SAVED_LR_IRQ
	    LDR r6, =SAVED_LR_SVC
		STR	lr, [r5]         ; save lr_irq to SAVED_LR_IRQ 	
		MOV	lr, #0x00000093          ; change forcely cpsr to svc mode
		MSR	cpsr_c, lr	
		STR     lr, [r6]         ; save lr_svc to SAVED_LR_SVC
		LDR	lr, [r5]         ; get lr_irq value saved in SAVED_LR_IRQ 
		STMFD 	sp!, {lr}                ; push future task pc (lr_irq should be pushed in place of PC)
		LDR	lr, [r6]         ; get lr_svc value saved in SAVED_LR_SVC 
        STMFD 	sp!, {r0-r12,lr}         ; push lr & r0-r12 register file
        MRS 	r4, spsr
        STMFD 	sp!, {r4}                ; push current psr
        MRS 	r4, spsr
        STMFD 	sp!, {r4}                ; push spsr

	    B	_OSCtxSw                 ; jump to _OSCtxSw	

;/***********************************************************************
;
; Functions: ARMDisableInt
; 	     ARMEnableInt
;
; Purpose:
;    Disable and enable IRQ and FIQ preserving current CPU mode.
;
; Processing:
;    Push the cpsr onto the stack
;    Disable IRQ and FIQ interrupts
;    Return 
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Notes:
;   (1) Can be called from SVC mode to protect Critical Sections. 
;   (2) Do not use these calls at interrupt level.
;   (3) Used in pairs within the same function level;
;   (4) Will restore interrupt state when called; i.e., if interrupts
;       are disabled when DisableInt is called, interrupts will still
;       still be disabled when the matching EnableInt is called.
;   (5) Uses the method described by Labrosse as "Method 2".
;
;*********************************************************************/
ARMDisableInt	
		MRS	r0, cpsr
		STMFD	sp!, {r0}	         ; push current PSR
		ORR	r0, r0, #0x80
		MSR	cpsr_c, r0		 ; disable IRQ Int s
	
		MOV	pc, lr


    ;------------------------------------------------------------------------
ARMEnableInt	
		LDMFD	sp!, {r0}                ; pop current PSR
		MSR	cpsr_c, r0               ; restore original cpsr	
		
		MOV	pc, lr
    ;------------------------------------------------------------------------
    	
END