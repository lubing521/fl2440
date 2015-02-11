#include<stdio.h>
#include<fcntl.h>
int main(int argc,char* argv[])
{
	int fd;
	char c;
	if(argc!=2){
		printf("usage: ledapp /dev/led");
		return -1;
	}
	
	fd=open(argv[1],O_RDWR);
	if(fd<0){
		printf("open error fd=%d\n",fd);
		return -2;
	}
	while(1){
		c=getchar();
		if(c=='0')
			ioctl(fd,0,0);
		else if(c=='1')
			ioctl(fd,1,0);
		else if(c=='q')
			break;
	}
}
