#include <interrupt.h>
#include <tools.h>
#include <soc_s3c2440_public.h>

/*
 * 1、SRCPND[R/W] 属于中断控制器的一部分，用来指示哪个中断产生了(值为1)，处理后清除对应位
 * 由SRCPND可以判定EINT0~EINT3，大于EINT3的中断需要EINTPEND进一步判定
 * 
 * 2、INTMASK[R/W] 寄存器也是中断控制器的一部分，和中断源的EINTMASK 寄存器类似(1表示屏蔽)，
 * INTMASK 用来在中断控制器上屏蔽中断，EINTMASK用来在中断源处屏蔽中断
 * 
 * 3、INTPND[R/W] 用来指示哪个中断正在被CPU处理
 * 4、INTOFFSET[RO] 用来指示INTPND中哪一位被设置为1，
 * 	  如EINT4~EINT7正在被处理(bit4=1)，则INTOFFSET=4，而INTPND=2^4=16
 *    INTOFFSET为只读的，当INTPND被清除时，INTOFFSET会自动同步
 * 
 * EINTPEND 寄存器和 EINTMASK寄存器都不控制EINT0~EINT3，
 * EINT4~EINT23T都单独有一个控制bit
 *
 * SRCPND 寄存器、 INTPND 和 INTMASK寄存器对于EINT0~EINT3单独控制，
 * EINT4~EINT7、EINT8~EINT23T公用一个控制bit:
 * bit0~bit3分别对应EINT0~EINT3
 * bit4由EINT4~EINT7公用
 * bit5由EINT8~EINT23公用
 */

/* 中断控制器初始化 */
void interrupt_controller_init(void)
{
	volatile uint32 regValue = 0x0;

	/* 在中断控制器处使能S2~S4按键中断 */
	regValue = INTMASKr;
	
	regValue &= ~(EINT0_BITSf | EINT2_BITSf | EINT8_23_BITSf);
	regValue |= (INTMASK_INT_ENABLE << EINT0_START_BIT);
	regValue |= (INTMASK_INT_ENABLE << EINT2_START_BIT);
	regValue |= (INTMASK_INT_ENABLE << EINT8_23_START_BIT);

	INTMASKr = regValue;
}

/* 按键中断初始化，全部设置为上升沿下降沿双触发 */
void interrupt_key_init(void)
{
	/* 
	 * 1、配置GPIO中断引脚
	 * 四个按键S2、S3、S4、S5对应EINT0、EINT2、EINT11、EINT19
	 * 即GPFCON和GPGCON两个寄存器
	 */

	/* GPFCON寄存器和寄存器GPGCON清空 */
	GPFCONr &= ~(GPFCON_GPF0_CONF_BITSf | GPFCON_GPF2_CONF_BITSf); 	
	GPGCONr &= ~(GPGCON_GPG3_CONF_BITSf | GPGCON_GPG11_CONF_BITSf); 
	
	/* 相关引脚配置为中断引脚 */
	GPFCONr |= ((GPFCON_EINT << GPIOCON_PIN0_START_BIT) | (GPFCON_EINT << GPIOCON_PIN2_START_BIT));
	GPGCONr |= ((GPGCON_EINT << GPIOCON_PIN3_START_BIT) | (GPGCON_EINT << GPIOCON_PIN11_START_BIT));
	
	/* 2、设置中断触发方式：双边沿触发 */
	EXTINT0r &= ~(EXTINT0_CON_EINT0_BITSf | EXTINT0_CON_EINT2_BITSf);
	EXTINT1r &= ~EXTINT1_CON_EINT11_BITSf;
	EXTINT2r &= ~EXTINT2_CON_EINT19_BITSf;
	
	EXTINT0r |= (EXTINT_CON_BOTH_EDGE_TRIGGER << EXTINT0_CON_EINT0_START_BIT);	/* s2双边沿触发中断 */
	EXTINT0r |= (EXTINT_CON_BOTH_EDGE_TRIGGER << EXTINT0_CON_EINT2_START_BIT);	/* s3双边沿触发中断 */
	EXTINT1r |= (EXTINT_CON_BOTH_EDGE_TRIGGER << EXTINT1_CON_EINT11_START_BIT);	/* s4双边沿触发中断 */
	EXTINT2r |= (EXTINT_CON_BOTH_EDGE_TRIGGER << EXTINT2_CON_EINT19_START_BIT);	/* s5双边沿触发中断 */

	/* 3、设置EINTMASK使能中断,EINT0和EINT2默认使能不能屏蔽 */
	EINTMASKr &= ~(EINTMASK_EINT11_BITSf | EINTMASK_EINT19_BITSf);
	
	EINTMASKr |= (EINTMASK_EINT_ENABLE << EINTMASK_EINT11_START_BIT);
	EINTMASKr |= (EINTMASK_EINT_ENABLE << EINTMASK_EINT19_START_BIT);
}

/* 中断状态获取，返回指定类型中断是否产生，以及实际产生的中断类型 */
BOOL interrupt_status_get
(
	interrupt_type_t type, 			/* 判断的中断类型 */
	interrupt_type_t * real			/* 当前CPU处理的实际中断 */
)
{

}

/* 中断状态清除 */
void interrupt_status_clear(interrupt_type_t type)
{

}

