#define GPBCON (*(volatile unsigned long *)0x56000010)
#define GPBDAT (*(volatile unsigned long *)0x56000014)


#define GPB5_out (1<<(5*2))
#define GPB6_out (1<<(6*2))
#define GPB7_out (1<<(7*2))
#define GPB8_out (1<<(8*2))


int main()
{
	GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out;
	
	while(1){
	
		unsigned int i=0;
		long long j=0;
		unsigned int n=11;
		
		for(i=0;i<=15;i++)
		{
		    GPBDAT = (i<<5);
			
			for(j=0;j<10000;j++)
				n=~n;
				;
		}
		
	}
	
	return 0;
}