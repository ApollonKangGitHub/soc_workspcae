#include <tools.h>
#include <interrupt.h>
#include <adc.h>
#include <soc_s3c2440_public.h>

#define PHOTOSENSITIVE_DETECT_DARKEN	(TRUE)		/* 电阻检测到变暗 */
#define PHOTOSENSITIVE_DETECT_LIGHT		(FALSE)		/* 电阻检测到变亮 */

/* 中断处理函数 */
static void * photosensitive_eint15_handle(void * pArgv)
{
	BOOL isDarken = FALSE;
	isDarken = (GPGDATr & GPGDAT_GPG7_DATA_BITSf) 
			? (PHOTOSENSITIVE_DETECT_DARKEN) 
			: (PHOTOSENSITIVE_DETECT_LIGHT);

	/* 
	 * 读取GPGDATA寄存器判断是上升沿还是下降沿触发
	 * 比较器的正相还是反相输出，决定可调电阻还是光敏电阻阻值大
	 * 进一步得出是流明值/光通量变大还是变小
	 */	
	if (isDarken)
	{
		print_screen("\r\n change to darken.");
	}
	else
	{
		print_screen("\r\n change to light.");
	}
}

/* 光敏电阻外部中断初始化 */
static void photosensitive_eint_init(void)
{
	/* 配置GPG7为中断引脚 */
	GPGCONr &= ~(GPGCON_GPG7_CONF_BITSf);
	GPGCONr |= (GPGCON_GPG7_EINT15 << GPGCON_GPG7_CONF_START);

	/* 配置GPG7/EINT15为双边沿触发用于光敏电阻 */
	EXTINT1r &= ~(EXTINT_EINT_MASK << EXTINT1_CON_EINT15_START_BIT);
	EXTINT1r |= (EXTINT_CON_BOTH_EDGE_TRIGGER << EXTINT1_CON_EINT15_START_BIT);
}

/* 光敏电阻相关初始化 */
void photosensitive_init(void)
{
	photosensitive_eint_init();
	interrupt_register(interrupt_type_EXT_INT15, photosensitive_eint15_handle);
}



