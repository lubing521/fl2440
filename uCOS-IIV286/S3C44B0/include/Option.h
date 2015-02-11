#ifndef __OPTION_H__
#define __OPTION_H__

// ************* OPTIONS **************
//#define MCLK 40000000
#define MCLK 40000000

#define WRBUFOPT (0x8)   //write_buf_on

#define SYSCFG_0KB (0x0|WRBUFOPT)
#define SYSCFG_4KB (0x2|WRBUFOPT)
#define SYSCFG_8KB (0x6|WRBUFOPT)

#define DRAM	    1		//In case DRAM is used
#define SDRAM	    2		//In case SDRAM is used
#define BDRAMTYPE   SDRAM	//used in power.c,44blib.c

//BUSWIDTH; 16,32
#define BUSWIDTH    (16)

#define CACHECFG    SYSCFG_8KB

#define _RAM_STARTADDRESS 0xc000000

//#define _ISR_STARTADDRESS 0xc1fff00   //GCS6:16M DRAM
#define _ISR_STARTADDRESS 0xc1fff00     //GCS6:64M DRAM/SDRAM


// NOTE: rom.mak,option.a have to be changed
#endif /*__OPTION_H__*/