#ifndef _SCO_S3C2440_INTERRUPT_FIELD_H_
#define _SCO_S3C2440_INTERRUPT_FIELD_H_

/***************************************外部中断方式控制寄存器字段*****************************************/
/* 各外部中断字段 */
#define EXTINT0_CON_EINT0_START_BIT		(0)
#define EXTINT0_CON_EINT1_START_BIT		(4)
#define EXTINT0_CON_EINT2_START_BIT		(8)
#define EXTINT0_CON_EINT3_START_BIT		(12)
#define EXTINT0_CON_EINT4_START_BIT		(16)
#define EXTINT0_CON_EINT5_START_BIT		(20)
#define EXTINT0_CON_EINT6_START_BIT		(24)
#define EXTINT0_CON_EINT7_START_BIT		(28)

#define EXTINT1_CON_EINT8_START_BIT		(0)
#define EXTINT1_CON_EINT9_START_BIT		(4)
#define EXTINT1_CON_EINT10_START_BIT	(8)
#define EXTINT1_CON_EINT11_START_BIT	(12)
#define EXTINT1_CON_EINT12_START_BIT	(16)
#define EXTINT1_CON_EINT13_START_BIT	(20)
#define EXTINT1_CON_EINT14_START_BIT	(24)
#define EXTINT1_CON_EINT15_START_BIT	(28)

#define EXTINT2_CON_EINT16_START_BIT	(0)
#define EXTINT2_CON_EINT17_START_BIT	(4)
#define EXTINT2_CON_EINT18_START_BIT	(8)
#define EXTINT2_CON_EINT19_START_BIT	(12)
#define EXTINT2_CON_EINT20_START_BIT	(16)
#define EXTINT2_CON_EINT21_START_BIT	(20)
#define EXTINT2_CON_EINT22_START_BIT	(24)
#define EXTINT2_CON_EINT23_START_BIT	(28)

/* signaling method of the EINT0~EINT23  */
#define EXTINT0_CON_EINT0_BITSf		(0x7 << 0)
#define EXTINT0_CON_EINT1_BITSf		(0x7 << 4)
#define EXTINT0_CON_EINT2_BITSf		(0x7 << 8)
#define EXTINT0_CON_EINT3_BITSf		(0x7 << 12)
#define EXTINT0_CON_EINT4_BITSf		(0x7 << 16)
#define EXTINT0_CON_EINT5_BITSf		(0x7 << 20)
#define EXTINT0_CON_EINT6_BITSf		(0x7 << 24)
#define EXTINT0_CON_EINT7_BITSf		(0x7 << 28)

#define EXTINT1_CON_EINT8_BITSf		(0x7 << 0)
#define EXTINT1_CON_EINT9_BITSf		(0x7 << 4)
#define EXTINT1_CON_EINT10_BITSf	(0x7 << 8)
#define EXTINT1_CON_EINT11_BITSf	(0x7 << 12)
#define EXTINT1_CON_EINT12_BITSf	(0x7 << 16)
#define EXTINT1_CON_EINT13_BITSf	(0x7 << 20)
#define EXTINT1_CON_EINT14_BITSf	(0x7 << 24)
#define EXTINT1_CON_EINT15_BITSf	(0x7 << 28)

#define EXTINT2_CON_EINT16_BITSf	(0x7 << 0)
#define EXTINT2_CON_EINT17_BITSf	(0x7 << 4)
#define EXTINT2_CON_EINT18_BITSf	(0x7 << 8)
#define EXTINT2_CON_EINT19_BITSf	(0x7 << 12)
#define EXTINT2_CON_EINT20_BITSf	(0x7 << 16)
#define EXTINT2_CON_EINT21_BITSf	(0x7 << 20)
#define EXTINT2_CON_EINT22_BITSf	(0x7 << 24)
#define EXTINT2_CON_EINT23_BITSf	(0x7 << 28)

