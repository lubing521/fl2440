#include "2440addr.h"
#include "def.h"
#define	EE_PAGE_ADDR	0xa0
#define EEFLAG	0x6a
#define	SIZE	8
#define	SIZE_CONVERT	32
extern void Uart_Printf(char *fmt,...);
void Wr24C02(U32 slvAddr,U32 addr,U8 data);
void Rd24C02(U32 slvAddr,U32 addr,U8 *data);
void Run_IicPoll(void);
void IicPoll(void) ;
void Delay(int x) ;
static U8 iicData[IICBUFSIZE]; 
static volatile int iicDataCount;
static volatile int iicStatus;
static volatile int iicMode;
static int iicPt;
void wrTStoIIC(int coef,unsigned char address);
int rdTSfromIIC(unsigned char address);

//int	a[6]={0x1a2b3c4d,0x5f6a7b8c,0x9daebfc0,0xe1f20314,0x25364758,0x697a8b9c};
//int k=0xadbecfd0;
int a[6]={-11520,2203,4567,99982,12,54};
int k=-998;
void iicMain(void)
{ 
    unsigned int i,j; 
    int read;
    static U8 data[256]; 
    
    int coef[SIZE];
 	U8 result[SIZE_CONVERT];
 	int rresult[SIZE];
 	U8 buffer[SIZE][4];
 	U8 rbuffer[SIZE_CONVERT];
    Uart_Printf("IIC Test(Polling) using AT24C02\n"); 
 
    //设置GPE15->IICSDA 和 GPE14->IICSCL 
    rGPEUP  |= 0xc000;                  //Pull-up disable 
    rGPECON &= ~0xf0000000; 
    rGPECON |= 0xa0000000;              //GPE15:IICSDA , GPE14:IICSCL  
 
 //Enable ACK, Prescaler IICCLK=PCLK/16, Enable interrupt, Transmit clock value Tx clock=IICCLK/16 
    rIICCON    = (1<<7) | (0<<6) | (1<<5) | (0xf);  //0xaf 
    rIICADD  = 0x10;                    //2440 slave address = [7:1] 
    rIICSTAT = 0x10;                     //IIC bus data output enable(Rx/Tx) 
    Uart_Printf("Write test data into AT24C02\n"); 
    
  
//写入一个page的数据，page的大小是256byte， 
    //page 的起始地址是0xa0，写入的数据是：0、1、2、...255。0xa0是AT24C02的页地址。 
    //AT24C02的页地址是0x00/0x20/0x40/0x60/0x80/0xa0/0xc0/0xe0 
    coef[0]=EEFLAG;
    coef[1]=k;
    for(i=2;i<8;i++)
     	coef[i]=a[i-2];
    
    for(i=0;i<SIZE;i++){
		buffer[i][0]=(unsigned char)((coef[i]&0xFF000000)>>24);
     	buffer[i][1]=(unsigned char)((coef[i]&0x00FF0000)>>16);
     	buffer[i][2]=(unsigned char)((coef[i]&0x0000FF00)>>8);
     	buffer[i][3]=(unsigned char)(coef[i]&0x000000FF);
     	Uart_Printf("buffer %x\t%x\t%x\t%x\n",buffer[i][0],buffer[i][1],buffer[i][2],buffer[i][3]);
     	}
    for(i=0,j=0;j<SIZE_CONVERT;i++){
    	result[j++]=buffer[i][0];
    	result[j++]=buffer[i][1];
    	result[j++]=buffer[i][2];
    	result[j++]=buffer[i][3];
    	Uart_Printf("buffer before write %x\t%x\t%x\t%x\n",buffer[i][0],buffer[i][1],buffer[i][2],buffer[i][3]);
    	
    	}
        
    for(i=0;i<SIZE_CONVERT;i++){ 
       Wr24C02(EE_PAGE_ADDR,(U8)i,result[i]);//U32 slvAddr,U32 addr,U8 data
       Uart_Printf("wr %x\t",result[i]);
       }
       Uart_Printf("\n");
     
    //for(i=0;i<256;i++) 
       //Wr24C02(0xa0,(U8)i,i);//U32 slvAddr,U32 addr,U8 data
    //wrTStoIIC(k,0xa0);   
       
    //初始化data数组的值为0。 
    //for(i=0;i<SIZE_CONVERT;i++) 
    //    data[i] = 0; 
    //Uart_Printf("Read test data from AT24C02\n"); 
   // read=rdTSfromIIC(0xa0);
 	//Uart_Printf("\nread=%d\n\n",read);
    //读24C02的0xa0地址中数据到data数组中。 
   /* for(i=0;i<SIZE_CONVERT;i++) 
        Rd24C02(0xa0,(U8)i,&(data[i]));  
    //输出data数组接收数据的值 
    for(i=0;i<16;i++) 
    { 
        for(j=0;j<16;j++) 
        Uart_Printf("%2x ",data[i*16+j]); 
        Uart_Printf("\n"); 
    } 
    */
    /*for(i=0,j=0;j<SIZE;j++){
       		Rd24C02(EE_PAGE_ADDR,(U8)i++,&(rbuffer[0]));
       		Rd24C02(EE_PAGE_ADDR,(U8)i++,&(rbuffer[1]));
       		Rd24C02(EE_PAGE_ADDR,(U8)i++,&(rbuffer[2]));
       		Rd24C02(EE_PAGE_ADDR,(U8)i++,&(rbuffer[3]));
       		Uart_Printf("rbuffer %x\t%x\t%x\t%x\n",rbuffer[0],rbuffer[1],rbuffer[2],rbuffer[3]);
       		rresult[j]=(rbuffer[3]<<24)|(rbuffer[2]<<16)|(rbuffer[1]<<8)|(rbuffer[0]);
       		Uart_Printf("read from eeprom result[%d]=%x\n",j,rresult[j]);
       		
	 }*/
	 for(i=0;i<SIZE_CONVERT;i++){
	 		Rd24C02(EE_PAGE_ADDR,(U8)i,&(rbuffer[i]));
	 		Uart_Printf("rd %x\t",rbuffer[i]);
	 
	 }
	  Uart_Printf("\n");
	 for(i=0,j=0;j<SIZE;j++){
	 	rresult[j]=(rbuffer[i]<<24)|(rbuffer[i+1]<<16)|(rbuffer[i+2]<<8)|(rbuffer[i+3]);
	 	i=i+4;
	 	//Uart_Printf("rbuffer %x\t%x\t%x\t%x\n",rbuffer[0],rbuffer[1],rbuffer[2],rbuffer[3]);
	 	Uart_Printf("rresult[%d]=%d\n",j,rresult[j]);
	 	}
	 	
	 		
	 		
   // Uart_Printf("OK! Write data is same to Read data!\n"); 
    while(1);
}

