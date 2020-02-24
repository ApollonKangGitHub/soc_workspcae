#include <tools.h>
#include <log.h>
#include <adc.h>
#include <timer.h>
#include <interrupt.h>
#include <touchScreen.h>
#include <soc_s3c2440_public.h>

static BOOL gTimerEnable = FALSE;

/* 软件控制定时器启动 */
static touchScreen_timer_enable(BOOL enable)
{
	gTimerEnable = (enable);
}

/* 等待触摸屏按下触发中断 */
static void touchScreen_wait_pen_down(void)
{	
	ADCTSCr = (ADCTSC_WAIT_PEN_DOWN \
				| ADCTSC_PULL_UP_ENABLE \
				| ADCTSC_YM_ENABLE \
				| ADCTSC_YP_DISABLE \
				| ADCTSC_XM_DISABLE \
				| ADCTSC_XP_DISABLE \
				| ADCTSC_XY_PST_WAIT_INT_MODE);
}

/* 等待松开触摸屏触发中断 */
static void touchScreen_wait_pen_up(void)
{	
	ADCTSCr = (ADCTSC_WAIT_PEN_UP \
				| ADCTSC_PULL_UP_ENABLE \
				| ADCTSC_YM_ENABLE \
				| ADCTSC_YP_DISABLE \
				| ADCTSC_XM_DISABLE \
				| ADCTSC_XP_DISABLE \
				| ADCTSC_XY_PST_WAIT_INT_MODE);
}

/* 设置为自动测量模式 */
static void touchScreen_auto_measure_convert(void)
{	
	ADCTSCr |= (ADCTSC_AUTO_PST_AUTO | ADCTSC_XY_PST_NO_OPER_MODE);
}

/* 是否是触摸屏按下 */
static BOOL touchScreen_isDown(void)
{
#if 1
	return (ADCDATA_UPDOWN_DOWN == (SOC_S3C2440_REG_BIT_GET(ADCDAT0r, ADCDATA_UPDOWN_BIT)));
#else
	return (ADCTSC_WAIT_PEN_DOWN == (SOC_S3C2440_REG_BIT_GET(ADCTSCr, ADCTSC_UD_SEN_START)));
#endif
}

/* 是否是触摸屏松开 */
static BOOL touchScreen_isUp(void)
{
#if 1
	return (ADCDATA_UPDOWN_UP == (SOC_S3C2440_REG_BIT_GET(ADCDAT0r, ADCDATA_UPDOWN_BIT)));
#else
	return (ADCTSC_WAIT_PEN_UP == (SOC_S3C2440_REG_BIT_GET(ADCTSCr, ADCTSC_UD_SEN_START)));
#endif
}

/* 判断是否是AD转换完成中断 */
static BOOL touchScreen_isAdcInterrupt(void)
{
	return SOC_S3C2440_REG_BIT_GET(SUBSRCPNDr, SUBSRCPND_INT_ADC_ADC_BIT);
}

/* 判断是否是触摸屏中断 */
static BOOL touchScreen_isTsInterrupt(void)
{
	return SOC_S3C2440_REG_BIT_GET(SUBSRCPNDr, SUBSRCPND_INT_ADC_TS_BIT);
}

/* 触摸屏中断处理函数 */
static void touchScreen_ts_interrupt_handle(void)
{
	/* 触摸屏被按下 */
	if (touchScreen_isDown())
	{
		print_screen("\r\n Stylus down interrupt occurred.");
		touchScreen_auto_measure_convert();

		/* 启动ADC开始AD转换,转换结束会产生ADC中断 */
		adc_start();
	}
	
	/* 触摸屏松开 */
	else if (touchScreen_isUp())
	{	
		print_screen("\r\n Stylus up interrupt occurred.");
		touchScreen_wait_pen_down();
	}	
}

/* AD转换完成产生ADC中断 */
void touchScreen_adc_interrupt_handle(void)
{
	uint32 x = 0;
	uint32 y = 0;

	/* 产生ADC中断时，判断还未松开，则获取触点坐标并打印坐标 */
	if (touchScreen_isDown())
	{
		x = adc_read_x();
		y = adc_read_y();
		print_screen("\r\n x=%d[%x], y=%d[%x]", x, x, y, y);

		/* 启动定时器中断 */
		touchScreen_timer_enable(TRUE);
	}
	
	/* 触摸屏松开则不再需要定时获取触点坐标，关闭定时器，进入等待触摸屏按下状态 */
	if (touchScreen_isUp())
	{
		print_screen("\r\n Stylus up interrupt occurred.");
		touchScreen_timer_enable(FALSE);
		touchScreen_wait_pen_down();
	}	
	
	/* 处理完成等待触摸屏松开 */
	touchScreen_wait_pen_up();
}

