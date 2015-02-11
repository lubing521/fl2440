AS = armasm
CC = armcc
LD = armlink
FL = fromelf

ASFLAGS = --cpu ARM920T
CCFLAGS = -O2 -g --cpu ARM920T -c
LDFLAGS = --info totals --ro-base 0x32000000 --rw-base 0x32008000 --first init.o(Init)
FLFLAGS = -c --bin --output

INCLUDE_PATH = .\include
SRC_PATH = .\src
OBJ_PATH = .\objs

INCLUDES = -I$(INCLUDE_PATH)

OBJS += $(OBJ_PATH)\main.o
OBJS += $(OBJ_PATH)\init.o
OBJS += $(OBJ_PATH)\app.o

all:myuart.bin
	@if exist *.bin	echo	Build Completed

myuart.bin:myuart.axf
	$(FL) $< $(FLFLAGS) $@

myuart.axf:$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@
	
$(OBJ_PATH)\init.o:$(SRC_PATH)\init.s
	$(AS) $(INCLUDES) $(ASFLAGS) $< -o $@
$(OBJ_PATH)\main.o:$(SRC_PATH)\main.c
	$(CC) $(INCLUDES) $(CCFLAGS) $< -o $@
$(OBJ_PATH)\app.o:$(SRC_PATH)\app.c
	$(CC) $(INCLUDES) $(CCFLAGS) $< -o $@
	
clean:
	if exist $(OBJ_PATH)\*.o del $(OBJ_PATH)\*.o
	if exist *.bin	del	*.bin
	if exist *.axf	del	*.axf
	
rebuild: clean all
