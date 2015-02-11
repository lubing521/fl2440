;=========================================
; NAME: 2440INIT.S
; DESC: C start up codes
;       Configure memory, ISR ,stacks
;	Initialize C-variables
; HISTORY:
; 2002.02.25:kwtark: ver 0.0
; 2002.03.20:purnnamu: Add some functions for testing STOP,Sleep mode
; 2003.03.14:DonGo: Modified for 2440.
;=========================================

	GET option.inc
	GET memcfg.inc
	GET 2440addr.inc

BIT_SELFREFRESH EQU	(1<<22)	;bit[22]=1,others=0

;Pre-defined constants		;系统的工作模式设定
USERMODE    EQU 	0x10
FIQMODE     EQU 	0x11
IRQMODE     EQU 	0x12
SVCMODE     EQU 	0x13
ABORTMODE   EQU 	0x17
UNDEFMODE   EQU 	0x1b
MODEMASK    EQU 	0x1f
NOINT       EQU 	0xc0

;The location of stacks		;系统的堆栈空间设定
UserStack	EQU	(_STACK_BASEADDRESS-0x3800)	;0x33ff4800 ~
SVCStack	EQU	(_STACK_BASEADDRESS-0x2800)	;0x33ff5800 ~
UndefStack	EQU	(_STACK_BASEADDRESS-0x2400)	;0x33ff5c00 ~
AbortStack	EQU	(_STACK_BASEADDRESS-0x2000)	;0x33ff6000 ~
IRQStack	EQU	(_STACK_BASEADDRESS-0x1000)	;0x33ff7000 ~
FIQStack	EQU	(_STACK_BASEADDRESS-0x0)	;0x33ff8000 ~

;arm处理器有两种工作状态 1.arm:32位 这种工作状态下执行字对准的arm指令 2.Thumb:16位 这种工作状
;态执行半字对准的Thumb指令 
;因为处理器分为16位 32位两种工作状态 程序的编译器也是分16位和32两种编译方式 所以下面的程序用
;于根据处理器工作状态确定编译器编译方式 
;code16伪指令指示汇编编译器后面的指令为16位的thumb指令 
;code32伪指令指示汇编编译器后面的指令为32位的arm指令 
;这段是为了统一目前的处理器工作状态和软件编译方式（16位编译环境使用tasm.exe编译
;Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.

	GBLL    THUMBCODE    ;定义一个全局变量
	[ {CONFIG} = 16			;if config==16 这里表示你的目前处于领先地16位编译方式
THUMBCODE SETL  {TRUE}		;设置THUMBCODE 为 true表示告诉系统当前想用thumb，但实际启动时不行，只能启动后再跳
							; [|]表示if else endif 
	    CODE32				;启动时强制使用32位编译模式
 		|
THUMBCODE SETL  {FALSE}		;如果系统要求是ARM指令，则直接设置THUMBCODE 为 false 说明当前的是32位编译模式
    ]

 		MACRO				;宏定义
	MOV_PC_LR
 		[ THUMBCODE
	    bx lr
 		|
	    mov	pc,lr
 		]
	MEND

 		MACRO
	MOVEQ_PC_LR
 		[ THUMBCODE
        bxeq lr		;相等Z=1,则跳转
 		|
	    moveq pc,lr
 		]
	MEND

;注意下面这段程序是个宏定义 很多人对这段程序不理解 我再次强调这是一个宏定义 所以大家要注意了
;下面包含的HandlerXXX HANDLER HandleXXX将都被下面这段程序展开 
;这段程序用于把中断服务程序的首地址装载到pc中，有人称之为“加载程序”。 
;本初始化程序定义了一个数据区（在文件最后），34个字空间，存放相应中断服务程序的首地址。每个字
;空间都有一个标号，以Handle***命名。 
;在向量中断模式下使用“加载程序”来执行中断服务程序。 
;这里就必须讲一下向量中断模式和非向量中断模式的概念 
;向量中断模式是当cpu读取位于0x18处的IRQ中断指令的时候，系统自动读取对应于该中断源确定地址上的;
;指令取代0x18处的指令，通过跳转指令系统就直接跳转到对应地址 
;函数中 节省了中断处理时间提高了中断处理速度标 例如 ADC中断的向量地址为0xC0,则在0xC0处放如下
;代码：ldr PC,=HandlerADC 当ADC中断产生的时候系统会 
;自动跳转到HandlerADC函数中 
;非向量中断模式处理方式是一种传统的中断处理方法，当系统产生中断的时候，系统将interrupt 
;pending寄存器中对应标志位置位 然后跳转到位于0x18处的统一中断 
;函数中 该函数通过读取interrupt pending寄存器中对应标志位 来判断中断源 并根据优先级关系再跳到
;对应中断源的处理代码中

 		MACRO 
