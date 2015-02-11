obj-m :=key_drv.o
KERNEL_DIR := /home/huniu/sources/kernel/linux-2.6.35
PWD :=$(shell pwd)
all:
	make -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
clean:
	rm *.o *.ko *.mod.c
.PHONY:clean

