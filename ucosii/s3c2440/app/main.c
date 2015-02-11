#define	GLOBAL_CLK		1

#include  "Includes.H"

//�������ȼ�����
#define NormalTaskPrio       5
#define MainTaskPrio 	NormalTaskPrio+11
#define Taskk_BK_Color_Prio 		NormalTaskPrio+10
#define Task_TS_ICC_Prio  		NormalTaskPrio+2
#define Task_BK_Bmp1_Prio  		NormalTaskPrio+3
#define Task_BK_Bmp2_Prio  		NormalTaskPrio+4
#define Task_LED_Prio  	NormalTaskPrio+5

//�����ջ���ȶ���
#define	TaskStkLeath		1024     	

//LCD��������ʾ�˵�λ�ó�������
#define LCD_X	LCD_XSIZE_TFT_240320
#define LCD_Y	LCD_YSIZE_TFT_240320

#define CH_X	LCD_X/5
#define CH_X_M	CH_X-CH_WIDTH
#define CH_Y_M	LCD_Y/4
#define CH_Y_1	CH_Y_M+3*CH_HIGH
#define CH_Y_2	CH_Y_1+3*CH_HIGH



//�����ջ����
OS_STK Task_Start_Stk[TaskStkLeath];
OS_STK Task_BK_Color_Stk[TaskStkLeath];
OS_STK Task_TS_ICC_Stk[TaskStkLeath];
OS_STK Task_BK_Bmp1_Stk[TaskStkLeath];
OS_STK Task_BK_Bmp2_Stk[TaskStkLeath];
OS_STK Task_LED_Stk[TaskStkLeath];


//�ź�������
OS_EVENT  *Touchpanel,*Convert,*Key0,*Key2,*TS_Input;
OS_TCB  *ptcb;

//������������LCD����ת��ϵ��
extern INT32S TS_LCD_COEF[8];
//�������������
extern volatile INT32S TS_COORDINATE_X,TS_COORDINATE_Y;
//ת�����������
volatile INT32S COORDINATE_X,COORDINATE_Y;

//BMPͼ������
extern const U8 slamdunk1[];
extern const U8 slamdunk2[];
//ʱ�ӳ�ʼ��
extern Init_Timer4(void);
//�����жϳ�ʼ��
extern key_Eint(void);

__inline void cpu_init(void);

void 		Task_Start (void *pdata);			//��ʼ���񣬴�����������
void 		Task_BK_Color(void *pdata);			//������ɫ�л�����
void 		Task_TS_ICC(void *pdata);			//������У�����񣬽�ת��ϵ������eeprom
void 		Task_BK_Bmp1(void *pdata);			//��ʾ����ͼƬ1����
void 		Task_BK_Bmp2(void *pdata);			//��ʾ����ͼƬ2����
void 		Task_LED(void *pdata);				//LED��˸����


void Main (void)
{

	//cpu��ʼ��������ʱ��Ƶ�ʵ�
    cpu_init();
    Uart_Init(0,115200);  //���ڳ�ʼ�� ������Ϊ115200
    Uart_Select(0);  //ѡ�ߴ���0
    
    Uart_Printf("uC/OS Start!\n");
    
    FL_Lcd_Init();//��ʼ��LCD
    //ucos����ϵͳ��ʼ��
    OSInit();
    //����ϵͳʱ�ӳ�ʼ��
    OSTimeSet(0);
     
    //���������ź���
    Touchpanel = OSSemCreate(0);
    Convert=OSSemCreate(0);
    Key0=OSSemCreate(0);    
    Key2=OSSemCreate(0);
    TS_Input=OSSemCreate(0);
    //������ʼ����
    OSTaskCreate(Task_Start, 0, &Task_Start_Stk[TaskStkLeath - 1], MainTaskPrio);
    //��������ʼ��
    Touch_Screen_Init();
    //IIC���߳�ʼ��
    IIC_Init();
    //�����������
    OSStart();
}

//��ʼ����

void Task_Start (void *i)
{
  
    /*ʹϵͳ��ʼ��ʱ   */   
     
    Init_Timer4();
    OSStatInit();
  
    OSTaskCreate(Task_BK_Color, 0, &Task_BK_Color_Stk[TaskStkLeath - 1], Taskk_BK_Color_Prio);
    OSTaskCreate(Task_TS_ICC, 0, &Task_TS_ICC_Stk[TaskStkLeath - 1], Task_TS_ICC_Prio);
    

    //�˳�����
    OSTaskDel(OS_PRIO_SELF);
    Uart_Printf("Exit TaskStart.\n");
}

