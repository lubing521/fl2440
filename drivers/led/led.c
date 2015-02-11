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
#define LED_MAJOR	249
#define DEVICE_NAME     "myled"
#define GPB0	(S3C2410_GPB(0))
#define GPB5	(S3C2410_GPB(5))
#define GPB6	(S3C2410_GPB(6))
#define GPB8	(S3C2410_GPB(8))
#define GPB10	(S3C2410_GPB(10))
static unsigned int led_index[5]={GPB0,GPB5,GPB6,GPB8,GPB10};
static int led_major=LED_MAJOR;
static int led_minor;
struct led_dev
{
	struct cdev cdev;
	unsigned char led_status;
};
struct led_dev *led_devp;

static int s3c2440_led_open(struct inode *inode,struct file *filp)
{
	struct led_dev *dev;
	dev=container_of(inode->i_cdev,struct led_dev,cdev);
	filp->private_data=dev;
	led_minor=MINOR(inode->i_rdev);
	printk(KERN_NOTICE "minor=%d\n",led_minor);
	//配置输出
	s3c2410_gpio_cfgpin(GPB0,S3C2410_GPIO_OUTPUT);
	s3c2410_gpio_cfgpin(GPB5,S3C2410_GPIO_OUTPUT);
	s3c2410_gpio_cfgpin(GPB6,S3C2410_GPIO_OUTPUT);
	s3c2410_gpio_cfgpin(GPB8,S3C2410_GPIO_OUTPUT);
	s3c2410_gpio_cfgpin(GPB10,S3C2410_GPIO_OUTPUT);
	
	//设置上啦电阻
	//s3c2410_gpio_pullup(GPB0,1);
	//s3c2410_gpio_pullup(GPB5,1);
	//s3c2410_gpio_pullup(GPB6,1);
	//s3c2410_gpio_pullup(GPB8,1);
	//s3c2410_gpio_pullup(GPB10,1);
	//iowrite32(GPBCON,S3C2410_GPBCON);
	return 0;
}
static int s3c2440_led_release(struct inode *inode,struct file *filp)
{
	return 0;
}
static int s3c2440_led_ioctl(struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg)
{
	unsigned int tmp;
	switch(cmd)
	{
		case 0:
			//tmp=ioread32(S3C2410_GPBDAT);
			//tmp=tmp & ~led_index[led_minor];
			//printk(KERN_NOTICE "tmp=%d\n",tmp);
			//iowrite32(tmp,S3C2410_GPBDAT);
			s3c2410_gpio_setpin(led_index[led_minor],0);
			return 0;
		case 1:
			//tmp=ioread32(S3C2410_GPBDAT);
			//tmp=tmp | led_index[led_minor];
			//printk(KERN_NOTICE "tmp=%d\n",tmp);
			//iowrite32(tmp,S3C2410_GPBDAT);
			s3c2410_gpio_setpin(led_index[led_minor],1);
			return 0;
	}
	return 0;

}
static struct file_operations s3c2440_led_ops=
{
	owner:THIS_MODULE,
	open:s3c2440_led_open,
	release:s3c2440_led_release,
	ioctl:s3c2440_led_ioctl,
};
static void led_setup_cdev(struct led_dev* dev,int index)
{
	int err,devno=MKDEV(led_major,index);
	cdev_init(&dev->cdev,&s3c2440_led_ops);
	dev->cdev.owner=THIS_MODULE;
	dev->cdev.ops=&s3c2440_led_ops;
	err=cdev_add(&dev->cdev,devno,1);
	if(err)
	{
		printk(KERN_NOTICE "Error %d adding led%d",err,index);
	}	

}
static int __init s3c2440_led_init(void)
{
	int result;
	dev_t devno=MKDEV(led_major,0);	
	result=register_chrdev_region(devno,5,"testled");
	//printk(KERN_NOTICE "devno:%x\n",devno);
	led_devp=kmalloc(5*sizeof(struct led_dev),GFP_KERNEL);
	if(!led_devp){
		result=-ENOMEM;
	}
	//申请了5个次设备号，对应要申请5个设备结构体
	memset(led_devp,0,5*sizeof(struct led_dev));
	led_setup_cdev(&led_devp[0],0);
	led_setup_cdev(&led_devp[1],1);
	led_setup_cdev(&led_devp[2],2);
	led_setup_cdev(&led_devp[3],3);
	led_setup_cdev(&led_devp[4],4);
	printk(KERN_NOTICE "init module, result=%d\n",result);
	return result;
		
}
static void __exit s3c2440_led_exit(void)
{
	cdev_del(&led_devp->cdev);
	kfree(led_devp);
	unregister_chrdev_region(MKDEV(led_major,0),1);
	printk(KERN_NOTICE "exit module\n");
}
MODULE_AUTHOR("weicz");
MODULE_LICENSE("Dual BSD/GPL");
module_init(s3c2440_led_init);
module_exit(s3c2440_led_exit);