void wrTStoIIC(int coef,unsigned char address)

{
	 int i;
	 U8 iic_buffer[4];
     iic_buffer[0]=(unsigned char)((coef&0xFF000000)>>24);
     iic_buffer[1]=(unsigned char)((coef&0x00FF0000)>>16);
     iic_buffer[2]=(unsigned char)((coef&0x0000FF00)>>8);
     iic_buffer[3]=(unsigned char)(coef&0x000000FF);
     for (i=0;i<4;i++)
     		Wr24C02(address,(U8)i,iic_buffer[i]);
}

//读取EEPROM中的4个8位字节，并把它们组合成一个32位的整型。
int rdTSfromIIC(unsigned char address)
{
       int temp;
       int i;
       U8 iic_buffer[4];
       for(i=0;i<4;i++)
       		Rd24C02(address,(U8)i,&(iic_buffer[i]));
       temp=(iic_buffer[0]<<24)|(iic_buffer[1]<<16)|(iic_buffer[2]<<8)|(iic_buffer[3]);
       return temp;
}


/*******************************Wr24C02*****************************************/
void Wr24C02(U32 slvAddr,U32 addr,U8 data)
{ 
    iicMode      = WRDATA; 
    iicPt        = 0; 
    iicData[0]   = (U8)addr; 
    iicData[1]   = data; 
    iicDataCount = 2; 
     
    //8-bit data shift register for IIC-bus Tx/Rx operation. 
    rIICDS        = slvAddr;            //0xa0 
     
    //Master Tx mode, Start(Write), IIC-bus data output enable 
        //Bus arbitration sucessful, Address as slave status flag Cleared, 
        //Address zero status flag cleared, Last received bit is 0 
    rIICSTAT      = 0xf0;       
     
    //Clearing the pending bit isn't needed because the pending bit has been cleared. 
    while(iicDataCount!=-1) 
       Run_IicPoll(); 
 
iicMode = POLLACK;
    while(1) 
    { 
 		rIICDS     = slvAddr; 
        iicStatus = 0x100;                 //To check if _iicStatus is changed   
        rIICSTAT   = 0xf0;           //Master Tx, Start, Output Enable, Sucessful, Cleared, Cleared, 0 
        rIICCON    = 0xe0;//0xaf;     //Resumes IIC operation. //hzh 
        while(iicStatus==0x100)   
            Run_IicPoll(); 
               
        if(!(iicStatus & 0x1)) 
            break;                      //When ACK is received 
    } 
    rIICSTAT = 0xd0;                     //Master Tx condition, Stop(Write), Output Enable 
    rIICCON  = 0xe0;//0xaf;              //Resumes IIC operation.  //hzh 
    Delay(1);                           //Wait until stop condtion is in effect. 
      //Write is completed. 
}

