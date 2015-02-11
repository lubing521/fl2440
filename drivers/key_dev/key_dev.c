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
//GPF的端口地址
#define GPF_PA	(0x56000050)
//平台设备
struct platform_device *key_dev;
//定义设备资源，这里两项，io地址资源和中断号资源
struct resource key_resource[]={
	[0]={
		.start=GPF_PA,
		.end=GPF_PA+8,
		.flags=IORESOURCE_MEM,
	},
	[1]={
		.start=IRQ_EINT0,
		.end=IRQ_EINT0,
		.flags=IORESOURCE_IRQ,
	},
};

static int __init s3c2440_key_init(void)
{
	int ret;
	//注册平台设备，第一个参数为设备名，这是与平台驱动driver进行匹配的依据，-1是id,自动分配，不care
	key_dev=platform_device_alloc("platform_key",-1);
	//为平台设备添加资源
	platform_device_add_resources(key_dev,key_resource,2);
	//添加设备
	ret=platform_device_add(key_dev);
	if(ret)
		platform_device_put(key_dev);
	printk(KERN_NOTICE "platform device added\n");
	return ret;
}
static void __exit s3c2440_key_exit(void)
{
	platform_device_unregister(key_dev);
}
MODULE_AUTHOR("weicz");
MODULE_LICENSE("Dual BSD/GPL");
module_init(s3c2440_key_init);
module_exit(s3c2440_key_exit);
