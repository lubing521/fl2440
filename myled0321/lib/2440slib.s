;=====================================================================
; File Name : 2440slib.s
; Function  : S3C2440  (Assembly)
; Date      : March 09, 2002
; Revision	: Programming start (February 26,2002) -> SOP
; Revision	: 03.11.2003 ver 0.0	Attatched for 2440
;=====================================================================

;Interrupt, FIQ/IRQ disable
NOINT  EQU 0xc0    ; 1100 0000

;Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.
   GBLL    THUMBCODE
   [ {CONFIG} = 16
THUMBCODE SETL  {TRUE}
	 CODE32
   |
THUMBCODE SETL  {FALSE}
   ]

   MACRO
     MOV_PC_LR
     [ THUMBCODE
       bx lr
     |
       mov pc,lr
     ]
   MEND

   AREA |C$$code|, CODE, READONLY

;========================================
;Workaround of problem between LCD and Framebuffer
;========================================
	EXPORT	SDRAMtest
SDRAMtest
    	ldr r0,=0x31000000	;//#表示立即数寻址，但是LDR立即数是有限制的，
   		ldr r1,=0x80000		;//只有部分可以，=是伪指令，表示先把这个数存
   		mov r2,#0xf000000f	;//在某个空间然后再给，如果可以直接寻址就自动
   							;//设置，编译器自己搞定
   		
LB2 	str r2,[r0],#4
		str r2,[r0],#4
   		subs r1,r1,#4
   		bne LB2
   		mov pc,lr
   		

;==============
; CPSR I,F bit
;==============
;int SET_IF(void);
;The return value is current CPSR.
	EXPORT	SET_IF
SET_IF
    ;This function works only if the processor is in previliged mode.
   mrs r0,cpsr
   mov r1,r0
   orr r1,r1,#NOINT
   msr cpsr_cxsf,r1
   MOV_PC_LR
;//置位IRQ和FIQ控制位，禁止中断



;void WR_IF(int cpsrValue);
   EXPORT WR_IF
WR_IF
    ;This function works only if the processor is in previliged mode.
   msr cpsr_cxsf,r0
   MOV_PC_LR
;//写CPSR


;void CLR_IF(void);
   EXPORT  CLR_IF
CLR_IF
    ;This function works only if the processor is in previliged mode.
   mrs r0,cpsr
   bic r0,r0,#NOINT
   msr cpsr_cxsf,r0
   MOV_PC_LR
;//清零IRQ和FIQ控制位，使能中断



;====================================
; MMU Cache/TLB/etc on/off functions  //TLB--页表缓冲
;====================================


;//=============CP15协处理器MMU的C1功能================
;//M-bit[0]:禁止/使能MMU 1使能
;//A-bit[1]:是否支持内存访问时地址对齐检查系统，1使能
;//C-bit[2]:禁止/使能数据Cache或整个Cache，1使能  不含Cache返回0，不能禁止Cache返回1
;//W-bit[3]:禁止/使能写入缓冲，1使能
;//P-bit[4]:控制PROG32控制信号
;//			0 异常中断处理程序进入32地址的模式
;//			1 异常中断处理程序进入26地址的模式  不支持26位时返回1

;//D-bit[5]:对于向前兼容的26位地址的ARM处理器
;//			0 禁止26位地址异常检查
;// 		1 使能26位地址异常检查

;//L-bit[6]:对于ARMv3及以前的版本
;//			1 选择后期中止模型
;//			0 选择早前中止模型

;//B-bit[7]:0 选择little-endian
;//			1 选择big-endian

;//S-bit[8]:用于系统保护
;//R-bit[9]:用于ROM保护
;//F-bit[10]:由生产商定义
;//Z-bit[11]:对于支持跳转预测的ARM系统
;//			1 使能跳转预测功能
;//			0 禁止跳转预测功能

;//I-bit[12]:Cache分开时，1 使能指令Cache，0 禁止使能Cache
;//V-bit[13]:对于支持高端异常中断向量表的系统
;//			0 选择 0x00000000-0x0000001c
;//			1 选择 0xFFFF0000-0xFFFF001c

;//RR-bit[14]:Cache的淘汰算法
;//			0 选择常规的淘汰算法，
;//			1 选择预测性的淘汰算法，如round-robin淘汰算法