/* Filter enable for EINT8~EINT23 */
#define EXTINT1_CON_FLTEN8_BITSf	(0x1 << 3)
#define EXTINT1_CON_FLTEN9_BITSf	(0x1 << 7)
#define EXTINT1_CON_FLTEN10_BITSf	(0x1 << 11)
#define EXTINT1_CON_FLTEN11_BITSf	(0x1 << 15)
#define EXTINT1_CON_FLTEN12_BITSf	(0x1 << 19)
#define EXTINT1_CON_FLTEN13_BITSf	(0x1 << 23)
#define EXTINT1_CON_FLTEN14_BITSf	(0x1 << 27)
#define EXTINT1_CON_FLTEN15_BITSf	(0x1 << 31)

#define EXTINT2_CON_FLTEN16_BITSf	(0x1 << 3)
#define EXTINT2_CON_FLTEN17_BITSf	(0x1 << 7)
#define EXTINT2_CON_FLTEN18_BITSf	(0x1 << 11)
#define EXTINT2_CON_FLTEN19_BITSf	(0x1 << 15)
#define EXTINT2_CON_FLTEN20_BITSf	(0x1 << 19)
#define EXTINT2_CON_FLTEN21_BITSf	(0x1 << 23)
#define EXTINT2_CON_FLTEN22_BITSf	(0x1 << 27)
#define EXTINT2_CON_FLTEN23_BITSf	(0x1 << 31)

#define EXTINT_EINT_MASK			(0x7)

/* 中断触发方式 */
#define EXTINT_CON_LOW_LEVEL				(0x0)		/* 低电平触发:0b'000 */
#define EXTINT_CON_HIGH_LEVEL				(0x1)		/* 高电平触发:0b'001 */

/* 跳变沿触发时，bit0都不关心，0/1都可以，这里统一设置为1 */
#define EXTINT_CON_FALL_EDGE_TRIGGER		(0x3)		/* 下降沿触发:0b'01x */
#define EXTINT_CON_RISE_EDGE_TRIGGER		(0x5)		/* 上升沿触发:0b'10x */
#define EXTINT_CON_BOTH_EDGE_TRIGGER		(0x7)		/* 下降沿、上升沿均触发:0b'11x */

/* 特定中断过滤:EINT8~EINT23 */
#define EXTINT_CON_FLTEN_DISABLE			(0x0)		/* Filter Disable */
#define EXTINT_CON_FLTEN_ENABLE				(0x1)		/* Filter Enable */


/***************************************外部中断屏蔽控制寄存器字段、中断状态寄存器字段*****************************************/

/* EINT0~EINT3没有被控制,因为其可以直达中断控制器，而不需要胫骨EINTMASK这一层控制 */
#define EINTMASK_EINT4_START_BIT				(4)
#define EINTMASK_EINT5_START_BIT				(5)
#define EINTMASK_EINT6_START_BIT				(6)
#define EINTMASK_EINT7_START_BIT				(7)
#define EINTMASK_EINT8_START_BIT				(8)
#define EINTMASK_EINT9_START_BIT				(9)
#define EINTMASK_EINT10_START_BIT				(10)
#define EINTMASK_EINT11_START_BIT				(11)
#define EINTMASK_EINT12_START_BIT				(12)
#define EINTMASK_EINT13_START_BIT				(13)
#define EINTMASK_EINT14_START_BIT				(14)
#define EINTMASK_EINT15_START_BIT				(15)
#define EINTMASK_EINT16_START_BIT				(16)
#define EINTMASK_EINT17_START_BIT				(17)
#define EINTMASK_EINT18_START_BIT				(18)
#define EINTMASK_EINT19_START_BIT				(19)
#define EINTMASK_EINT20_START_BIT				(20)
#define EINTMASK_EINT21_START_BIT				(21)
#define EINTMASK_EINT22_START_BIT				(22)
#define EINTMASK_EINT23_START_BIT				(23)


