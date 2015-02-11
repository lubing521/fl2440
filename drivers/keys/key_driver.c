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

#define MAX_KEY_BUF	16
#define KEY_NUM		4
#define KEY_MAJOR	250
#define KEYSTATUS_UP	1
#define KEYSTATUS_DOWNX	2
#define KEYSTATUS_DOWN	0
//保证x在mod-1中循环
//比如，mod-1=7 -> 0x0000 0111
//x累加超过7的部分，经过与操作之后，高位为0
#define INC_BUF_POINTOR(x,mod)    ((++(x))&((mod)-1))
//判断按键是否按下，参数为对应的pin值
#define ISKEY_DOWN(x)	(s3c2410_gpio_getpin(key_info_tab[x].gpio_port)==KEYSTATUS_DOWN)
#define KEY_TIMER_DELAY1	HZ/100
#define KEY_TIMER_DELAY2	HZ/10
#define DEVICE_NAME		"mykeys"
//#define GPFCON	(0x02A2)
#define GPF0	(S3C2410_GPF(0)) //定义pin mach/gpio-nrs.h 头文件中的宏
#define	GPF2	(S3C2410_GPF(2))
#define GPF3	(S3C2410_GPF(3))
#define GPF4	(S3C2410_GPF(4))

//主设备号
static int key_major=KEY_MAJOR;
typedef unsigned char key_ret;
//typedef void (*f)(unsigned int);
//f keyEvent;
//设备结构体
struct key_dev
{
	unsigned int keyStatus[KEY_NUM];//记录按键状态
	key_ret buf[MAX_KEY_BUF];//记录键值
	unsigned int head,tail;
	wait_queue_head_t wq;
	struct cdev cdev;
};
struct key_dev *key_devp;
//定义定时器，定时器使用之前要init_timer
static struct timer_list key_timer[KEY_NUM];
//按键信息，中断号，pin号，键值
static struct key_info
{
	int irq_no;
	unsigned int gpio_port;
	int key_no;
}key_info_tab[KEY_NUM]=
{
	{IRQ_EINT0,GPF0,0},
	{IRQ_EINT2,GPF2,1},
	{IRQ_EINT3,GPF3,2},
	{IRQ_EINT4,GPF4,3},
};
//记录按键值
static void keyEvent(unsigned int key_index)
{
	//printk(KERN_EMERG "keyEvent\n");
	key_devp->buf[key_devp->head]=key_index;
	//printk(KERN_EMERG "key_pressed=%d,key_head=%d\n",key_devp->buf[key_devp->head],key_devp->head);

	key_devp->head=INC_BUF_POINTOR(key_devp->head,MAX_KEY_BUF);
	//printk(KERN_EMERG "key_head=%d\n",key_devp->head);
	//唤醒等待按键的列队
	wake_up_interruptible(&(key_devp->wq));

}
static int s3c2440_key_open(struct inode *inode, struct file *filp)
{
	//printk(KERN_EMERG "s3c2440_key_open\n");
	key_devp->head=0;
	key_devp->tail=0;
	return 0;
}
static int s3c2440_key_release(struct inode *inode, struct file *filp)
{
	//keyEvent=keyEvent_dummy;
	return 0;
}
static ssize_t s3c2440_key_read(struct file *filp,char __user *buf,size_t count,loff_t *ppos)
{
	unsigned long flag;
	unsigned int ret,tmp;
	//printk(KERN_EMERG "s3c2440_key_read\n");

	retry: if(key_devp->head!=key_devp->tail)
	{
		//进入临界区
		local_irq_save(flag);
		ret=key_devp->buf[key_devp->tail];
		key_devp->tail=INC_BUF_POINTOR(key_devp->tail,MAX_KEY_BUF);
		//推出临界区
		local_irq_restore(flag);

		//printk(KERN_EMERG "buffer not empty,\n");
		tmp=copy_to_user(buf,&ret,sizeof(unsigned int));
		//printk(KERN_EMERG "copy to user,tmp=%d\n",tmp);
		return (sizeof(unsigned int));
	}
	else{
		if(filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		//请求按键值，没有读到则挂起等待	
		interruptible_sleep_on(&(key_devp->wq));
		goto retry;
	}
	return 0;
}
static struct file_operations s3c2440_key_ops=
{
	owner:THIS_MODULE,
	open:s3c2440_key_open,
	release:s3c2440_key_release,
	read:s3c2440_key_read,
};
//字符设备注册
static void key_setup_cdev(struct key_dev* dev,int index)
{
	int err,devno=MKDEV(key_major,index);
	cdev_init(&dev->cdev,&s3c2440_key_ops);
	dev->cdev.owner=THIS_MODULE;
	dev->cdev.ops=&s3c2440_key_ops;
	err=cdev_add(&dev->cdev,devno,1);
	if(err)
	{
		printk(KERN_NOTICE "Error %d adding Key%d",err,index);
	}	

}
//按键中断
static irqreturn_t s3c2440_eint_key(int irq,void *dev_id)
{
	int key,cnt;
	key=0;
	//根据中断号判断当前按键是哪个
	for(cnt=0;cnt<KEY_NUM;cnt++){
		if(key_info_tab[cnt].irq_no==irq)
		{
			key=key_info_tab[cnt].key_no;
			break;
		}
	}
	//printk(KERN_EMERG "key=%d,irq_no=%d,irq=%d\n",key,key_info_tab[key].irq_no,irq);
	disable_irq_nosync(key_info_tab[key].irq_no);
	key_devp->keyStatus[key]=KEYSTATUS_DOWNX;
	key_timer[key].expires=jiffies+KEY_TIMER_DELAY1;
	//启动去抖动定时器
	add_timer(&key_timer[key]);
	return IRQ_HANDLED;
}
static void key_timer_handler(unsigned long data)
{
	int key=data;
	//printk(KERN_EMERG "Timer interrupt handler,key=%d\n",key);
	//printk(KERN_EMERG "tab1=%x,tab2=%x,tab3=%x,tab4=%x\n",&key_info_tab[0],&key_info_tab[1],&key_info_tab[2],&key_info_tab[3]);
	//printk(KERN_EMERG "gpio=%x\n",key_info_tab[key].gpio_port);
	
	//printk(KERN_EMERG "is_key_down=%x,gpio_port=%x\n",ISKEY_DOWN(key),s3c2410_gpio_getpin(key_info_tab[key].gpio_port));
	if(ISKEY_DOWN(key))
	{
		//printk(KERN_EMERG "Key Down\n");
		if(key_devp->keyStatus[key]==KEYSTATUS_DOWNX){
			key_devp->keyStatus[key]=KEYSTATUS_DOWN;
			key_timer[key].expires=jiffies+KEY_TIMER_DELAY2;
			//printk(KERN_EMERG "key downX,key=%d\n",key);
			keyEvent(key);
			add_timer(&key_timer[key]);
		}
		else{
			//printk(KERN_EMERG "Key Down down\n");
			key_timer[key].expires=jiffies+KEY_TIMER_DELAY2;
			add_timer(&key_timer[key]);
		}
	}
	else{
		key_devp->keyStatus[key]=KEYSTATUS_UP;
		//printk(KERN_EMERG "key status=%d\n",key_devp->keyStatus[key]);
		enable_irq(key_info_tab[key].irq_no);
		//printk(KERN_EMERG "enabled key_irq_no=%d\n",key_info_tab[key].irq_no);
	}

}
//按键id分配，用于传入中断申请的第三个参数，目前用中断号判断，这个参数暂时用不上
//如果相同中断号的设备用统一个中断程序，则可用这个参数
static int key_id[4]={0,1,2,3};
//申请中断
static int request_irqs(void)
{
	int i;
	for(i=0;i<(sizeof(key_info_tab))/sizeof(key_info_tab[0]);i++){
		//printk(KERN_EMERG "irq_no=%d\n",key_info_tab[i].irq_no);
		set_irq_type(key_info_tab[i].irq_no,IRQF_TRIGGER_RISING);
		if(request_irq(key_info_tab[i].irq_no,s3c2440_eint_key,IRQF_SHARED,DEVICE_NAME,(void *)(&key_id[i]))){
			return -1;
		}
	}
	return 0;

}
static void free_irqs(void)
{
	struct key_info *k;
	int i;
	for(i=0;i<sizeof(key_info_tab)/sizeof(key_info_tab[0]);i++)
	{
		k=key_info_tab+i;
		free_irq(k->irq_no,s3c2440_eint_key);
	}
}

static int __init s3c2440_key_init(void)
{
	int result,i,tmp;
	//设备号，次设备号为0
	dev_t devno=MKDEV(key_major,0);	
	result=register_chrdev_region(devno,1,"key");
	key_devp=kmalloc(sizeof(struct key_dev),GFP_KERNEL);
	if(!key_devp)
	{
		result=-ENOMEM;
		//goto fail_malloc;
	}
	memset(key_devp,0,sizeof(struct key_dev));
	key_setup_cdev(key_devp,0);
	tmp=request_irqs();
	if(tmp==-1)
	{
		printk(KERN_NOTICE "request_irqs error\n");
	}
	key_devp->head=0;
	key_devp->tail=0;
	//配置GPIO管脚，中断模式
	s3c2410_gpio_cfgpin(GPF0,S3C2410_GPIO_IRQ);
	s3c2410_gpio_cfgpin(GPF2,S3C2410_GPIO_IRQ);
	s3c2410_gpio_cfgpin(GPF3,S3C2410_GPIO_IRQ);
	s3c2410_gpio_cfgpin(GPF4,S3C2410_GPIO_IRQ);


	for(i=0;i<KEY_NUM;i++)
	{
		key_devp->keyStatus[i]=KEYSTATUS_UP;
	}
	init_waitqueue_head(&(key_devp->wq));
	for(i=0;i<KEY_NUM;i++)
	{
		init_timer(&key_timer[i]);
		setup_timer(&key_timer[i],key_timer_handler,i);
	}
	printk(KERN_NOTICE "init module, result=%d\n",result);
	//fail_malloc: unregister_chrdev_region(devno,1);
	return result;
		
}
static void __exit s3c2440_key_exit(void)
{
	free_irqs();
	cdev_del(&key_devp->cdev);
	kfree(key_devp);
	unregister_chrdev_region(MKDEV(key_major,0),1);
	printk(KERN_NOTICE "exit module\n");
}
MODULE_AUTHOR("weicz");
MODULE_LICENSE("Dual BSD/GPL");
module_init(s3c2440_key_init);
module_exit(s3c2440_key_exit);