;//L4-bit[15]:对于ARM版本5以上的处理器，用于兼容以前的ARM版本的功能
;//			0 保持当前ARM的正常的功能
;//			1 对于一些根据跳转地址的位[0]进行状态切换的指令，忽略[0]，不进行状态切换，保持和以前的ARM版本兼容
;//=====================================================

R1_I	EQU	(1<<12) ;//Cache分开时，1 使能指令Cache，0 禁止使能Cache
R1_C	EQU	(1<<2)	;//禁止/使能数据Cache或整个Cache，1使能  不含Cache返回0，不能禁止Cache返回1
R1_A	EQU	(1<<1)	;//是否支持内存访问时地址对齐检查系统，1使能
R1_M    EQU	(1)		;//禁止/使能MMU 1使能
R1_iA	EQU	(1<<31)
R1_nF   EQU	(1<<30)

;void MMU_EnableICache(void)
   EXPORT MMU_EnableICache
MMU_EnableICache
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_DisableICache(void)
   EXPORT MMU_DisableICache
MMU_DisableICache
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR
;//禁止/使能指令Cache



;void MMU_EnableDCache(void)
   EXPORT MMU_EnableDCache
MMU_EnableDCache
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_DisableDCache(void)
   EXPORT MMU_DisableDCache
MMU_DisableDCache
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR
;//禁止/使能数据Cache      



;void MMU_EnableAlignFault(void)
   EXPORT MMU_EnableAlignFault
MMU_EnableAlignFault
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_DisableAlignFault(void)
   EXPORT MMU_DisableAlignFault
MMU_DisableAlignFault
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR
;//使能/禁止存储器对齐功能   
   
   

;void MMU_EnableMMU(void)
   EXPORT MMU_EnableMMU
MMU_EnableMMU
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_DisableMMU(void)
   EXPORT MMU_DisableMMU
MMU_DisableMMU
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR
;//使能/禁止MMU
;//注意：使能/禁止MMU时

;//====================================================================================================
;//1、在使能MMU之前，要在内存中建立好页表，同时CP15中的各相关寄存器必须完成初始化。
;//2、如果使用的不是平板存储模式（物理地址和虚拟地址相等），在禁止/使能MMU时虚拟地
;//址和物理地址的对应关系会发生改变，这时应该清楚Cache中的当前地址变换条目。
;//3、如果完成禁止/使能MMU的代码的物理地址和虚拟地址不相同，则禁止/使能MMU时将造成
;//很大麻烦，因此强烈完成禁止/使能MMU的代码的物理地址和虚拟地址最好相同。
;//====================================================================================================

;void MMU_SetFastBusMode(void)
; FCLK:HCLK= 1:1
  EXPORT MMU_SetFastBusMode
MMU_SetFastBusMode
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_iA:OR:R1_nF
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_SetAsyncBusMode(void)
; FCLK:HCLK= 1:2
   EXPORT MMU_SetAsyncBusMode
MMU_SetAsyncBusMode
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_nF:OR:R1_iA
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR


;//C2用于保存页表的在内存中的基地址
;//页表中每一行对应一个虚地址页对应的实地址页的地址、该位的方位权限和该页的缓冲特性
;//通常把部分页表放在页表缓冲器TLB（translation lookaside buffer）中，换页表时，TLB要清空，因为。。。。
;//C8控制清空TLB
;//C10用于控制TLB中内容的锁定
;=========================
; Set TTBase
;=========================
;void MMU_SetTTBase(int base)
   EXPORT MMU_SetTTBase
MMU_SetTTBase
   ;ro=TTBase
   mcr p15,0,r0,c2,c0,0
   MOV_PC_LR



;//MMU将整个存储空间分成16个域（domain），每个域具有相同的访问属性
;//C3用于设置域的属性
;=========================
; Set Domain
;=========================
;void MMU_SetDomain(int domain)
   EXPORT MMU_SetDomain
MMU_SetDomain
   ;ro=domain
   mcr p15,0,r0,c3,c0,0
   MOV_PC_LR

;=========================
; ICache/DCache functions
;=========================
;void MMU_InvalidateIDCache(void)
   EXPORT MMU_InvalidateIDCache
MMU_InvalidateIDCache
   mcr p15,0,r0,c7,c7,0
   MOV_PC_LR

;void MMU_InvalidateICache(void)
   EXPORT MMU_InvalidateICache
MMU_InvalidateICache
   mcr p15,0,r0,c7,c5,0
   MOV_PC_LR

;void MMU_InvalidateICacheMVA(U32 mva)
   EXPORT MMU_InvalidateICacheMVA
