#ifndef _SOC_S3C2440_TIMER_FIELD_H_
#define _SOC_S3C2440_TIMER_FIELD_H_

#define TCFG0_PRESCALER0_BITSf			(0xFF << 0)		/* These 8 bits determine prescaler value for Timer 0 and 1. */
#define TCFG0_PRESCALER1_BITSf			(0xFF << 8)		/* These 8 bits determine prescaler value for Timer 2, 3 and 4. */
#define TCFG0_DEAD_ZONE_LEN_BITSf		(0xFF << 16)
#define TCFG0_RESERVED_BITSf			(0xFF << 24)

#define TCFG0_PRESCALER0_START_BIT		(0)
#define TCFG0_PRESCALER1_START_BIT		(8)
#define TCFG0_DEAD_ZONE_LEN_START_BIT	(16)
#define TCFG0_RESERVED_START_BIT		(24)

/*
 * MUX select value:
 * 0000 = 1/2 
 * 0001 = 1/4 
 * 0010 = 1/8 
 * 0011 = 1/16 
 * 01xx = External TCLKn
 */
#define TCFG1_MUX0_BITSf				(0xF << 0)		/* Select MUX input for PWM Timer0. */
#define TCFG1_MUX1_BITSf				(0xF << 4)		/* Select MUX input for PWM Timer1. */
#define TCFG1_MUX2_BITSf				(0xF << 8)		/* Select MUX input for PWM Timer2. */
#define TCFG1_MUX3_BITSf				(0xF << 12)		/* Select MUX input for PWM Timer3. */
#define TCFG1_MUX4_BITSf				(0xF << 16)		/* Select MUX input for PWM Timer4. */

/*
 * DMA select value
 * 0000 = No select (all interrupt) 
 * 0001 = Timer0
 * 0010 = Timer1 
 * 0011 = Timer2 
 * 0100 = Timer3 
 * 0101 = Timer4
 * 0110 = Reserved
 */
#define TCFG1_DMA_MODE_BITSf			(0xF << 20)		/* Select DMA request channel */

#define TCFG1_RESERVED_BITSf			(0xF << 24)

#define TCFG1_MUX0_START_BIT			(0)
#define TCFG1_MUX1_START_BIT			(4)
#define TCFG1_MUX2_START_BIT			(8)
#define TCFG1_MUX3_START_BIT			(12)
#define TCFG1_MUX4_START_BIT			(16)
#define TCFG1_DMA_MODE_START_BIT		(20)
#define TCFG1_RESERVED_START_BIT		(24)

#define TCFG1_MUX_PWM_1_2				(0x0)	/* 0000 = 1/2  */
#define TCFG1_MUX_PWM_1_4				(0x1)	/* 0001 = 1/4 */
#define TCFG1_MUX_PWM_1_8				(0x2)	/* 0010 = 1/8  */
#define TCFG1_MUX_PWM_1_16				(0x3)	/* 0011 = 1/16  */
#define TCFG1_MUX_PWM_1_EXT				(0x4)	/* 01xx = External TCLK1 */

#define TCFG1_DMA_REQ_ALL_INTERRUPT		(0x0)	/* 0000 = No select (all interrupt) */
#define TCFG1_DMA_REQ_TIMER0			(0x1)	/* 0001 = Timer0 */
#define TCFG1_DMA_REQ_TIMER1			(0x2)	/* 0010 = Timer1 */
#define TCFG1_DMA_REQ_TIMER2			(0x3) 	/* 0011 = Timer2 */
#define TCFG1_DMA_REQ_TIMER3			(0x4)	/* 0100 = Timer3 */
#define TCFG1_DMA_REQ_TIMER4			(0x5)	/* 0101 = Timer4 */
#define TCFG1_DMA_REQ_RESERVED			(0x6) 	/* 0110 = Reserved */

#define TCON_TIMER0_BITSf				(0XF << 0)
#define TCON_TIMER1_BITSf				(0XF << 8)
#define TCON_TIMER2_BITSf				(0XF << 12)
#define TCON_TIMER3_BITSf				(0XF << 16)
#define TCON_TIMER4_BITSf				(0X7 << 20)

#define TCON_TIMER0_START_BIT			(0)
#define TCON_TIMER1_START_BIT			(8)
#define TCON_TIMER2_START_BIT			(12)
#define TCON_TIMER3_START_BIT			(16)
#define TCON_TIMER4_START_BIT			(20)

#define TCON_TIMER_START_ON				(0x1)	/* 0 = Stop, 1 = Start for Timer 0 */
#define TCON_TIMER_MANUAL_UPDATE		(0x2)	/* 0 = No, operation 1 = Update from TCNTB0 & TCMPB0; The bit has to be cleared at next writing */
#define TCON_TIMER_OUT_INVERTER_ON		(0x4)	/* 0 = Inverter off, 1 = Inverter on for TOUT0 */
#define TCON_TIMER_AUTO_RELOAD_ON		(0x8)	/* 0 = One-shot, 1 = Interval mode(auto reload) */

#endif	/* _SOC_S3C2440_TIMER_FIELD_H_ */