$HandlerLabel HANDLER $HandleLabel

$HandlerLabel
	sub	sp,sp,#4	;decrement sp(to store jump address)
	stmfd	sp!,{r0}	;PUSH the work register to stack(lr does't push because it return to original address)
	ldr     r0,=$HandleLabel;load the address of HandleXXX to r0
	ldr     r0,[r0]	 ;load the contents(service routine start address) of HandleXXX
	str     r0,[sp,#4]      ;store the contents(ISR) of HandleXXX to stack
	ldmfd   sp!,{r0,pc}     ;POP the work register and pc(jump to ISR)
	MEND
;将$HandleLabel地址空间中的数据给PC，中断服务程序的入口


	IMPORT  |Image$$RO$$Limit|  ; End of ROM code (=start of ROM data)
	IMPORT  |Image$$RW$$Base|   ; Base of RAM to initialise
	IMPORT  |Image$$ZI$$Base|   ; Base and limit of area
	IMPORT  |Image$$ZI$$Limit|  ; to zero initialise

	IMPORT	Main
;导入要用到的字符常量

	AREA    Init,CODE,READONLY

;异常中断矢量表（每个表项占4个字节） 下面是中断向量表 一旦系统运行时有中断发生 即使移植了操作
;系统 如linux 处理器已经把控制权交给了操作系统 一旦发生中断 处理器还是会跳转到从0x0开始 
;中断向量表中某个中断表项（依据中断类型）开始执行 
;具体中断向量布局请参考s3c44b0 spec 例如 adc中断向量为 0x000000c0下面对应表中第49项位置 向量地址0x0+4*(49-1)=0x000000c0 

	ENTRY
;板子上电和复位后 程序开始从位于0x0处开始执行硬件刚刚上电复位后 程序从这里开始执行跳转到标
;为ResetHandler处执行

	;1)The code, which converts to Big-endian, should be in little endian code.
	;2)The following little endian code will be compiled in Big-Endian mode.
	;  The code byte order should be changed as the memory bus width.
	;3)The pseudo instruction,DCD can't be used here because the linker generates error.
	
	;条件编译，在编译成机器码前就设定好
	ASSERT	:DEF:ENDIAN_CHANGE	;判断ENDIAN_CHANGE是否已定义
	[ ENDIAN_CHANGE				;如果已经定义了ENDIAN_CHANGE，则判断,here is FALSE
	    ASSERT  :DEF:ENTRY_BUS_WIDTH	;判断ENTRY_BUS_WIDTH是否已定义
	    [ ENTRY_BUS_WIDTH=32	;如果已经定义了ENTRY_BUS_WIDTH，则判断是不是为32
		b	ChangeBigEndian	    	;DCD 0xea000007
	    ]
	;在bigendian中，地址为A的字单元包括字节单元A，A+1，A+2，A+3，字节单元由高位到低位为A，A+1，A+2，A+3
	;				地址为A的字单元包括半字单元A，A+2，半字单元由高位到低位为A，A+2
	    [ ENTRY_BUS_WIDTH=16
		andeq	r14,r7,r0,lsl #20   ;DCD 0x0007ea00	也是b	ChangeBigEndian指令，只是由于总线不一样而取机器码的顺序不一样
	    ]							;先取低位->高位	上述指令是通过机器码装换而来的

	    [ ENTRY_BUS_WIDTH=8
		streq	r0,[r0,-r10,ror #1] ;DCD 0x070000ea 也是b	ChangeBigEndian指令，只是由于总线不一样而取机器码的顺序不一样
	    ]
	|
	    b	ResetHandler		;//here is the first instrument  0x00
    ]
	b	HandlerUndef	;handler for Undefined mode	;0x04
	b	HandlerSWI	;handler for SWI interrupt		;0x08
	b	HandlerPabort	;handler for PAbort			;0x0c
	b	HandlerDabort	;handler for DAbort			;0x10
	b	.		;reserved							;0x14
	b	HandlerIRQ	;handler for IRQ interrupt		;0x18
	b	HandlerFIQ	;handler for FIQ interrupt		;0x1c

;@0x20
	b	EnterPWDN	; Must be @0x20.


;通过设置CP15的C1的位7，设置存储格式为Bigendian，三种总线方式
ChangeBigEndian ;//here ENTRY_BUS_WIDTH=16
;@0x24
	[ ENTRY_BUS_WIDTH=32
	    DCD	0xee110f10	;0xee110f10 => mrc p15,0,r0,c1,c0,0
	    DCD	0xe3800080	;0xe3800080 => orr r0,r0,#0x80;  //Big-endian
	    DCD	0xee010f10	;0xee010f10 => mcr p15,0,r0,c1,c0,0
	    ;对存储器控制寄存器操作，指定内存模式为Big-endian
	    ;因为刚开始CPU都是按照32位总线的指令格式运行的，如果采用其他的话，CPU别不了，必须转化
	    ;但当系统初始化好以后，则CPU能自动识别
	]
	[ ENTRY_BUS_WIDTH=16
	    DCD 0x0f10ee11
	    DCD 0x0080e380
	    DCD 0x0f10ee01
	    ;因为采用Big-endian模式，采用16位总线时，物理地址的高位和数据的地位对应
	    ;所以指令的机器码也相应的高低对调
	]
	[ ENTRY_BUS_WIDTH=8
	    DCD 0x100f11ee
	    DCD 0x800080e3
	    DCD 0x100f01ee
    ]
	DCD 0xffffffff  ;swinv 0xffffff is similar with NOP and run well in both endian mode.
	DCD 0xffffffff
	DCD 0xffffffff
	DCD 0xffffffff
	DCD 0xffffffff
	b ResetHandler

;Function for entering power down mode
; 1. SDRAM should be in self-refresh mode.
; 2. All interrupt should be maksked for SDRAM/DRAM self-refresh.
; 3. LCD controller should be disabled for SDRAM/DRAM self-refresh.
; 4. The I-cache may have to be turned on.
; 5. The location of the following code may have not to be changed.

;void EnterPWDN(int CLKCON);
EnterPWDN
	mov r2,r0		;r2=rCLKCON 保存原始数据 0x4c00000c 使能各模块的时钟输入
	tst r0,#0x8		;测试bit[3] SLEEP mode? 1=>sleep
	bne ENTER_SLEEP	;C=0,即TST结果非0，bit[3]=1

;//进入PWDN后如果不是sleep则进入stop

;//进入Stop mode
ENTER_STOP
	ldr r0,=REFRESH		;0x48000024   DRAM/SDRAM refresh config
	ldr r3,[r0]			;r3=rREFRESH
	mov r1, r3
	orr r1, r1, #BIT_SELFREFRESH	;Enable SDRAM self-refresh
	str r1, [r0]		;Enable SDRAM self-refresh
;//Enable SDRAM self-refresh
	mov r1,#16			;wait until self-refresh is issued. may not be needed.
0	subs r1,r1,#1
	bne %B0
;//wait 16 fclks for self-refresh
	ldr r0,=CLKCON		;enter STOP mode.
	str r2,[r0]
;//？？？？？？？？？？？？？？

	mov r1,#32
0	subs r1,r1,#1	;1) wait until the STOP mode is in effect.
	bne %B0			;2) Or wait here until the CPU&Peripherals will be turned-off
					;Entering SLEEP mode, only the reset by wake-up is available.

	ldr r0,=REFRESH ;exit from SDRAM self refresh mode.
	str r3,[r0]

	MOV_PC_LR		;back to main process
		

