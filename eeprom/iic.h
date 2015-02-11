#include "2440addr.h"
#include "2440lib.h"
#define IICBUFSIZE 0x20
#define WRDATA      (1)
#define POLLACK     (2)
#define RDDATA      (3)
#define SETRDADDR   (4)
void iic_init(void);
void Wreeprom(U32 slvAddr,U32 addr,U8 data);
void Rdeeprom(U32 slvAddr,U32 addr,U8 *data);
void Run_IicPoll(void);
void IicPoll(void) ;
void wrTStoIIC(int coef,unsigned char address);
int rdTSfromIIC(unsigned char address);
static U8 iicData[IICBUFSIZE]; 
static volatile int iicDataCount;
static volatile int iicStatus;
static volatile int iicMode;
static int iicPt;