#define EINTMASK_EINT4_BITSf				(0x1 << 4)
#define EINTMASK_EINT5_BITSf				(0x1 << 5)
#define EINTMASK_EINT6_BITSf				(0x1 << 6)
#define EINTMASK_EINT7_BITSf				(0x1 << 7)
#define EINTMASK_EINT8_BITSf				(0x1 << 8)
#define EINTMASK_EINT9_BITSf				(0x1 << 9)
#define EINTMASK_EINT10_BITSf				(0x1 << 10)
#define EINTMASK_EINT11_BITSf				(0x1 << 11)
#define EINTMASK_EINT12_BITSf				(0x1 << 12)
#define EINTMASK_EINT13_BITSf				(0x1 << 13)
#define EINTMASK_EINT14_BITSf				(0x1 << 14)
#define EINTMASK_EINT15_BITSf				(0x1 << 15)
#define EINTMASK_EINT16_BITSf				(0x1 << 16)
#define EINTMASK_EINT17_BITSf				(0x1 << 17)
#define EINTMASK_EINT18_BITSf				(0x1 << 18)
#define EINTMASK_EINT19_BITSf				(0x1 << 19)
#define EINTMASK_EINT20_BITSf				(0x1 << 20)
#define EINTMASK_EINT21_BITSf				(0x1 << 21)
#define EINTMASK_EINT22_BITSf				(0x1 << 22)
#define EINTMASK_EINT23_BITSf				(0x1 << 23)


#define EINTMASK_MIN				(EINTMASK_EINT4_START_BIT)
#define EINTMASK_MAX				(EINTMASK_EINT23_START_BIT)


#define EINTMASK_EINT_ENABLE				(0)		/* 使能中断 */
#define EINTMASK_EINT_DISABLE				(1)		/* 关闭中断 */

/* 和EINTMASK一样，没有EINT0~EINT3的控制/状态位 */
#define EINTPEND_EINT4_START_BIT				(4)
#define EINTPEND_EINT5_START_BIT				(5)
#define EINTPEND_EINT6_START_BIT				(6)
#define EINTPEND_EINT7_START_BIT				(7)
#define EINTPEND_EINT8_START_BIT				(8)
#define EINTPEND_EINT9_START_BIT				(9)
#define EINTPEND_EINT10_START_BIT				(10)
#define EINTPEND_EINT11_START_BIT				(11)
#define EINTPEND_EINT12_START_BIT				(12)
#define EINTPEND_EINT13_START_BIT				(13)
#define EINTPEND_EINT14_START_BIT				(14)
#define EINTPEND_EINT15_START_BIT				(15)
#define EINTPEND_EINT16_START_BIT				(16)
#define EINTPEND_EINT17_START_BIT				(17)
#define EINTPEND_EINT18_START_BIT				(18)
#define EINTPEND_EINT19_START_BIT				(19)
#define EINTPEND_EINT20_START_BIT				(20)
#define EINTPEND_EINT21_START_BIT				(21)
#define EINTPEND_EINT22_START_BIT				(22)
#define EINTPEND_EINT23_START_BIT				(23)

#define EINTPEND_MIN					(EINTPEND_EINT4_START_BIT)
#define EINTPEND_MAX					(EINTPEND_EINT23_START_BIT)

#define EINTPEND_EINT4v					(0x1 << EINTPEND_EINT4_START_BIT)
#define EINTPEND_EINT5v					(0x1 << EINTPEND_EINT5_START_BIT)
#define EINTPEND_EINT6v					(0x1 << EINTPEND_EINT6_START_BIT)
#define EINTPEND_EINT7v					(0x1 << EINTPEND_EINT7_START_BIT)
#define EINTPEND_EINT8v					(0x1 << EINTPEND_EINT8_START_BIT)
#define EINTPEND_EINT9v					(0x1 << EINTPEND_EINT9_START_BIT)
#define EINTPEND_EINT10v				(0x1 << EINTPEND_EINT10_START_BIT)
#define EINTPEND_EINT11v				(0x1 << EINTPEND_EINT11_START_BIT)
#define EINTPEND_EINT12v				(0x1 << EINTPEND_EINT12_START_BIT)
#define EINTPEND_EINT13v				(0x1 << EINTPEND_EINT13_START_BIT)
#define EINTPEND_EINT14v				(0x1 << EINTPEND_EINT14_START_BIT)
#define EINTPEND_EINT15v				(0x1 << EINTPEND_EINT15_START_BIT)
#define EINTPEND_EINT16v				(0x1 << EINTPEND_EINT16_START_BIT)
#define EINTPEND_EINT17v				(0x1 << EINTPEND_EINT17_START_BIT)
#define EINTPEND_EINT18v				(0x1 << EINTPEND_EINT18_START_BIT)
#define EINTPEND_EINT19v				(0x1 << EINTPEND_EINT19_START_BIT)
#define EINTPEND_EINT20v				(0x1 << EINTPEND_EINT20_START_BIT)
#define EINTPEND_EINT21v				(0x1 << EINTPEND_EINT21_START_BIT)
#define EINTPEND_EINT22v				(0x1 << EINTPEND_EINT22_START_BIT)
#define EINTPEND_EINT23v				(0x1 << EINTPEND_EINT23_START_BIT)


