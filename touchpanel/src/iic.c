#include "iic.h"
U8 iic_buffer[8];
void iic_init(void)
{ 
    Uart_Printf("IIC Test(Polling) using AT24C02\n"); 
 
    //设置GPE15->IICSDA 和 GPE14->IICSCL 
    rGPEUP  |= 0xc000;                  //Pull-up disable 
    rGPECON &= ~0xf0000000; 
    rGPECON |= 0xa0000000;              //GPE15:IICSDA , GPE14:IICSCL  
 
 //Enable ACK, Prescaler IICCLK=PCLK/16, Enable interrupt, Transmit clock value Tx clock=IICCLK/16 
    rIICCON    = (1<<7) | (0<<6) | (1<<5) | (0xf);  //0xaf 
    rIICADD  = 0x10;                    //2440 slave address = [7:1] 
    rIICSTAT = 0x10;                     //IIC bus data output enable(Rx/Tx) 
   
}

//把一个32位整型转换为4个8位字节型，并写入EEPROM中

void wrTStoIIC(int coef,unsigned char address)

{
		 int i;
     iic_buffer[0]=(unsigned char)((coef&0xFF000000)>>24);
     iic_buffer[1]=(unsigned char)((coef&0x00FF0000)>>16);
     iic_buffer[2]=(unsigned char)((coef&0x0000FF00)>>8);
     iic_buffer[3]=(unsigned char)(coef&0x000000FF);
     for (i=0;i<4;i++)
     		Wreeprom(address,(U8)i,iic_buffer[i]);
}

//读取EEPROM中的4个8位字节，并把它们组合成一个32位的整型。
int rdTSfromIIC(unsigned char address)
{
       int temp;
       int i;
       for(i=0;i<4;i++)
       		Rdeeprom(address,(U8)i,&(iic_buffer[i]));
       temp=(iic_buffer[0]<<24)|(iic_buffer[1]<<16)|(iic_buffer[2]<<8)|(iic_buffer[3]);
       return temp;
}

/*******************************Wr24C02*****************************************/
void Wreeprom(U32 slvAddr,U32 addr,U8 data)
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
void Rdeeprom(U32 slvAddr,U32 addr,U8 *data) 
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