ENTER_SLEEP
	;NOTE.
	;1) rGSTATUS3 should have the return address after wake-up from SLEEP mode.

	ldr r0,=REFRESH
	ldr r1,[r0]		;r1=rREFRESH
	orr r1, r1, #BIT_SELFREFRESH
	str r1, [r0]		;Enable SDRAM self-refresh
;//Enable SDRAM self-refresh

	mov r1,#16			;Wait until self-refresh is issued,which may not be needed.
0	subs r1,r1,#1
	bne %B0
;//Wait until self-refresh is issued,which may not be needed

	ldr	r1,=MISCCR		;IO register 
	ldr	r0,[r1]
	orr	r0,r0,#(7<<17)  ;Set SCLK0=1, SCLK1=1, SCKE=1.
	str	r0,[r1]

	ldr r0,=CLKCON		; Enter sleep mode
	str r2,[r0]

	b .			;CPU will die here.
;//进入Sleep Mode，1）设置SDRAM为self-refresh
;//				   2）设置MISCCR bit[17] 1:sclk0=sclk 0:sclk0=0
;//								 bit[18] 1:sclk1=sclk 0:sclk1=0
;//								 bit[19] 1:Self refresh retain enable
;//										 0:Self refresh retain disable  
;//										 When 1, After wake-up from sleep, The self-refresh will be retained.

