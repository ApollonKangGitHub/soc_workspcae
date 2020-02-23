#include <tools.h>
#include <log.h>
#include <adc.h>
#include <interrupt.h>
#include <touchScreen.h>
#include <soc_s3c2440_public.h>

/* 
 * BUG:S3C2440在启动后即使没有按下也会有一个中断产生，
 * 包含按下和松开都有，所以第一个中断不操作
 */
static BOOL gTouchScreenFirstInterrupt = FALSE;

static void touchScreen_wait_pen_down(void)
{	
	/* 等待第一次按下触发中断 */
	ADCTSCr = (ADCTSC_WAIT_PEN_DOWN \
				| ADCTSC_PULL_UP_ENABLE \
				| ADCTSC_YM_ENABLE \
				| ADCTSC_YP_DISABLE \
				| ADCTSC_XM_DISABLE \
				| ADCTSC_XP_DISABLE \
				| ADCTSC_XY_PST_WAIT_INT_MODE);
}

static void touchScreen_wait_pen_up(void)
{	
	/* 等待松开触摸屏 */
	ADCTSCr = (ADCTSC_WAIT_PEN_UP \
				| ADCTSC_PULL_UP_ENABLE \
				| ADCTSC_YM_ENABLE \
				| ADCTSC_YP_DISABLE \
				| ADCTSC_XM_DISABLE \
				| ADCTSC_XP_DISABLE \
				| ADCTSC_XY_PST_WAIT_INT_MODE);
}

static void touchScreen_ts_interrupt_handle(void)
{
	/* 触摸屏被按下 */
	if (gTouchScreenFirstInterrupt && 
		(ADCDATA_UPDOWN_DOWN == (SOC_S3C2440_REG_BIT_GET(ADCDAT0r, ADCDATA_UPDOWN_BIT))))
	{
		print_screen("\r\n Stylus down interrupt occurred.");
		touchScreen_wait_pen_up();
	}

	/* 触摸屏松开 */
	if (gTouchScreenFirstInterrupt &&
		(ADCDATA_UPDOWN_UP == (SOC_S3C2440_REG_BIT_GET(ADCDAT0r, ADCDATA_UPDOWN_BIT))))
	{	
		print_screen("\r\n Stylus up interrupt occurred.");
		touchScreen_wait_pen_down();
	}	

	gTouchScreenFirstInterrupt = TRUE;
}

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
	if (SOC_S3C2440_REG_BIT_GET(SUBSRCPNDr, SUBSRCPND_INT_ADC_TS_BIT))
	{
		touchScreen_ts_interrupt_handle();
	}
	
	/* ADC转换完成中断 */
	if (SOC_S3C2440_REG_BIT_GET(SUBSRCPNDr, SUBSRCPND_INT_ADC_ADC_BIT))
	{
		//touchScreen_adc_interrupt_handle();
	}
}

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

static void touchScreen_adc_ts_reg_init(void)
{
	adc_init();
}

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
}
