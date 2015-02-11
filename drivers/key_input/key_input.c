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
#include<linux/input.h>
#include<asm/bitops.h>
#define GPF0	(S3C2410_GPF(0)) //定义pin mach/gpio-nrs.h 头文件中的宏
#define	GPF2	(S3C2410_GPF(2))
#define GPF3	(S3C2410_GPF(3))
#define GPF4	(S3C2410_GPF(4))
#define KEY_NUM	4
#define KEY_TEST	10 //evnet中的code值，键盘编码，value为按键高低电平值，type为事件类型
//按键信息，中断号，pin号，键值
struct input_dev *key_dev;
static struct key_info
{
	int irq_no;
	int pin;
	int pin_setting;
	int key_no;
	char *name;
}key_info_tab[KEY_NUM]=
{
	{IRQ_EINT0,GPF0,S3C2410_GPF0_EINT0,0,"key_1"},
	{IRQ_EINT2,GPF2,S3C2410_GPF2_EINT2,1,"key_2"},
	{IRQ_EINT3,GPF3,S3C2410_GPF3_EINT3,2,"key_3"},
	{IRQ_EINT4,GPF4,S3C2410_GPF4_EINT4,3,"key_4"},
};

//按键中断
static irqreturn_t s3c2440_eint_key(int irq,void *dev_id)
{
	input_report_key(key_dev,KEY_TEST,s3c2410_gpio_getpin(GPF0));
	input_report_key(key_dev,KEY_B,s3c2410_gpio_getpin(GPF2));
	input_report_key(key_dev,KEY_C,s3c2410_gpio_getpin(GPF3));
	input_report_key(key_dev,KEY_D,s3c2410_gpio_getpin(GPF4));
	input_sync(key_dev);
	return IRQ_HANDLED;
}
static int s3c2440_key_open(struct input_dev *dev)
{
		int i;
		int err=0;
		for(i=0;i<KEY_NUM;i++){
			set_irq_type(key_info_tab[i].irq_no,IRQF_TRIGGER_RISING);
			err=request_irq(key_info_tab[i].irq_no,s3c2440_eint_key,IRQF_SAMPLE_RANDOM,key_info_tab[i].name,NULL);
			if(err)
				return -1;
		}
		//error handler -> free_irq
		return 0;
}
static void s3c2440_key_release(struct input_dev *dev)
{
	int i;
	for(i=0;i<KEY_NUM;i++){
		disable_irq_nosync(key_info_tab[i].irq_no);
		free_irq(key_info_tab[i].irq_no,NULL);
	}
}


static int __init s3c2440_key_init(void)
{
	int err; 
	//设备号，次设备号为0
	struct input_dev *input_dev;
	input_dev=input_allocate_device();
	if(!input_dev){
		return -ENOMEM;		
	}
	key_dev=input_dev;
	set_bit(EV_KEY,key_dev->evbit);
	set_bit(KEY_TEST,key_dev->keybit);
	set_bit(KEY_B,key_dev->keybit);
	set_bit(KEY_C,key_dev->keybit);
	set_bit(KEY_D,key_dev->keybit);
	key_dev->name="mykeys";
	key_dev->dev.init_name="input_key";
	key_dev->open=s3c2440_key_open;
	key_dev->close=s3c2440_key_release;
	err=input_register_device(key_dev);
	if(err){
		return err;
	}
	return 0;
		
}
static void __exit s3c2440_key_exit(void)
{
	input_unregister_device(key_dev);
}
MODULE_AUTHOR("weicz");
MODULE_LICENSE("Dual BSD/GPL");
module_init(s3c2440_key_init);
module_exit(s3c2440_key_exit);