WAKEUP_SLEEP
	;Release SCLKn after wake-up from the SLEEP mode.
	ldr	r1,=MISCCR
	ldr	r0,[r1]
	bic	r0,r0,#(7<<17)  ;SCLK0:0->SCLK, SCLK1:0->SCLK, SCKE:0->=SCKE.
	str	r0,[r1]
;//设置MISCCR

	;Set memory control registers
 	ldr	r0,=SMRDATA
	ldr	r1,=BWSCON	;BWSCON Address	;//总线宽度和等待控制寄存器
	add	r2, r0, #52	;End address of SMRDATA
0
	ldr	r3, [r0], #4	;数据处理后R0自加4，[R0]->R3，R0+4->R0
	str	r3, [r1], #4
	cmp	r2, r0
	bne	%B0
;//设置所有的memory control register，他的初始地址为BWSCON，初始化
;//数据在以SMRDATA为起始的存储区

	mov r1,#256
0	subs r1,r1,#1	;1) wait until the SelfRefresh is released.
	bne %B0
;//1) wait until the SelfRefresh is released.

	ldr r1,=GSTATUS3 	;GSTATUS3 has the start address just after SLEEP wake-up
	ldr r0,[r1]

	mov pc,r0
;//跳出Sleep Mode，进入Sleep状态前的PC



;//异常中断宏调用
	LTORG
HandlerFIQ      HANDLER HandleFIQ
HandlerIRQ      HANDLER HandleIRQ
HandlerUndef    HANDLER HandleUndef
HandlerSWI      HANDLER HandleSWI
HandlerDabort   HANDLER HandleDabort
HandlerPabort   HANDLER HandlePabort