/*******************************Rd24C02*****************************************/
void Rd24C02(U32 slvAddr,U32 addr,U8 *data) 
{ 
    iicMode      = SETRDADDR;
    iicPt        = 0; 
    iicData[0]   = (U8)addr; 
    iicDataCount = 1; 
 
    rIICDS   = slvAddr; 
    rIICSTAT = 0xf0;                    //MasTx,Start  

  //Clearing the pending bit isn't needed because the pending bit has been cleared. 
    while(iicDataCount!=-1) 
        Run_IicPoll(); 
 
    iicMode      = RDDATA; 
    iicPt        = 0; 
    iicDataCount = 1; 
     
    rIICDS   = slvAddr; 
    rIICSTAT = 0xb0;                    //Master Rx,Start 
    rIICCON  = 0xe0;//0xaf;              //Resumes IIC operation.   
    while(iicDataCount!=-1) 
        Run_IicPoll(); 
 
    *data = iicData[1]; 
}


void Run_IicPoll(void) 
{ 
    if(rIICCON & 0x10)                  //Tx/Rx Interrupt Enable 
       IicPoll(); 
} 

       
void IicPoll(void) 
{ 
    U32 iicSt,i;
    iicSt = rIICSTAT;  
    if(iicSt & 0x8){};                   //When bus arbitration is failed. 总线仲裁失败
    if(iicSt & 0x4){};                    //When a slave address is matched with IICADD 
    if(iicSt & 0x2){};                   //When a slave address is 0000000b 
    if(iicSt & 0x1){} ;                  //When ACK isn't received 
 
    switch(iicMode) 
    {
    case POLLACK: 
        iicStatus = iicSt; 
        break; 
        
    case RDDATA: 
        if((iicDataCount--)==0) 
        { 
            iicData[iicPt++] = rIICDS; 
         
            rIICSTAT = 0x90;           //Stop MasRx condition  
            rIICCON  = 0xe0;//0xaf;     //Resumes IIC operation. 
            Delay(1);                   //Wait until stop condtion is in effect. 
                                      //Too long time...  
                                       //The pending bit will not be set after issuing stop condition. 
            break;     
        }       
        iicData[iicPt++] = rIICDS; 
                    //The last data has to be read with no ack. 
 if((iicDataCount)==0) 
            rIICCON = 0x60;//0x2f;       //Resumes IIC operation with NOACK.     
    else  
            rIICCON = 0xe0;//0xaf;      //Resumes IIC operation with ACK 
        break;
        
      case WRDATA: 
          if((iicDataCount--)==0) 
          { 
              rIICSTAT = 0xd0;           //stop MasTx condition  
              rIICCON  = 0xe0;//0xaf;     //resumes IIC operation.   
              Delay(1);                   //wait until stop condtion is in effect. 
              //The pending bit will not be set after issuing stop condition. 
              break;     
          } 
          rIICDS = iicData[iicPt++];        //_iicData[0] has dummy. 
          for(i=0;i<10;i++);               //for setup time until rising edge of IICSCL 
          rIICCON = 0xe0;//0xaf;          //resumes IIC operation.   
          break; 
          
      case SETRDADDR: 
          if((iicDataCount--)==0) 
          { 
              break;                  //IIC operation is stopped because of IICCON[4]     
          } 
          rIICDS = iicData[iicPt++];
	 for(i=0;i<10;i++);          //for setup time until rising edge of IICSCL 
            rIICCON = 0xe0;//0xaf;             //resumes IIC operation.   
            break; 
            default: 
            break;       
    } 
}

void Delay(int x)  
 {
 	int k,j;
 	while(x)
 	{
 		for(k=0;k<=0xff;k++)
 			for(j=0;j<=0xff;j++);
 			x--;
 	}
 }