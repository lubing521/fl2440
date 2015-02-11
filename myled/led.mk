#
AS=armasm
CC=armcc
LD=armlink
FE=fromelf

#source files path
SRC_PATH = .\src

#header files path
INCLUDES_PATH = .\include

#object files path
OBJS_PATH = .\objs

#compile option
ASFLAGS = -g --cpu arm920t
CCFLAGS = -c -O1 -g --cpu ARM920T
LDFLAGS = --info totals --ro-base 0x32000000 --first init.o
FEFLAGS = -c --bin --output 

#header files info in compiling
INCLUDES = -I$(INCLUDES_PATH)

#object files
OBJS = $(OBJS_PATH)\main.o 
OBJS += $(OBJS_PATH)\init.o


all: led.bin
	@if exist *.bin echo Build completed

led.bin: led.axf
	$(FE) $< $(FEFLAGS) $@
led.axf: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
	

$(OBJS_PATH)\main.o: $(SRC_PATH)\main.c 
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(OBJS_PATH)\init.o: $(SRC_PATH)\init.s
	$(AS) $(ASFLAGS) $< -o $@
	
rebuild: clean all

clean:
	@if exist $(OBJS_PATH)\*.o del $(OBJS_PATH)\*.o
	@if exist *.axf del *.axf
	@if exist *.bin del *.bin