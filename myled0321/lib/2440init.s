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

;Pre-defined constants		;ϵͳ�Ĺ���ģʽ�趨
USERMODE    EQU 	0x10
FIQMODE     EQU 	0x11
IRQMODE     EQU 	0x12
SVCMODE     EQU 	0x13
ABORTMODE   EQU 	0x17
UNDEFMODE   EQU 	0x1b
MODEMASK    EQU 	0x1f
NOINT       EQU 	0xc0

;The location of stacks		;ϵͳ�Ķ�ջ�ռ��趨
UserStack	EQU	(_STACK_BASEADDRESS-0x3800)	;0x33ff4800 ~
SVCStack	EQU	(_STACK_BASEADDRESS-0x2800)	;0x33ff5800 ~
UndefStack	EQU	(_STACK_BASEADDRESS-0x2400)	;0x33ff5c00 ~
AbortStack	EQU	(_STACK_BASEADDRESS-0x2000)	;0x33ff6000 ~
IRQStack	EQU	(_STACK_BASEADDRESS-0x1000)	;0x33ff7000 ~
FIQStack	EQU	(_STACK_BASEADDRESS-0x0)	;0x33ff8000 ~

;arm�����������ֹ���״̬ 1.arm:32λ ���ֹ���״̬��ִ���ֶ�׼��armָ�� 2.Thumb:16λ ���ֹ���״
;ִ̬�а��ֶ�׼��Thumbָ�� 
;��Ϊ��������Ϊ16λ 32λ���ֹ���״̬ ����ı�����Ҳ�Ƿ�16λ��32���ֱ��뷽ʽ ��������ĳ�����
;�ڸ��ݴ���������״̬ȷ�����������뷽ʽ 
;code16αָ��ָʾ�������������ָ��Ϊ16λ��thumbָ�� 
;code32αָ��ָʾ�������������ָ��Ϊ32λ��armָ�� 
;�����Ϊ��ͳһĿǰ�Ĵ���������״̬��������뷽ʽ��16λ���뻷��ʹ��tasm.exe����
;Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.

	GBLL    THUMBCODE    ;����һ��ȫ�ֱ���
	[ {CONFIG} = 16			;if config==16 �����ʾ���Ŀǰ�������ȵ�16λ���뷽ʽ
THUMBCODE SETL  {TRUE}		;����THUMBCODE Ϊ true��ʾ����ϵͳ��ǰ����thumb����ʵ������ʱ���У�ֻ������������
							; [|]��ʾif else endif 
	    CODE32				;����ʱǿ��ʹ��32λ����ģʽ
 		|
THUMBCODE SETL  {FALSE}		;���ϵͳҪ����ARMָ���ֱ������THUMBCODE Ϊ false ˵����ǰ����32λ����ģʽ
    ]

 		MACRO				;�궨��
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
        bxeq lr		;���Z=1,����ת
 		|
	    moveq pc,lr
 		]
	MEND

;ע��������γ����Ǹ��궨�� �ܶ��˶���γ������ ���ٴ�ǿ������һ���궨�� ���Դ��Ҫע����
;���������HandlerXXX HANDLER HandleXXX������������γ���չ�� 
;��γ������ڰ��жϷ��������׵�ַװ�ص�pc�У����˳�֮Ϊ�����س��򡱡� 
;����ʼ����������һ�������������ļ���󣩣�34���ֿռ䣬�����Ӧ�жϷ��������׵�ַ��ÿ����
;�ռ䶼��һ����ţ���Handle***������ 
;�������ж�ģʽ��ʹ�á����س�����ִ���жϷ������ 
;����ͱ��뽲һ�������ж�ģʽ�ͷ������ж�ģʽ�ĸ��� 
;�����ж�ģʽ�ǵ�cpu��ȡλ��0x18����IRQ�ж�ָ���ʱ��ϵͳ�Զ���ȡ��Ӧ�ڸ��ж�Դȷ����ַ�ϵ�;
;ָ��ȡ��0x18����ָ�ͨ����תָ��ϵͳ��ֱ����ת����Ӧ��ַ 
;������ ��ʡ���жϴ���ʱ��������жϴ����ٶȱ� ���� ADC�жϵ�������ַΪ0xC0,����0xC0��������
;���룺ldr PC,=HandlerADC ��ADC�жϲ�����ʱ��ϵͳ�� 
;�Զ���ת��HandlerADC������ 
;�������ж�ģʽ����ʽ��һ�ִ�ͳ���жϴ���������ϵͳ�����жϵ�ʱ��ϵͳ��interrupt 
;pending�Ĵ����ж�Ӧ��־λ��λ Ȼ����ת��λ��0x18����ͳһ�ж� 
;������ �ú���ͨ����ȡinterrupt pending�Ĵ����ж�Ӧ��־λ ���ж��ж�Դ ���������ȼ���ϵ������
;��Ӧ�ж�Դ�Ĵ��������

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
;��$HandleLabel��ַ�ռ��е����ݸ�PC���жϷ����������


	IMPORT  |Image$$RO$$Limit|  ; End of ROM code (=start of ROM data)
	IMPORT  |Image$$RW$$Base|   ; Base of RAM to initialise
	IMPORT  |Image$$ZI$$Base|   ; Base and limit of area
	IMPORT  |Image$$ZI$$Limit|  ; to zero initialise

	IMPORT	Main
