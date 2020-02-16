#include <log.h>
#include <tools.h>
#include <timer.h>
#include <soc_s3c2440_public.h>
#include <interrupt.h>

/* 
 * 注意：在设置自动加载时，TCONr寄存器对应的TIMERn，其
 * TCON_TIMER_MANUAL_UPDATE 位 必须先置为1，表示手动加载初始值TCNTB0r到定时器内部，
 * 然后随后将其置为0，表示之后为自动加载，否则直接设置为0或者直接设置为1都有问题，
 * 因为timer内部的计数器值初始化一直为0，没有手动加载，也不会触发中断并自动加载新值
 */
void timer_0_init(void)
{
	/* 设置TIMER0的时钟 
	 * Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value} 
	 * {prescaler value} = 0~255 
	 * {divider value} = 2, 4, 8, 16
	 * 假设:50000000 / (99+1) / 16 = 31250
	 */
	 
	/* prescaler 0 = 99 */
	SOC_S3C2440_REG_BITS_SET(TCFG0r, \
		TCFG0_PRESCALER0_START_BIT, TCFG0_PRESCALER0_BITSf, 99);
	
	/* MUX0 = 1/16 */
	SOC_S3C2440_REG_BITS_SET(TCFG1r, \
		TCFG1_MUX0_START_BIT, TCFG1_MUX0_BITSf, TCFG1_MUX_PWM_1_16);	
	
	/* 设置TIMER0的初值,15625即0.5s中断一次 */
	TCNTB0r = 15625;
	TCMPB0r = 0;
	
	/* 加载初值, 启动TIMER0 */
	SOC_S3C2440_REG_BITS_SET(TCONr, \
		 TCON_TIMER0_START_BIT, TCON_TIMER0_BITSf, TCON_TIMER_MANUAL_UPDATE);

	/* 设置为自动加载并启动（清除TCON_TIMER_MANUAL_UPDATE） */
	SOC_S3C2440_REG_BITS_SET(TCONr, TCON_TIMER0_START_BIT, \
		 TCON_TIMER0_BITSf, TCON_TIMER_AUTO_RELOAD_ON | TCON_TIMER_START_ON);
}

void timer_0_debug_print(void)
{
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "------------------------------");
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCFG0r :%x", TCFG0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCFG1r :%x", TCFG1r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCONr  :%x", TCONr);
	
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCNTB0r:%x", TCNTB0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCMPB0r:%x", TCMPB0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCNTO0r:%x", TCNTO0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "------------------------------");
}