void Task_BK_Color(void *pdata)
{
   INT8U Reply;
   INT32S x,y;
  
   OSSemPend(Convert,0, &Reply);
   OSTaskCreate(Task_BK_Bmp1, 0, &Task_BK_Bmp1_Stk[TaskStkLeath - 1], Task_BK_Bmp1_Prio);
   OSTaskCreate(Task_BK_Bmp2, 0, &Task_BK_Bmp2_Stk[TaskStkLeath - 1], Task_BK_Bmp2_Prio);
   OSTaskCreate(Task_LED, 0, &Task_LED_Stk[TaskStkLeath - 1], Task_LED_Prio);
    for (;;)
    {
        
         Lcd_printf(CH_X_M,CH_Y_M,0,0,1,"��ѡ�񱳾���ɫ��\n");
         Lcd_printf(CH_X,CH_Y_1,0,0,1,"1.��ɫ\n");
         Lcd_printf(CH_X,CH_Y_2,0,0,1,"2.��ɫ\n");
        //�����ź������ȴ������������ж�
        OSSemPend(Touchpanel,0,&Reply);
        
        x=COORDINATE_X;	
 		y=COORDINATE_Y;	
        OS_ENTER_CRITICAL();
        Uart_Printf("Task_BK_Color  "); 
        //�ڵ�����������ʾ���������
        Uart_Printf("x_lcd=%d,y_lcd=%d\n",x,y);
       
         OS_EXIT_CRITICAL();
         
         
         //�жϴ���������λ�ö�Ӧ�Ĳ˵���
         if(x>CH_X && x<CH_X+3*CH_WIDTH && y>CH_Y_1 && y<CH_Y_1+CH_HIGH){
         	//����ɫ����ɫ
         	Lcd_ClearScr(0x1111);
         	
         	OSTimeDlyHMSM(0,0,0,100);
        }
         if(x>CH_X && x<CH_X+3*CH_WIDTH && y>CH_Y_2 && y<CH_Y_2+CH_HIGH){
     
         	OSTimeDlyHMSM(0,0,0,10);
         	//����ɫ�ú�ɫ
         	Lcd_ClearScr(0x8000);
        }
         
    	
    }
}



void Task_TS_ICC (void *pdata) //��ʼ����������У��������ת��
{
     U8 err;
     Uart_Printf("Task_TS_ICC\n");
     Uart_Printf("***** Calibrate TS ****** \n");
    
     TS_cal_init();
     OSSemPost(Convert);
     
  	 while(1){
  	 	OSSemPend(TS_Input,0,&err);
  	 	
  	 	//����ת��
  	 	COORDINATE_X=(TS_LCD_COEF[2]*TS_COORDINATE_X+TS_LCD_COEF[3]*TS_COORDINATE_Y+TS_LCD_COEF[4])/TS_LCD_COEF[1];	
   		COORDINATE_Y=(TS_LCD_COEF[5]*TS_COORDINATE_X+TS_LCD_COEF[6]*TS_COORDINATE_Y+TS_LCD_COEF[7])/TS_LCD_COEF[1];
   		}
    
}



void Task_BK_Bmp1 (void *pdata)
{
	U8 err;
	
	while(1){
		Uart_Printf("Task_BK_Bmp1\n");
		//�����ź������ȴ��жϣ��������
		OSSemPend(Key0,0,&err);
		Lcd_ClearScr(0xffff);
		//��ʾλͼ
		Paint_Bmp(0,0,LCD_X,LCD_Y,slamdunk1);
		
	}
}

void Task_BK_Bmp2 (void *pdata)
{
	U8 err;
	while(1){
		Uart_Printf("Task_BK_Bmp2\n");
		//�����ź������ȴ��жϣ��������
		OSSemPend(Key2,0,&err);
		Lcd_ClearScr(0xffff);
		//��ʾλͼ
		Paint_Bmp(0,0,LCD_X,LCD_Y,slamdunk2);
		
	}

}

void Task_LED(void *pdata)
{
	while(1){
		Uart_Printf("Task_LED\n");
		Led_On();
		OSTimeDly(3000);
		Led_Off();
		OSTimeDly(3000);
		}
}

__inline void cpu_init(void)
{
	
    int i;
    U8 key;
	U32 mpll_val = 0 ;
    
    Port_Init();  //�˿ڳ�ʼ��
    Isr_Init();   //�жϳ�ʼ��
    key_Eint();
    i = 2 ;	//don't use 100M!
		//boot_params.cpu_clk.val = 3;
	switch ( i ) {
	case 0:	//200
		key = 12;
		mpll_val = (92<<12)|(4<<4)|(1);
		break;
	case 1:	//300
		key = 13;
		mpll_val = (67<<12)|(1<<4)|(1);
		break;
	case 2:	//400
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	case 3:	//440!!!
		key = 14;
		mpll_val = (102<<12)|(1<<4)|(1);
		break;
	default:
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	}
	
	//init FCLK=400M, so change MPLL first
	ChangeMPllValue((mpll_val>>12)&0xff, (mpll_val>>4)&0x3f, mpll_val&3);
	ChangeClockDivider(key, 12);
	cal_cpu_bus_clk();
}