/* ADC中断处理函数 */
static void * touchScreen_adc_ts_interrupt_handle(void * pArgv)
{
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "--------------------------------------------------------");
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " adc or ts interrupt ocurred!");
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " ADCUPDNr   :[%X-%x]", ADDR_ADCUPDNr, ADCUPDNr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " ADCCONr    :[%X-%x]", ADDR_ADCCONr, ADCCONr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " ADCDLYr    :[%X-%x]", ADDR_ADCDLYr, ADCDLYr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " ADCTSCr    :[%X-%x]", ADDR_ADCTSCr, ADCTSCr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " ADCDAT0r   :[%X-%x]", ADDR_ADCCONr, ADCDAT0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " ADCDAT1r   :[%X-%x]", ADDR_ADCDLYr, ADCDAT1r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "--------------------------------------------------------");

	/* 触摸屏中断 */
	if (touchScreen_isTsInterrupt())
	{
		touchScreen_ts_interrupt_handle();
	}
	
	/* ADC转换完成中断 */
	if (touchScreen_isAdcInterrupt())
	{
		touchScreen_adc_interrupt_handle();
	}
}

/* TS中断处理函数 */
static void touchScreen_adc_ts_interrupt_init(void)
{
	/* 清除中断 */
 	SOC_S3C2440_REG_BIT_SET(SUBSRCPNDr, SUBSRCPND_INT_ADC_ADC_BIT, SUBSRCPND_INT_CLEAR);
	SOC_S3C2440_REG_BIT_SET(SUBSRCPNDr, SUBSRCPND_INT_ADC_TS_BIT, SUBSRCPND_INT_CLEAR);

	/* 注册中断处理函数，中断处理函数有去使能INT_ADC中断(INTMASKr) */
	(void)interrupt_register(interrupt_type_INT_ADC, touchScreen_adc_ts_interrupt_handle);

	/*
	 * 使能ADC和TS中断(INTSUBMASKr)
	 * INT_ADC_S [10]:
	 *		0 = Service available, 1 = Masked. Init status is 1.
	 * INT_TC [9]: 
	 * 		0 = Service available, 1 = Masked. Init status is 1.
	 */
 	SOC_S3C2440_REG_BIT_SET(INTSUBMASKr, INTSUBMASK_INT_ADC_TS_BIT, INTSUBMASK_INT_ENABLE);
	SOC_S3C2440_REG_BIT_SET(INTSUBMASKr, INTSUBMASK_INT_ADC_ADC_BIT, INTSUBMASK_INT_ENABLE);
}

/* 触摸屏ADC寄存器初始化 */
static void touchScreen_adc_ts_reg_init(void)
{
	adc_init();

	/* 从按下到测量需要一定延时时间，得到的测量结果才够准确 */
	adc_delay(10);
}

/* 定时器处理函数，用于长按和滑动操作 */
void * touchScreen_timer_handle(void * pArgv)
{
	if (!gTimerEnable)
	{
		return NULL;
	}

	/* 触摸屏松开则不再需要定时获取触点坐标，关闭定时器，进入等待触摸屏按下状态 */
	if (touchScreen_isUp())
	{	
		print_screen("\r\n Stylus up interrupt occurred.");
		touchScreen_timer_enable(FALSE);
		touchScreen_wait_pen_down();
		return NULL;
	}
	
	/* 
	 * 每10ms产生一次定时器中断，如果产生定时器中断时，
	 * 触摸屏被按下则再次触发一次测量与AD转换
	 */
	if (touchScreen_isDown()) 
	{
		touchScreen_auto_measure_convert();

		/* 启动ADC开始AD转换,转换结束会产生ADC中断 */
		adc_start();
	}
	
	return NULL;
}

/* 触摸屏初始化 */
void touchScreen_init(void)
{
	/* 设置触摸屏相关寄存器 */
	(void)touchScreen_adc_ts_reg_init();

	/* 设置中断 */
	(void)touchScreen_adc_ts_interrupt_init();

	/* 让触摸屏控制器进入等待中断模式 */
	(void)touchScreen_wait_pen_down();

	print_screen("\r\n --------------------------------------------------------");
	print_screen("\r\n touchScreen init succeed!!!");
	print_screen("\r\n INTSUBMASKr:[%X-%x]", ADDR_INTSUBMASKr, INTSUBMASKr);
	print_screen("\r\n SUBSRCPNDr :[%X-%x]", ADDR_SUBSRCPNDr, SUBSRCPNDr);
	print_screen("\r\n ADCUPDNr   :[%X-%x]", ADDR_ADCUPDNr, ADCUPDNr);
	print_screen("\r\n ADCCONr    :[%X-%x]", ADDR_ADCCONr, ADCCONr);
	print_screen("\r\n ADCDLYr    :[%X-%x]", ADDR_ADCDLYr, ADCDLYr);
	print_screen("\r\n ADCTSCr    :[%X-%x]", ADDR_ADCTSCr, ADCTSCr);
	print_screen("\r\n ADCDAT0r   :[%X-%x]", ADDR_ADCCONr, ADCDAT0r);
	print_screen("\r\n ADCDAT1r   :[%X-%x]", ADDR_ADCDLYr, ADCDAT1r);
	print_screen("\r\n --------------------------------------------------------");

		
	/* 注册定时器回掉函数, 启动定时器 */
	timer_register("touch screen timer 0", touchScreen_timer_handle);
}
