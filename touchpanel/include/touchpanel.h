#define GLOBAL_CLK 1
#define MCLK	50000000
#define XD0	50
#define YD0	50
#define XD1	300
#define	YD1	150
#define	XD2	150
#define	YD2 200
#define	EE_PAGE_ADDR	0xa0
#define EEFLAG	0x6a000000
#define	SIZE	8
#define	SIZE_CONVERT	32


#define COLOR_MAX	 3		//—’…´÷÷¿‡
#include "2440lib.h"
#include "2440addr.h"
#include "def.h"
#include "option.h"
#include "2440slib.h"
#include "lcd.h"
#include "iic.h"

void touchpanel(void);