#include <tools.h>
#include <log.h>
#include <adc.h>
#include <timer.h>
#include <interrupt.h>
#include <touchScreen.h>
#include <soc_s3c2440_public.h>

/*
 * 注意点：
 * 1、启动ADC转换时，不能进入等待中断模式，他会对AD转换产生影响
 * 2、只有在等待中断模式下，才可以通过ADCDATA0的BIT15进行UP/DOWN的判断，自动测量模式时不可以
 * 3、校准很重要，决定后面使用校准的值对LCD操作的精准度
 */

#define TOUCH_SCREEN_CONTACT_AVERAGE_TIMES	(16)

static BOOL gTimerEnable = FALSE;
static uint32 gTs_x_save = 0;
static uint32 gTs_y_save = 0;
static volatile BOOL gTs_data_valid = FALSE;
static volatile BOOL gPressure = FALSE;

/* 软件控制定时器启动 */
static touchScreen_timer_enable(BOOL enable)
{
	gTimerEnable = (enable);
}

/* 保存触点坐标,只保存一次，保存之后无人读取则不再保存 */
static touchScreen_contact_coordinates_update(uint32 x, uint32 y, BOOL pressure)
{
	if (!gTs_data_valid)
	{
		gTs_x_save = x;
		gTs_y_save = y;
		gPressure = pressure;
		gTs_data_valid = TRUE;
	}
}

/* 获取触点坐标，获取完成后，数据可以被再次ADC中断更新 */
void touchScreen_get_contact_coordinates(uint32 *x, uint32 *y, BOOL * pressure)
{
	while(!gTs_data_valid);
	*x = gTs_x_save;
	*y = gTs_y_save;
	*pressure = gPressure;
	gTs_data_valid = FALSE;
}

/* 等待触摸屏按下触发中断 */
void touchScreen_wait_pen_down(void)
{	
	ADCTSCr = (ADCTSC_WAIT_PEN_DOWN \
				| ADCTSC_PULL_UP_ENABLE \
				| ADCTSC_YM_ENABLE \
				| ADCTSC_YP_DISABLE \
				| ADCTSC_XM_DISABLE \
				| ADCTSC_XP_DISABLE \
				| ADCTSC_XY_PST_WAIT_INT_MODE);
}

/*
 * 等待松开触摸屏触发中断
 * 注意：等待AD中断处理完成后再修改为wait pen up
 * 因为TS中断和TIMER中断里，down处理只是启动了AD转换，并没完全处理王DOWN的流程
 * 如果此时就wait pen up会修改ADCTSC的为非AUTO状态，那么AD转换的值也不会准确
 */
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

/* 退出自动测量模式 */
static void touchScreen_auto_measure_quit(void)
{	
	ADCTSCr &= ~(ADCTSC_AUTO_PST_AUTO | ADCTSC_XY_PST_NO_OPER_MODE);
}

/* 是否是触摸屏按下 */
static BOOL touchScreen_isDown(void)
{
	return (ADCDATA_UPDOWN_DOWN == (SOC_S3C2440_REG_BIT_GET(ADCDAT0r, ADCDATA_UPDOWN_BIT)));
}

/* 是否是触摸屏松开 */
static BOOL touchScreen_isUp(void)
{
	return (ADCDATA_UPDOWN_UP == (SOC_S3C2440_REG_BIT_GET(ADCDAT0r, ADCDATA_UPDOWN_BIT)));
}

/* 是否是自动测量模式 */
static BOOL touchScreen_isAutoMeasureMode(void)
{
	return (ADCTSCr & ADCTSC_AUTO_PST_AUTO);
}

