#include<linux/module.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/errno.h>
#include<linux/mm.h>
#include<linux/sched.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<asm/io.h>
#include<asm/system.h>
#include<asm/uaccess.h>
#include<mach/regs-gpio.h>
#include<linux/interrupt.h>
#include<linux/irq.h>
#include<linux/slab.h>
#include<linux/sched.h>
#include<linux/wait.h>
#include<mach/gpio-fns.h>
#include<linux/ioport.h>
#include<linux/platform_device.h>



