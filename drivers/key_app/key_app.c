#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#define LED0 "/dev/led0"	
#define LED1 "/dev/led1"	
#define LED2 "/dev/led2"	
#define LED3 "/dev/led3"	
#define LED4 "/dev/led4"	
static char* led_group[]={LED1,LED2,LED3,LED4};
static int cmd=0;
int main()
{
	int fd,ret,key_num;
	int led_fd;
	fd=open("/dev/key",0);
	if(fd<0)
	{
		printf("open error\n");
		return -1;
	}
	printf("open /dev/key\n");
	while(1){
		//printf("reading...\n");
		ret=read(fd,&key_num,sizeof(int));
		printf("key_value=%d\n",key_num);
		led_fd=open(led_group[key_num],0);
		ioctl(led_fd,cmd,0);
		cmd=(cmd==0)?1:0;
		close(led_fd);

	}
	close(fd);
	return 0;
}
