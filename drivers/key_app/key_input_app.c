#include<stdio.h>
#include<fcntl.h>
#include<linux/input.h>
#include<errno.h>
#include<stdlib.h>

int main(void )
{
	int fd;
	int key_value,i=0,count;
	struct input_event ev_key;
	fd=open("/dev/event0",666);
	if(fd<0){
		perror("open device");
		exit(1);
	}
	while(1){
		count=read(fd,&ev_key,sizeof(struct input_event));
		for(i=0;i<(int)count/sizeof(struct input_event);i++)
		{
			if(EV_KEY==ev_key.type)
				printf("type:%d,code:%d,value:%d\n",ev_key.type,ev_key.code,ev_key.value);
			if(EV_SYN==ev_key.type)
				printf("syn event\n");
		}
	}
		close(fd);
	return 0;
}

