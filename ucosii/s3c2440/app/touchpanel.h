//#define GLOBAL_CLK 1
//#define MCLK	50000000
//校正点坐标
#define XD0	(LCD_XSIZE_TFT_240320/8)
#define YD0	(LCD_YSIZE_TFT_240320/4)
#define XD1	(LCD_XSIZE_TFT_240320-XD0)
#define	YD1	(LCD_YSIZE_TFT_240320-YD0)
#define	XD2	((XD0+XD1)/2)
#define	YD2 (YD1)

#define	EE_PAGE_ADDR	0xa0
#define EEFLAG	0x6a000000
#define	SIZE	8
#define	SIZE_CONVERT	32

void TS_cal_init(void);
void TS_cal(void);
void Touch_Screen_Init(void) ;