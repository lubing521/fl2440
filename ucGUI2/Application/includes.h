#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "Target.h"
#include "def.h"
#include "option.h"
#include "uc_os.h"
#include "app_cfg.h"
//#include "FL2440_LCD_for_GUI.h"
#include "GUI.h"
//#include "iic.h"
//#include "touchpanel.h"
//#include "led.h"

#define	EnableIrq(bit)		rINTMSK &= ~(bit)
#define	DisableIrq(bit)		rINTMSK |= (bit)
#define	EnableSubIrq(bit)	rINTSUBMSK &= ~(bit)
#define	DisableSubIrq(bit)	rINTSUBMSK |= (bit)