/* 读取相应位为0即未发生中断，为1即发生中断，清除中断时向相应位写1即可 */
#define EINTPEND_STATUS_OCCUR				(1)
#define EINTPEND_STATUS_NO_OCCUR			(0)
#define EINTPEND_CLEAR_WR					(1)

/***************************************中断控制器相关寄存器字段*****************************************/

/* 中断mask寄存器、srcpnd寄存器、intmod寄存器、intpnd寄存器 */
#define EINT0_BITSf					(0x1 << 0)
#define EINT1_BITSf					(0x1 << 1)
#define EINT2_BITSf					(0x1 << 2)
#define EINT3_BITSf					(0x1 << 3)
#define EINT4_7_BITSf				(0x1 << 4)
#define EINT8_23_BITSf				(0x1 << 5)
#define INT_CAM_BITSf				(0x1 << 6)
#define INT_nBATT_FLT_BITSf			(0x1 << 7)
#define INT_TICK_BITSf				(0x1 << 8)
#define INT_WDT_AC97_BITSf			(0x1 << 9)
#define INT_TIMER0_BITSf			(0x1 << 10)
#define INT_TIMER1_BITSf			(0x1 << 11)
#define INT_TIMER2_BITSf			(0x1 << 12)
#define INT_TIMER3_BITSf			(0x1 << 13)
#define INT_TIMER4_BITSf			(0x1 << 14)
#define INT_UART2_BITSf				(0x1 << 15)
#define INT_LCD_BITSf				(0x1 << 16)
#define INT_DMA0_BITSf				(0x1 << 17)
#define INT_DMA1_BITSf				(0x1 << 18)
#define INT_DMA2_BITSf				(0x1 << 19)
#define INT_DMA3_BITSf				(0x1 << 20)
#define INT_SDI_BITSf				(0x1 << 21)
#define INT_SPI0_BITSf				(0x1 << 22)
#define INT_UART1_BITSf				(0x1 << 23)
#define INT_NFCON_BITSf				(0x1 << 24)
#define INT_USBD_BITSf				(0x1 << 25)
#define INT_USBH_BITSf				(0x1 << 26)
#define INT_IIC_BITSf				(0x1 << 27)
#define INT_UART0_BITSf				(0x1 << 28)
#define INT_SPI1_BITSf				(0x1 << 29)
#define INT_RTC_BITSf				(0x1 << 30)
#define INT_ADC_BITSf				(0x1 << 31)

#define EINT0_START_BIT					(0)
#define EINT1_START_BIT					(1)
#define EINT2_START_BIT					(2)
#define EINT3_START_BIT					(3)
#define EINT4_7_START_BIT				(4)
#define EINT8_23_START_BIT				(5)
#define INT_CAM_START_BIT				(6)
#define INT_nBATT_FLT_START_BIT			(7)
#define INT_TICK_START_BIT				(8)
#define INT_WDT_AC97_START_BIT			(9)
#define INT_TIMER0_START_BIT			(10)
#define INT_TIMER1_START_BIT			(11)
#define INT_TIMER2_START_BIT			(12)
#define INT_TIMER3_START_BIT			(13)
#define INT_TIMER4_START_BIT			(14)
#define INT_UART2_START_BIT				(15)
#define INT_LCD_START_BIT				(16)
#define INT_DMA0_START_BIT				(17)
#define INT_DMA1_START_BIT				(18)
#define INT_DMA2_START_BIT				(19)
#define INT_DMA3_START_BIT				(20)
#define INT_SDI_START_BIT				(21)
#define INT_SPI0_START_BIT				(22)
#define INT_UART1_START_BIT				(23)
#define INT_NFCON_START_BIT				(24)
#define INT_USBD_START_BIT				(25)
#define INT_USBH_START_BIT				(26)
#define INT_IIC_START_BIT				(27)
#define INT_UART0_START_BIT				(28)
#define INT_SPI1_START_BIT				(29)
#define INT_RTC_START_BIT				(30)
#define INT_ADC_START_BIT				(31)

