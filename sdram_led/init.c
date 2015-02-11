#define WTCON  (*(volatile unsigned long *)0x53000000)
#define MEM_CTL_BASE 0x48000000

void disable_watch_dog()
{
	WTCON=0;
}

void memsetup()
{
	unsigned long const mem_cfg_val[]={0x22011110,
	0x00000700,0x00000700,0x00000700,0x00000700,
	0x00000700,0x00000700,0x00018005,0x00018005,
    0x008c07a3,0x000000b1,0x00000030,0x00000030};
    
    int i=0;
    volatile unsigned long *p=(volatile unsigned long *)
    	MEM_CTL_BASE;
    for(;i<13;i++)
    	p[i]=mem_cfg_val[i];
}

void copy_2th_to_sdram()
{
	unsigned int *pdwSrc=(unsigned int *)0;
	unsigned int *pdwDest=(unsigned int *)0x30000000;
	
	while(pdwSrc<(unsigned int *)4096)
		*pdwDest++=*pdwSrc++;
}