;����Ҫ�õ����ַ�����

	AREA    Init,CODE,READONLY

;�쳣�ж�ʸ����ÿ������ռ4���ֽڣ� �������ж������� һ��ϵͳ����ʱ���жϷ��� ��ʹ��ֲ�˲���
;ϵͳ ��linux �������Ѿ��ѿ���Ȩ�����˲���ϵͳ һ�������ж� ���������ǻ���ת����0x0��ʼ 
;�ж���������ĳ���жϱ�������ж����ͣ���ʼִ�� 
;�����ж�����������ο�s3c44b0 spec ���� adc�ж�����Ϊ 0x000000c0�����Ӧ���е�49��λ�� ������ַ0x0+4*(49-1)=0x000000c0 

	ENTRY
;�����ϵ�͸�λ�� ����ʼ��λ��0x0����ʼִ��Ӳ���ո��ϵ縴λ�� ��������￪ʼִ����ת����
;ΪResetHandler��ִ��

	;1)The code, which converts to Big-endian, should be in little endian code.
	;2)The following little endian code will be compiled in Big-Endian mode.
	;  The code byte order should be changed as the memory bus width.
	;3)The pseudo instruction,DCD can't be used here because the linker generates error.
	
	;�������룬�ڱ���ɻ�����ǰ���趨��
	ASSERT	:DEF:ENDIAN_CHANGE	;�ж�ENDIAN_CHANGE�Ƿ��Ѷ���
	[ ENDIAN_CHANGE				;����Ѿ�������ENDIAN_CHANGE�����ж�,here is FALSE
	    ASSERT  :DEF:ENTRY_BUS_WIDTH	;�ж�ENTRY_BUS_WIDTH�Ƿ��Ѷ���
	    [ ENTRY_BUS_WIDTH=32	;����Ѿ�������ENTRY_BUS_WIDTH�����ж��ǲ���Ϊ32
		b	ChangeBigEndian	    	;DCD 0xea000007
	    ]
	;��bigendian�У���ַΪA���ֵ�Ԫ�����ֽڵ�ԪA��A+1��A+2��A+3���ֽڵ�Ԫ�ɸ�λ����λΪA��A+1��A+2��A+3
	;				��ַΪA���ֵ�Ԫ�������ֵ�ԪA��A+2�����ֵ�Ԫ�ɸ�λ����λΪA��A+2
	    [ ENTRY_BUS_WIDTH=16
		andeq	r14,r7,r0,lsl #20   ;DCD 0x0007ea00	Ҳ��b	ChangeBigEndianָ�ֻ���������߲�һ����ȡ�������˳��һ��
	    ]							;��ȡ��λ->��λ	����ָ����ͨ��������װ��������

	    [ ENTRY_BUS_WIDTH=8
		streq	r0,[r0,-r10,ror #1] ;DCD 0x070000ea Ҳ��b	ChangeBigEndianָ�ֻ���������߲�һ����ȡ�������˳��һ��
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


;ͨ������CP15��C1��λ7�����ô洢��ʽΪBigendian���������߷�ʽ
ChangeBigEndian ;//here ENTRY_BUS_WIDTH=16
;@0x24
	[ ENTRY_BUS_WIDTH=32
	    DCD	0xee110f10	;0xee110f10 => mrc p15,0,r0,c1,c0,0
	    DCD	0xe3800080	;0xe3800080 => orr r0,r0,#0x80;  //Big-endian
	    DCD	0xee010f10	;0xee010f10 => mcr p15,0,r0,c1,c0,0
	    ;�Դ洢�����ƼĴ���������ָ���ڴ�ģʽΪBig-endian
	    ;��Ϊ�տ�ʼCPU���ǰ���32λ���ߵ�ָ���ʽ���еģ�������������Ļ���CPU���ˣ�����ת��
	    ;����ϵͳ��ʼ�����Ժ���CPU���Զ�ʶ��
	]
	[ ENTRY_BUS_WIDTH=16
	    DCD 0x0f10ee11
	    DCD 0x0080e380
	    DCD 0x0f10ee01
	    ;��Ϊ����Big-endianģʽ������16λ����ʱ�������ַ�ĸ�λ�����ݵĵ�λ��Ӧ
	    ;����ָ��Ļ�����Ҳ��Ӧ�ĸߵͶԵ�
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
	mov r2,r0		;r2=rCLKCON ����ԭʼ���� 0x4c00000c ʹ�ܸ�ģ���ʱ������
	tst r0,#0x8		;����bit[3] SLEEP mode? 1=>sleep
	bne ENTER_SLEEP	;C=0,��TST�����0��bit[3]=1

;//����PWDN���������sleep�����stop

;//����Stop mode
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
;//����������������������������

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
;//����Sleep Mode��1������SDRAMΪself-refresh
;//				   2������MISCCR bit[17] 1:sclk0=sclk 0:sclk0=0
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
;//����MISCCR

	;Set memory control registers
 	ldr	r0,=SMRDATA
	ldr	r1,=BWSCON	;BWSCON Address	;//���߿�Ⱥ͵ȴ����ƼĴ���
	add	r2, r0, #52	;End address of SMRDATA
0
	ldr	r3, [r0], #4	;���ݴ����R0�Լ�4��[R0]->R3��R0+4->R0
	str	r3, [r1], #4
	cmp	r2, r0
	bne	%B0
;//�������е�memory control register�����ĳ�ʼ��ַΪBWSCON����ʼ��
;//��������SMRDATAΪ��ʼ�Ĵ洢��

	mov r1,#256
0	subs r1,r1,#1	;1) wait until the SelfRefresh is released.
	bne %B0
;//1) wait until the SelfRefresh is released.

	ldr r1,=GSTATUS3 	;GSTATUS3 has the start address just after SLEEP wake-up
	ldr r0,[r1]

	mov pc,r0
;//����Sleep Mode������Sleep״̬ǰ��PC



;//�쳣�жϺ����
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

	ldr	r9,=INTOFFSET	;��ַΪ0x4a000014�Ŀռ�����жϵ�ƫ��
	ldr	r9,[r9]			;I_ISR
	ldr	r8,=HandleEINT0
	add	r8,r8,r9,lsl #2
	ldr	r8,[r8]
	str	r8,[sp,#8]
	ldmfd	sp!,{r8-r9,pc}
;//�ⲿ�жϺ��жϣ�ͨ���жϷ��������ڵ�ַ�洢���ĵ�ַƫ��ȷ��
;//PC=[HandleEINT0+[INTOFFSET]]

;=======
; ENTRY
;�����ϵ�͸�λ�� ����ʼ��λ��0x0ִ��b ResetHandler �������ת������ִ�� 
;�����ϵ縴λ�� ִ�м�����������ͨ�������ע���м�1��2��3....��ʾ ��ű�ʾִ��˳�� 
;1.��ֹ���Ź� ���������ж�
;=======
ResetHandler

;//1.��ֹ���Ź� ���������ж�
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

;//2.���ݹ���Ƶ������pll 
;�������һ�¼��㹫ʽ 
;//Fpllo=(m*Fin)/(p*2^s) 
;//m=MDIV+8,p=PDIV+2,s=SDIV 
;The proper range of P and M: 1<=P<=62, 1<=M<=248

;Fpllo�������20MhzС��66Mhz 
;Fpllo*2^s����С��170Mhz 
;�������PLLCON�趨�е�M_DIV P_DIV S_DIV��ȡ��option.h�� 
;#elif (MCLK==40000000) 
;#define PLL_M (0x48) 
;#define PLL_P (0x3) 
;#define PLL_S (0x2) 
;����m=MDIV+8=80,p=PDIV+2=5,s=SDIV=2 
;Ӳ��ʹ�þ���Ϊ10Mhz,��Fin=10Mhz 
;Fpllo=80*10/5*2^2=40Mhz

	;To reduce PLL lock time, adjust the LOCKTIME register.
	ldr	r0,=LOCKTIME
	ldr	r1,=0xffffff
	str	r1,[r0]
;//����PLL�������ӳ�

    [ PLL_ON_START
	; Added for confirm clock divide. for 2440.
	; Setting value Fclk:Hclk:Pclk
	ldr	r0,=CLKDIVN  
	ldr	r1,=CLKDIV_VAL		; 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6.
	str	r1,[r0]				;//���ݱ�ʾ��Ƶ��

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

;//3.�ô洢��ؼĴ����ĳ��� 
;��������SDRAM,flash ROM �洢�����Ӻ͹���ʱ��ĳ���,Ƭѡ����ĳ��� 
;SMRDATA map������ĳ����ж��� 
;SMRDATA���漰��ֵ��ο�memcfg.s���� 
;����Ĵ�����λ������ο�s3c44b0 spec	
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


;//4.��ʼ����ģʽ�µ�ջָ��
 		;Initialize stacks
	bl	InitStacks
	
	

;//5.����ȱʡ�жϴ����� 
  	; Setup IRQ handler
	ldr	r0,=HandleIRQ       ;This routine is needed
	ldr	r1,=IsrIRQ	  ;if there isn't 'subs pc,lr,#4' at 0x18, 0x1c
	str	r1,[r0]
	;//initialize the IRQ ����ͨ�ж��жϳ������ڵ�ַ��HandleIRQ
	
	

;//6.�����ݶο�����ram�� �����ʼ�����ݶ����� ����C���Ե�main����ִ�� ���ⲽ����bootloader������������
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
	;//Ϊʲô���ó�ʼ��user��stacks��ϵͳ��������ʱ���������ĸ�ģʽ�£�������������������������������������
	mov	pc,lr
	;The LR register won't be valid if the current mode is not SVC mode.��������������������������
;//ϵͳһ��ʼ���о���SVCmode��������������������������������������������������������������������������������
	
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


;�洢�����ƼĴ����Ķ�����
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