IsrIRQ
	sub	sp,sp,#4       ;reserved for PC
	stmfd	sp!,{r8-r9}

	ldr	r9,=INTOFFSET	;地址为0x4a000014的空间存着中断的偏移
	ldr	r9,[r9]			;I_ISR
	ldr	r8,=HandleEINT0
	add	r8,r8,r9,lsl #2
	ldr	r8,[r8]
	str	r8,[sp,#8]
	ldmfd	sp!,{r8-r9,pc}
;//外部中断号判断，通过中断服务程序入口地址存储器的地址偏移确定
;//PC=[HandleEINT0+[INTOFFSET]]

;=======
; ENTRY
;扳子上电和复位后 程序开始从位于0x0执行b ResetHandler 程序从跳转到这里执行 
;板子上电复位后 执行几个步骤这里通过标号在注释中加1，2，3....标示 标号表示执行顺序 
;1.禁止看门狗 屏蔽所有中断
;=======
ResetHandler

;//1.禁止看门狗 屏蔽所有中断
	ldr	r0,=WTCON       ;watch dog disable
	ldr	r1,=0x0
	str	r1,[r0]

	ldr	r0,=INTMSK
	ldr	r1,=0xffffffff  ;all interrupt disable
	str	r1,[r0]

	ldr	r0,=INTSUBMSK
	ldr	r1,=0x3ff		;all sub interrupt disable
	str	r1,[r0]

	[ {FALSE}
			;//rGPFDAT = (rGPFDAT & ~(0xf<<4)) | ((~data & 0xf)<<4);
			;//Led_Display
	ldr	r0,=GPFCON		;//F-IO In/Out config 10 10 10 10 00 00 00 00
	ldr	r1,=0x5500			;//00 = Input 01 = Output
	str	r1,[r0]				;//10 = EINT[0] 11 = Reserved    
	ldr	r0,=GPFDAT		;//F-IO data register
	ldr	r1,=0x10
	str	r1,[r0]
	]

;//2.根据工作频率设置pll 
;这里介绍一下计算公式 
;//Fpllo=(m*Fin)/(p*2^s) 
;//m=MDIV+8,p=PDIV+2,s=SDIV 
;The proper range of P and M: 1<=P<=62, 1<=M<=248

;Fpllo必须大于20Mhz小于66Mhz 
;Fpllo*2^s必须小于170Mhz 
;如下面的PLLCON设定中的M_DIV P_DIV S_DIV是取自option.h中 
;#elif (MCLK==40000000) 
;#define PLL_M (0x48) 
;#define PLL_P (0x3) 
;#define PLL_S (0x2) 
;所以m=MDIV+8=80,p=PDIV+2=5,s=SDIV=2 
;硬件使用晶振为10Mhz,即Fin=10Mhz 
;Fpllo=80*10/5*2^2=40Mhz

	;To reduce PLL lock time, adjust the LOCKTIME register.
	ldr	r0,=LOCKTIME
	ldr	r1,=0xffffff
	str	r1,[r0]
;//设置PLL的重置延迟

    [ PLL_ON_START
	; Added for confirm clock divide. for 2440.
	; Setting value Fclk:Hclk:Pclk
	ldr	r0,=CLKDIVN  
	ldr	r1,=CLKDIV_VAL		; 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6.
	str	r1,[r0]				;//数据表示分频数

	;//Configure UPLL Fin=12.0MHz UFout=48MHz
	ldr	r0,=UPLLCON
	ldr	r1,=((U_MDIV<<12)+(U_PDIV<<4)+U_SDIV) ;//USB PLL CONFIG
	str	r1,[r0]
	
	nop	;// Caution: After UPLL setting, at least 7-clocks delay must be inserted for setting hardware be completed.
	nop
	nop
	nop
	nop
	nop
	nop
	;//Configure MPLL Fin=12.0MHz MFout=304.8MHz
	ldr	r0,=MPLLCON
	ldr	r1,=((M_MDIV<<12)+(M_PDIV<<4)+M_SDIV) 
	str	r1,[r0]
    ]
	
;//Check if the boot is caused by the wake-up from SLEEP mode.
	ldr	r1,=GSTATUS2
	ldr	r0,[r1]
	tst	r0,#0x2		;test if bit[1] is 1 or 0 0->C=1
					;						  1->C=0
	;In case of the wake-up from SLEEP mode, go to SLEEP_WAKEUP handler.
	bne	WAKEUP_SLEEP	;C=0,jump

	EXPORT StartPointAfterSleepWakeUp
StartPointAfterSleepWakeUp

;//3.置存储相关寄存器的程序 
;这是设置SDRAM,flash ROM 存储器连接和工作时序的程序,片选定义的程序 
;SMRDATA map在下面的程序中定义 
;SMRDATA中涉及的值请参考memcfg.s程序 
;具体寄存器各位含义请参考s3c44b0 spec	
	;Set memory control registers
 	ldr	r0,=SMRDATA
	ldr	r1,=BWSCON	;BWSCON Address
	add	r2, r0, #52	;End address of SMRDATA

	
0
	ldr	r3, [r0], #4
	str	r3, [r1], #4
	cmp	r2, r0
	bne	%B0
;//set memory registers


;//4.初始化各模式下的栈指针
 		;Initialize stacks
	bl	InitStacks
	
	

;//5.设置缺省中断处理函数 
  	; Setup IRQ handler
	ldr	r0,=HandleIRQ       ;This routine is needed
	ldr	r1,=IsrIRQ	  ;if there isn't 'subs pc,lr,#4' at 0x18, 0x1c
	str	r1,[r0]
	;//initialize the IRQ 将普通中断判断程序的入口地址给HandleIRQ
	
	

;//6.将数据段拷贝到ram中 将零初始化数据段清零 跳入C语言的main函数执行 到这步结束bootloader初步引导结束
	;If main() is used, the variable initialization will be done in __main().
	
    [	:LNOT:USE_MAIN	;initialized {FALSE}
  						;Copy and paste RW data/zero initialized data
  						
	LDR     r0, =|Image$$RO$$Limit| ; Get pointer to ROM data
	LDR     r1, =|Image$$RW$$Base|  ; and RAM copy
	LDR     r3, =|Image$$ZI$$Base|  
	
	;Zero init base => top of initialised data
	CMP     r0, r1      ; Check that they are different just for debug??????????????????????????
	BEQ     %F2
1       
	CMP     r1, r3      ; Copy init data
	LDRCC   r2, [r0], #4    ;--> LDRCC r2, [r0] + ADD r0, r0, #4         
	STRCC   r2, [r1], #4    ;--> STRCC r2, [r1] + ADD r1, r1, #4
	BCC     %B1
2       
	LDR     r1, =|Image$$ZI$$Limit| ; Top of zero init segment
	MOV     r2, #0
3       
	CMP     r3, r1      ; Zero init
	STRCC   r2, [r3], #4
	BCC     %B3
    ]


   	
    [ :LNOT:THUMBCODE	;if thumbcode={false} bl main
    	bl	Main        ;Don't use main() because ......
    	b	.                       
    ]


;//if thumbcod={ture}
    [ THUMBCODE         ;for start-up code for Thumb mode
    	orr	lr,pc,#1
    	bx	lr
    	CODE16
    	bl	Main        ;Don't use main() because ......
    	b	.
    	CODE32
    ]


;function initializing stacks
InitStacks
	;Don't use DRAM,such as stmfd,ldmfd......
	;SVCstack is initialized before
	;Under toolkit ver 2.5, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'
	
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#UNDEFMODE|NOINT
	msr	cpsr_cxsf,r1		;UndefMode
	ldr	sp,=UndefStack		; UndefStack=0x33FF_5C00

	orr	r1,r0,#ABORTMODE|NOINT
	msr	cpsr_cxsf,r1		;AbortMode
	ldr	sp,=AbortStack		; AbortStack=0x33FF_6000

	orr	r1,r0,#IRQMODE|NOINT
	msr	cpsr_cxsf,r1		;IRQMode
	ldr	sp,=IRQStack		; IRQStack=0x33FF_7000

	orr	r1,r0,#FIQMODE|NOINT
	msr	cpsr_cxsf,r1		;FIQMode
	ldr	sp,=FIQStack		; FIQStack=0x33FF_8000

	bic	r0,r0,#MODEMASK|NOINT
	orr	r1,r0,#SVCMODE
	msr	cpsr_cxsf,r1		;SVCMode
	ldr	sp,=SVCStack		; SVCStack=0x33FF_5800

	;USER mode has not be initialized.
	;//为什么不用初始化user的stacks，系统刚启动的时候运行在哪个模式下？？？？？？？？？？？？？？？？？？？
	mov	pc,lr
	;The LR register won't be valid if the current mode is not SVC mode.？？？？？？？？？？？？？
;//系统一开始运行就是SVCmode？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
	
;=====================================================================
; Clock division test
; Assemble code, because VSYNC time is very short
;=====================================================================
	EXPORT CLKDIV124
	EXPORT CLKDIV144
	
CLKDIV124
	
	ldr     r0, = CLKDIVN
	ldr     r1, = 0x3		; 0x3 = 1:2:4
	str     r1, [r0]
;	wait until clock is stable
	nop
	nop
	nop
	nop
	nop

	ldr     r0, = REFRESH
	ldr     r1, [r0]
	bic		r1, r1, #0xff
	bic		r1, r1, #(0x7<<8)
	orr		r1, r1, #0x470	; REFCNT135
	str     r1, [r0]
	nop
	nop
	nop
	nop
	nop
	mov     pc, lr

CLKDIV144
	ldr     r0, = CLKDIVN
	ldr     r1, = 0x4		; 0x4 = 1:4:4
	str     r1, [r0]
;	wait until clock is stable
	nop
	nop
	nop
	nop
	nop

	ldr     r0, = REFRESH
	ldr     r1, [r0]
	bic		r1, r1, #0xff
	bic		r1, r1, #(0x7<<8)
	orr		r1, r1, #0x630	; REFCNT675 - 1520
	str     r1, [r0]
	nop
	nop
	nop
	nop
	nop
	mov     pc, lr


;存储器控制寄存器的定义区
	LTORG

SMRDATA DATA
; Memory configuration should be optimized for best performance
; The following parameter is not optimized.
; Memory access cycle parameter strategy
; 1) The memory settings is  safe parameters even at HCLK=75Mhz.
; 2) SDRAM refresh period is for HCLK<=75Mhz.

	DCD (0+(B1_BWSCON<<4)+(B2_BWSCON<<8)+(B3_BWSCON<<12)+(B4_BWSCON<<16)+(B5_BWSCON<<20)+(B6_BWSCON<<24)+(B7_BWSCON<<28))
	DCD ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))   ;GCS0
	DCD ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))   ;GCS1
	DCD ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))   ;GCS2
	DCD ((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))   ;GCS3
	DCD ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))   ;GCS4
	DCD ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))   ;GCS5
	DCD ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))    ;GCS6
	DCD ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))    ;GCS7
	DCD ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Trc<<18)+(Tchr<<16)+REFCNT)

	DCD 0x32	    ;SCLK power saving mode, BANKSIZE 128M/128M

	DCD 0x30	    ;MRSR6 CL=3clk
	DCD 0x30	    ;MRSR7 CL=3clk


	ALIGN

	AREA RamData, DATA, READWRITE

	^   _ISR_STARTADDRESS		; _ISR_STARTADDRESS=0x33FF_FF00