MMU_InvalidateICacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c5,1
   MOV_PC_LR

;void MMU_PrefetchICacheMVA(U32 mva)
   EXPORT MMU_PrefetchICacheMVA
MMU_PrefetchICacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c13,1
   MOV_PC_LR

;void MMU_InvalidateDCache(void)
   EXPORT MMU_InvalidateDCache
MMU_InvalidateDCache
   mcr p15,0,r0,c7,c6,0
   MOV_PC_LR

;void MMU_InvalidateDCacheMVA(U32 mva)
   EXPORT MMU_InvalidateDCacheMVA
MMU_InvalidateDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c6,1
   MOV_PC_LR

;void MMU_CleanDCacheMVA(U32 mva)
   EXPORT MMU_CleanDCacheMVA
MMU_CleanDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c10,1
   MOV_PC_LR

;void MMU_CleanInvalidateDCacheMVA(U32 mva)
   EXPORT MMU_CleanInvalidateDCacheMVA
MMU_CleanInvalidateDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c14,1
   MOV_PC_LR

;void MMU_CleanDCacheIndex(U32 index)
   EXPORT MMU_CleanDCacheIndex
MMU_CleanDCacheIndex
   ;r0=index
   mcr p15,0,r0,c7,c10,2
   MOV_PC_LR

;void MMU_CleanInvalidateDCacheIndex(U32 index)
   EXPORT MMU_CleanInvalidateDCacheIndex
MMU_CleanInvalidateDCacheIndex
   ;r0=index
   mcr p15,0,r0,c7,c14,2
   MOV_PC_LR

;void MMU_WaitForInterrupt(void)
   EXPORT MMU_WaitForInterrupt
MMU_WaitForInterrupt
   mcr p15,0,r0,c7,c0,4
   MOV_PC_LR

;===============
; TLB functions
;===============
;voic MMU_InvalidateTLB(void)
   EXPORT MMU_InvalidateTLB
MMU_InvalidateTLB
   mcr p15,0,r0,c8,c7,0
   MOV_PC_LR

;void MMU_InvalidateITLB(void)
   EXPORT MMU_InvalidateITLB
MMU_InvalidateITLB
   mcr p15,0,r0,c8,c5,0
   MOV_PC_LR

;void MMU_InvalidateITLBMVA(U32 mva)
   EXPORT MMU_InvalidateITLBMVA
MMU_InvalidateITLBMVA
   ;ro=mva
   mcr p15,0,r0,c8,c5,1
   MOV_PC_LR

;void MMU_InvalidateDTLB(void)
	EXPORT MMU_InvalidateDTLB
MMU_InvalidateDTLB
	mcr p15,0,r0,c8,c6,0
	MOV_PC_LR

;void MMU_InvalidateDTLBMVA(U32 mva)
	EXPORT MMU_InvalidateDTLBMVA
MMU_InvalidateDTLBMVA
	;r0=mva
	mcr p15,0,r0,c8,c6,1
	MOV_PC_LR

;=================
; Cache lock down
;=================
;void MMU_SetDCacheLockdownBase(U32 base)
   EXPORT MMU_SetDCacheLockdownBase
MMU_SetDCacheLockdownBase
   ;r0= victim & lockdown base
   mcr p15,0,r0,c9,c0,0
   MOV_PC_LR

;void MMU_SetICacheLockdownBase(U32 base)
   EXPORT MMU_SetICacheLockdownBase
MMU_SetICacheLockdownBase
   ;r0= victim & lockdown base
   mcr p15,0,r0,c9,c0,1
   MOV_PC_LR

;=================
; TLB lock down
;=================
;void MMU_SetDTLBLockdown(U32 baseVictim)
   EXPORT MMU_SetDTLBLockdown
MMU_SetDTLBLockdown
   ;r0= baseVictim
   mcr p15,0,r0,c10,c0,0
   MOV_PC_LR

;void MMU_SetITLBLockdown(U32 baseVictim)
   EXPORT MMU_SetITLBLockdown
MMU_SetITLBLockdown
   ;r0= baseVictim
   mcr p15,0,r0,c10,c0,1
   MOV_PC_LR

;============
; Process ID
;============
;void MMU_SetProcessId(U32 pid)
   EXPORT MMU_SetProcessId
MMU_SetProcessId
   ;r0= pid
   mcr p15,0,r0,c13,c0,0
   MOV_PC_LR

   END
