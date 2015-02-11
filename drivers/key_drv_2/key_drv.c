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
#include<asm/bitops.h>
#include<linux/input.h>
#define DEVICE_NAME	"plat_key_driver"
//#define GPFCON (0x56000050)
//#define GPFDAT (0x56000054)
//#define GPFCFG	(0x0002)
//地址映射后的io虚拟地址，这里用到了GPIOF，只用GPFCON和GPFDAT两个寄存器
//GPFCON 16bit GPFDAT 8bit
void __iomem *gpfcon;
void __iomem *gpfdat;
//定义输入设备，使用输入子系统
struct input_dev *key_input_dev;
//定义中断资源，这里只做了一个按键，这个结构显得有点多余，为的方便扩展
struct irq_des
{
	int key_irq;
	char *name;
};
struct irq_des key_irqs={
	.name="KEY0",
};
//中断处理函数
static irqreturn_t s3c2440_eint_key(int irq,void *dev_id)
{
	int value;
	//读出端口状态，并且只关心做后一个bit,对应我的按键key0
	value=ioread8(gpfdat);
	value=value & 0x01;
	//讲述据上报给输入子系统
	input_report_key(key_input_dev,KEY_0,value);
	//printk(KERN_NOTICE "value=%d\n",value);
	input_sync(key_input_dev);
	return IRQ_HANDLED;
}
//打开函数，主要工作就是申请中断
static int s3c2440_key_open(struct input_dev *dev)
{
	set_irq_type(key_irqs.key_irq,IRQF_TRIGGER_RISING);
	if(request_irq(key_irqs.key_irq,s3c2440_eint_key,IRQF_SAMPLE_RANDOM,key_irqs.name,NULL))
		return -1;
	printk(KERN_NOTICE "key_open\n");
	return 0;

}
static void s3c2440_key_release(struct input_dev *dev)
{
	disable_irq_nosync(key_irqs.key_irq);
	free_irq(key_irqs.key_irq,NULL);
}
//probe函数，这是driver的核心内容
//当总线匹配到driver和device时，调用driver的probe函数
//这里主要做的工作就是从对应的device的resource中获得device的硬件资源
//比如io地址，做地址映射，同时为对应的按键申请输入子系统的相关数据结构
static int key_probe(struct platform_device *dev)
{
	int ret=0;
	int value;
	struct resource *plat_resource;
	struct platform_device *pdev=dev;
	struct input_dev *input_dev;
	
	printk(KERN_NOTICE "platform driver match platform device\n");
	//get resource[0]
	//num代表的不是resource数组的数组号
	//而是具有相同资源类型的序号(从0开始的序号)
	//在本例中，因为IORESOURCE_IRQ类型的资源只有一个
	//故platform_get_irq()函数的第二个参数是0，而不是resource数组的序号1
	//获取io地址资源
	plat_resource=platform_get_resource(pdev,IORESOURCE_MEM,0);
	//申请iomem资源
	request_mem_region(plat_resource->start,resource_size(plat_resource),"platform key io_res");
	//地址映射,每个地址4个字节
	//GPFCON
	gpfcon=ioremap(plat_resource->start,0x00000004);
	//GPFDAT
	gpfdat=ioremap(plat_resource->start+4,0x00000004);
	//由于我的板子网卡用到了GPF的管脚，为防止配置中破坏其他管教配置
	//先读出原有配置，再对按键对应的GPF0进行单独配置，也就是最低两位配置位10，代表中断模式，具体的参见手册
	value=ioread16(gpfcon);
	value=(value | (0x1<<1))&(0xfffe);
	//写回配置
	iowrite16(value,gpfcon);
	//获取中断号资源
	plat_resource=platform_get_resource(pdev,IORESOURCE_IRQ,0);
	//向全局的数据结构中填写中断号，这个值再open函数调用时进行中断申请
	key_irqs.key_irq=plat_resource->start;
	//申请input子系统资源
	input_dev=input_allocate_device();

	key_input_dev=input_dev;
	//input子系统的按键配置
	set_bit(EV_KEY,key_input_dev->evbit);
	set_bit(KEY_0,key_input_dev->keybit);
	key_input_dev->name="mykeys";
	key_input_dev->dev.init_name="input_key";
	key_input_dev->open=s3c2440_key_open;
	key_input_dev->close=s3c2440_key_release;

	ret=input_register_device(key_input_dev);
	if(ret)
		return -1;
	printk(KERN_NOTICE "registered\n");	
	return ret;
}
static int key_remove(struct platform_device *dev)
{
	printk(KERN_NOTICE "platform device removed\n");
	input_unregister_device(key_input_dev);
	return 0;
}
struct platform_driver key_drv={
	.probe=key_probe,
	.remove=key_remove,
	.driver={
		.owner=THIS_MODULE,
		//这个名字要和对应的device中的名字一致，这是匹配dirver和device的依据
		.name="platform_key",
	},
};


static int __init s3c2440_key_init(void)
{
	int ret;
	//注册驱动程序
	ret=platform_driver_register(&key_drv);
	return ret;	
}
static void __exit s3c2440_key_exit(void)
{
	platform_driver_unregister(&key_drv);
}
MODULE_AUTHOR("weicz");
MODULE_LICENSE("Dual BSD/GPL");
module_init(s3c2440_key_init);
module_exit(s3c2440_key_exit);