HandleReset 	#   4
HandleUndef 	#   4
HandleSWI		#   4
HandlePabort    #   4
HandleDabort    #   4
HandleReserved  #   4
HandleIRQ		#   4
HandleFIQ		#   4

;Don't use the label 'IntVectorTable',
;The value of IntVectorTable is different with the address you think it may be.
;IntVectorTable
;@0x33FF_FF20
HandleEINT0		#   4
HandleEINT1		#   4
HandleEINT2		#   4
HandleEINT3		#   4
HandleEINT4_7	#   4
HandleEINT8_23	#   4
HandleCAM		#   4		; Added for 2440.
HandleBATFLT	#   4
HandleTICK		#   4
HandleWDT		#   4
HandleTIMER0 	#   4
HandleTIMER1 	#   4
HandleTIMER2 	#   4
HandleTIMER3 	#   4
HandleTIMER4 	#   4
HandleUART2  	#   4
;@0x33FF_FF60
HandleLCD 		#   4
HandleDMA0		#   4
HandleDMA1		#   4
HandleDMA2		#   4
HandleDMA3		#   4
HandleMMC		#   4
HandleSPI0		#   4
HandleUART1		#   4
HandleNFCON		#   4		; Added for 2440.
HandleUSBD		#   4
HandleUSBH		#   4
HandleIIC		#   4
HandleUART0 	#   4
HandleSPI1 		#   4
HandleRTC 		#   4
HandleADC 		#   4
;@0x33FF_FFA0
	END