/* 触摸屏中断处理函数 */
static void * touchScreen_ts_interrupt_handle(void * pArgv)
{
	/* 保证判断down、up时不处于adc auto measure模式 */
	(void)touchScreen_auto_measure_quit();

	/* 触摸屏松开 */
	if (touchScreen_isUp())
	{	
		touchScreen_wait_pen_down();
		(void)touchScreen_contact_coordinates_update(0, 0, FALSE);
	}	
	/* 触摸屏被按下,启动ADC的自动转换,转换结束会产生ADC中断 */
	else if (touchScreen_isDown())
	{
		touchScreen_auto_measure_convert();
		adc_start();
	}
}

/* AD转换完成产生ADC中断 */
static void * touchScreen_adc_interrupt_handle(void * pArgv)
{
	uint32 x = 0;
	uint32 y = 0;
	static volatile uint32 x_sum = 0;
	static volatile uint32 y_sum = 0;
	static volatile uint32 adcTimes = 0;
	
	/* 保证判断down、up时不处于adc auto measure模式 */
	(void)touchScreen_wait_pen_up();

	/* 产生ADC中断时，判断还未松开，则获取触点坐标并打印坐标 */
	if (touchScreen_isDown())
	{
		x_sum += adc_read_x();
		y_sum += adc_read_y();
		adcTimes++;

		/* 更新触点坐标平均值 */
		if (TOUCH_SCREEN_CONTACT_AVERAGE_TIMES == adcTimes)
		{
			x = x_sum / TOUCH_SCREEN_CONTACT_AVERAGE_TIMES;
			y = y_sum / TOUCH_SCREEN_CONTACT_AVERAGE_TIMES;
			(void)touchScreen_contact_coordinates_update(x, y, TRUE);
			adcTimes = 0;
			x_sum = 0;
			y_sum = 0;
			
			/* 先设置TS为等待up中断模式 */
			(void)touchScreen_wait_pen_up();		
			/* 启动定时器中断 */
			(void)touchScreen_timer_enable(TRUE);
		}		
		else
		{
			/* 再次启动ADC */
			touchScreen_auto_measure_convert();
			adc_start();
		}
	}
	
	/* 触摸屏松开则不再需要定时获取触点坐标，关闭定时器，进入等待触摸屏按下状态 */
	else if (touchScreen_isUp())
	{
		(void)touchScreen_timer_enable(FALSE);
		(void)touchScreen_wait_pen_down();
		(void)touchScreen_contact_coordinates_update(0, 0, FALSE);
		adcTimes = 0;
		x_sum = 0;
		y_sum = 0;
	}	
}

/* TS中断处理函数 */
void touchScreen_adc_ts_interrupt_init(void)
{
	/* 注册TS、ADC中断处理函数 */
	(void)interrupt_register(interrupt_type_INT_ADC, touchScreen_adc_interrupt_handle);
	(void)interrupt_register(interrupt_type_INT_TSC, touchScreen_ts_interrupt_handle);
}

/* 触摸屏ADC寄存器初始化 */
void touchScreen_adc_ts_reg_init(void)
{
	adc_init();

	/* 从按下到测量需要一定延时时间，得到的测量结果才够准确 */
	adc_delay(10);
}

/* 定时器处理函数，用于长按和滑动操作 */
void * touchScreen_timer_handle(void * pArgv)
{
	/* 定时器不响应或自动测量模式直接返回 */
	if (!gTimerEnable || (touchScreen_isAutoMeasureMode()))
	{
		return NULL;
	}

	/* 触摸屏松开则不再需要定时获取触点坐标，关闭定时器，进入等待触摸屏按下状态 */
	if (touchScreen_isUp())
	{	
		touchScreen_timer_enable(FALSE);
		touchScreen_wait_pen_down();
		touchScreen_contact_coordinates_update(0,0,FALSE);
		return NULL;
	}
	
	/* 
	 * 每10ms产生一次定时器中断，如果产生定时器中断时，
	 * 触摸屏被按下则再次触发一次测量与AD转换
	 */
	else if (touchScreen_isDown()) 
	{
		touchScreen_auto_measure_convert();
		adc_start();
	}

	return NULL;
}