/* MASK设置，1表示屏蔽，0表示不屏蔽即使能中断 */
#define INTMASK_INT_ENABLE				(0)
#define INTMASK_INT_DISABLE				(1)

/* INOFFSETr相关值与含义，值表示INTPNDr对应bit为1，INTPND同一时刻最多一个bit为1 */
#define INTOFFSET_EINT0v				(0)
#define INTOFFSET_EINT1v				(1)
#define INTOFFSET_EINT2v				(2)
#define INTOFFSET_EINT3v				(3)
#define INTOFFSET_EINT4_7v			(4)
#define INTOFFSET_EINT8_23v			(5)
#define INTOFFSET_INT_CAMv			(6)
#define INTOFFSET_INT_nBATT_FLTv	(7)
#define INTOFFSET_INT_TICKv			(8)
#define INTOFFSET_INT_WDT_AC97v		(9)
#define INTOFFSET_INT_TIMER0v		(10)
#define INTOFFSET_INT_TIMER1v		(11)
#define INTOFFSET_INT_TIMER2v		(12)
#define INTOFFSET_INT_TIMER3v		(13)
#define INTOFFSET_INT_TIMER4v		(14)
#define INTOFFSET_INT_UART2v		(15)
#define INTOFFSET_INT_LCDv			(16)
#define INTOFFSET_INT_DMA0v			(17)
#define INTOFFSET_INT_DMA1v			(18)
#define INTOFFSET_INT_DMA2v			(19)
#define INTOFFSET_INT_DMA3v			(20)
#define INTOFFSET_INT_SDIv			(21)
#define INTOFFSET_INT_SPI0v			(22)
#define INTOFFSET_INT_UART1v		(23)
#define INTOFFSET_INT_NFCONv		(24)
#define INTOFFSET_INT_USBDv			(25)
#define INTOFFSET_INT_USBHv			(26)
#define INTOFFSET_INT_IICv			(27)
#define INTOFFSET_INT_UART0v		(28)
#define INTOFFSET_INT_SPI1v			(29)
#define INTOFFSET_INT_RTCv			(30)
#define INTOFFSET_INT_ADCv			(31)


/*
 * 由于触摸屏中断和ADC转换完成中断适用同一个中断，
 * 所以有SUBSRCPND寄存器来区分，产生的具体是什么中断
 */
#define SUBSRCPND_INT_AC97_BIT 		(14) 
#define SUBSRCPND_INT_WDT_BIT 		(13)  
#define SUBSRCPND_INT_CAM_P_BIT 	(12)
#define SUBSRCPND_INT_CAM_C_BIT 	(11)
#define SUBSRCPND_INT_ADC_S_BIT 	(10)
#define SUBSRCPND_INT_TC_BIT 		(9)   
#define SUBSRCPND_INT_ERR2_BIT 		(8)  
#define SUBSRCPND_INT_TXD2_BIT 		(7)  
#define SUBSRCPND_INT_RXD2_BIT 		(6)  
#define SUBSRCPND_INT_ERR1_BIT 		(5] 
#define SUBSRCPND_INT_TXD1_BIT 		(4) 
#define SUBSRCPND_INT_RXD1_BIT 		(3) 
#define SUBSRCPND_INT_ERR0_BIT 		(2) 
#define SUBSRCPND_INT_TXD0_BIT 		(1) 
#define SUBSRCPND_INT_RXD0_BIT 		(0) 

#endif /* _SCO_S3C2440_INTERRUPT_FIELD_H_ */

