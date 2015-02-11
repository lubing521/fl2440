MYPATH = D:\arm7\myucos\uCOS-IIV286
PAT = D:\Program Files\ARM\ADSv1_2\Bin
MYLDPATH = D:\arm7\lib

CC = $(PAT)/armcc
LD = $(PAT)/armlink
OBJTOOL = $(PAT)/fromelf

RM = del
AS = $(PAT)/armasm -keep -g

CFLAGS = -g+ -o1 -Wa -DNO_UNDERSCORES=1
uCOSII_COMMON_INCLUDES = -I$(MYPATH)/uCOS-II/include
uCOSII_API_INCLUDES = -I$(MYPATH)/uCOS-II/API
uCOSII_SRC_INCLUDES = -I$(MYPATH)/uCOS-II/SOURCE
S3C44B0_INCLUDES = -I$(MYPATH)/S3C44B0/include
#-I$(MYPATH)/uCOS-II/include -I$(MYPATH)/uCOS-II/API -I$(MYPATH)/S3C44B0/include
#ASFLAGS = -I$(MYPATH)/S3C44B0/source

#-----------------------------------------------------------------------------
OS_CPU_C = $(MYPATH)/uCOS-II/ARM/os_cpu_c.c
OS_CPU_A = $(MYPATH)/uCOS-II/ARM/os_cpu_a.s
S3C44BINIT_S = $(MYPATH)/S3C44B0/source/44BINIT.S

OS_CPU_C_O = $(MYPATH)/myobj/os_cpu_c.o
OS_CPU_A_O = $(MYPATH)/myobj/os_cpu_a.o
S3C44BINIT_S_O = $(MYPATH)/myobj/44BINIT.o

uCOS_II_C = $(MYPATH)/uCOS-II/SOURCE/ucos_ii.c
uCOS_II_O = $(MYPATH)/myobj/ucos_ii.o

uCOSII_SRC_LIST = os_core.c os_dbg_r.c os_flag.c os_mbox.c os_mem.c os_mutex.c \
		os_q.c os_sem.c os_task.c os_time.c os_tmr.c
uCOSII_SRC = $(addprefix $(MYPATH)/uCOS-II/SOURCE/, $(uCOSII_SRC_LIST)) 
uCOSII_OBJS_LIST_TMP = $(patsubst %.c, %.o, $(uCOSII_SRC_LIST))
uCOSII_OBJS_LIST = $(foreach n, $(uCOSII_OBJS_LIST_TMP), $(n))
uCOSII_OBJS = $(addprefix $(MYPATH)/myobj/, $(uCOSII_OBJS_LIST))

S3C44B0_SRC_LIST = 44BLIB.C MAIN.c
S3C44B0_SRC = $(addprefix $(MYPATH)/S3C44B0/source/, $(S3C44B0_SRC_LIST))
S3C44B0_OBJS_LIST_TMP = $(patsubst %.c, %.o,$(S3C44B0_SRC_LIST))
S3C44B0_OBJS_LIST = $(patsubst %.C, %.o,$(S3C44B0_OBJS_LIST_TMP))
S3C44B0_OBJS = $(addprefix $(MYPATH)/myobj/, $(S3C44B0_OBJS_LIST))

OBJS += $(OS_CPU_C_O)
OBJS += $(OS_CPU_A_O)
#OBJS += $(uCOS_II_O)
OBJS += $(uCOSII_OBJS)
OBJS += $(S3C44B0_OBJS)
#-----------------------------------------------------------------------------

MODEL = test

#-----------------------------------------------------------------------------
all:$(MODEL).axf $(MODEL).bin

$(MODEL).bin:$(MODEL).axf
	$(OBJTOOL) -c -bin -output $@ $<
	$(OBJTOOL) -c -s -o $(<:.axf=.lst)

$(MODEL).axf:$(OBJS) $(S3C44BINIT_S_O)
	@echo "### Linking ..."
	$(LD) $(OBJS) $(S3C44BINIT_S_O) -ro-base 0xc000000 -first 44BINIT.o(Init) -o $@ -libpath $(MYLDPATH)
#-----------------------------------------------------------------------------

$(OS_CPU_C_O):$(OS_CPU_C)
	$(CC) $(CFLAGS) $(S3C44B0_INCLUDES) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(OS_CPU_A_O):$(OS_CPU_A)
	$(AS) $< -o $@
$(S3C44BINIT_S_O):$(S3C44BINIT_S)
	$(AS) $(S3C44B0_INCLUDES) $< -o $@

$(uCOS_II_O):$(uCOS_II_C)
	$(CC) $(CFLAGS) $(uCOSII_SRC_INCLUDES) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
#$(uCOSII_OBJS):$(uCOSII_SRC)
#	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_core.o:$(MYPATH)/uCOS-II/SOURCE/os_core.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_dbg_r.o:$(MYPATH)/uCOS-II/SOURCE/os_dbg_r.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_flag.o:$(MYPATH)/uCOS-II/SOURCE/os_flag.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_mbox.o:$(MYPATH)/uCOS-II/SOURCE/os_mbox.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_mem.o:$(MYPATH)/uCOS-II/SOURCE/os_mem.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_mutex.o:$(MYPATH)/uCOS-II/SOURCE/os_mutex.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_q.o:$(MYPATH)/uCOS-II/SOURCE/os_q.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_sem.o:$(MYPATH)/uCOS-II/SOURCE/os_sem.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_task.o:$(MYPATH)/uCOS-II/SOURCE/os_task.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_time.o:$(MYPATH)/uCOS-II/SOURCE/os_time.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@
$(MYPATH)/myobj/os_tmr.o:$(MYPATH)/uCOS-II/SOURCE/os_tmr.c
	$(CC) $(CFLAGS) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@


$(S3C44B0_OBJS):$(S3C44B0_SRC)
	$(CC) $(CFLAGS) $(S3C44B0_INCLUDES) $(uCOSII_COMMON_INCLUDES) $(uCOSII_API_INCLUDES) -c $< -o $@

#-----------------------------------------------------------------------------
clean:
	$(RM) $(MYPATH)/myobj/*.o
test:
	@echo "###Just a test###"
	@echo "###uCOSII_SRC###"
	@echo "###$(uCOSII_SRC)###"
	@echo "######"
	@echo "###$(uCOSII_OBJS_LIST)###"
	@echo "###uCOSII_OBJS###"
	@echo "###$(uCOSII_OBJS)###"
	@echo "######"
	@echo "###$(S3C44B0_SRC)###"
	@echo "######"
	@echo "###$(S3C44B0_OBJS)###"
	@echo "######"
	@echo "###$(OBJS)###"
#-----------------------------------------------------------------------------
