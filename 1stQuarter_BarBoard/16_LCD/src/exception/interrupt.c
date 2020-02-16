/*
 * FileName：interrupt.c
 * Description:用于控制中断处理
 * Time：2020年2月8日18:33:55
 * Author：Kangruojin
 * Version：V1.0
*/

#include <interrupt.h>
#include <exception.h>
#include <tools.h>
#include <log.h>
#include <soc_s3c2440_public.h>

#define INTERRUPT_TYPE_ENUM_STR(intName)	#intName
static char * gInterruptTypeStr[interrupt_type_MAX + 1] = {
	/* 外部中断 */
	INTERRUPT_TYPE_ENUM_STR(EXT_INT0),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT1),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT2),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT3),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT4),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT5),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT6),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT7),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT8),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT9),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT10),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT11),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT12),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT13),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT14),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT15),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT16),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT17),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT18),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT19),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT20),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT21),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT22),
	INTERRUPT_TYPE_ENUM_STR(EXT_INT23),
	
	/* 内部中断 */
	INTERRUPT_TYPE_ENUM_STR(INT_CAM),
	INTERRUPT_TYPE_ENUM_STR(INT_nBATT_FLT),
	INTERRUPT_TYPE_ENUM_STR(INT_TICK),
	INTERRUPT_TYPE_ENUM_STR(INT_WDT_AC97),
	INTERRUPT_TYPE_ENUM_STR(INT_TIMER0),
	INTERRUPT_TYPE_ENUM_STR(INT_TIMER1),
	INTERRUPT_TYPE_ENUM_STR(INT_TIMER2),
	INTERRUPT_TYPE_ENUM_STR(INT_TIMER3),
	INTERRUPT_TYPE_ENUM_STR(INT_TIMER4),
	INTERRUPT_TYPE_ENUM_STR(INT_UART2),
	INTERRUPT_TYPE_ENUM_STR(INT_LCD),
	INTERRUPT_TYPE_ENUM_STR(INT_DMA0),
	INTERRUPT_TYPE_ENUM_STR(INT_DMA1),
	INTERRUPT_TYPE_ENUM_STR(INT_DMA2),
	INTERRUPT_TYPE_ENUM_STR(INT_DMA3),
	INTERRUPT_TYPE_ENUM_STR(INT_SDI),
	INTERRUPT_TYPE_ENUM_STR(INT_SPI0),
	INTERRUPT_TYPE_ENUM_STR(INT_UART1),
	INTERRUPT_TYPE_ENUM_STR(INT_NFCON),
	INTERRUPT_TYPE_ENUM_STR(INT_USBD),
	INTERRUPT_TYPE_ENUM_STR(INT_USBH),
	INTERRUPT_TYPE_ENUM_STR(INT_IIC),
	INTERRUPT_TYPE_ENUM_STR(INT_UART0),
	INTERRUPT_TYPE_ENUM_STR(INT_SPI1),
	INTERRUPT_TYPE_ENUM_STR(INT_RTC),
	INTERRUPT_TYPE_ENUM_STR(INT_ADC),

	/* last please */
	INTERRUPT_TYPE_ENUM_STR(MAX)
};

/* 中断测试驱动注册函数数组 */
static interrupt_handle_hook gInterruptHandleDrv[interrupt_type_MAX] = {NULL};
void * interrupt_handle(interrupt_type_t type, void * pArg)
{
	if (NULL != gInterruptHandleDrv[type])
	{
		return gInterruptHandleDrv[type](pArg);
	}
	else 
	{
		print_screen("\r\nInvalid function for interrupt %s[%d]", 
			gInterruptTypeStr[type], type);

		return NULL;
	}
}

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

interrupt_type_t interrupt_status_eintpend_get(void)
{
	uint32 eintPend = 0x0;
	interrupt_type_t occurInterrupt = interrupt_type_MAX;
	eintPend = EINTPENDr;
	
	switch (eintPend) {			
		case EINTPEND_EINT4v:	
			occurInterrupt = interrupt_type_EXT_INT4;
			break;
		case EINTPEND_EINT5v:	
			occurInterrupt = interrupt_type_EXT_INT5;
			break;
		case EINTPEND_EINT6v:	
			occurInterrupt = interrupt_type_EXT_INT6;
			break;
		case EINTPEND_EINT7v:	
			occurInterrupt = interrupt_type_EXT_INT7;
			break;
		case EINTPEND_EINT8v:	
			occurInterrupt = interrupt_type_EXT_INT8;
			break;
		case EINTPEND_EINT9v:	
			occurInterrupt = interrupt_type_EXT_INT9;
			break;
		case EINTPEND_EINT10v:	
			occurInterrupt = interrupt_type_EXT_INT10;
			break;
		case EINTPEND_EINT11v:	
			occurInterrupt = interrupt_type_EXT_INT11;
			break;
		case EINTPEND_EINT12v:	
			occurInterrupt = interrupt_type_EXT_INT12;
			break;
		case EINTPEND_EINT13v:	
			occurInterrupt = interrupt_type_EXT_INT13;
			break;
		case EINTPEND_EINT14v:	
			occurInterrupt = interrupt_type_EXT_INT14;
			break;
		case EINTPEND_EINT15v:	
			occurInterrupt = interrupt_type_EXT_INT15;
			break;
		case EINTPEND_EINT16v:	
			occurInterrupt = interrupt_type_EXT_INT16;
			break;
		case EINTPEND_EINT17v:	
			occurInterrupt = interrupt_type_EXT_INT17;
			break;
		case EINTPEND_EINT18v:	
			occurInterrupt = interrupt_type_EXT_INT18;
			break;
		case EINTPEND_EINT19v:	
			occurInterrupt = interrupt_type_EXT_INT19;
			break;
		case EINTPEND_EINT20v:	
			occurInterrupt = interrupt_type_EXT_INT20;
			break;
		case EINTPEND_EINT21v:	
			occurInterrupt = interrupt_type_EXT_INT21;
			break;
		case EINTPEND_EINT22v:	
			occurInterrupt = interrupt_type_EXT_INT22;
			break;
		case EINTPEND_EINT23v:	
			occurInterrupt = interrupt_type_EXT_INT23;
			break;
		default:
			occurInterrupt = interrupt_type_MAX;
			break;
	}

	return occurInterrupt;
}

/* 中断状态获取，返回指定类型中断是否产生，以及实际产生的中断类型 */
BOOL interrupt_status_get
(
	interrupt_type_t type, 			/* 判断的中断类型 */
	interrupt_type_t * real			/* 当前CPU处理的实际中断 */
)
{
	volatile uint32 offset = 0x0;
	interrupt_type_t occurInterrupt = interrupt_type_MAX;
	
	offset = INTOFFSETr;
	
	switch (offset) {
		case INTOFFSET_EINT0v:	
			occurInterrupt =interrupt_type_EXT_INT0;
			break;
		case INTOFFSET_EINT1v:	
			occurInterrupt =interrupt_type_EXT_INT1;
			break;
		case INTOFFSET_EINT2v:	
			occurInterrupt =interrupt_type_EXT_INT2;
			break;
		case INTOFFSET_EINT3v:	
			occurInterrupt =interrupt_type_EXT_INT3;
			break;
		case INTOFFSET_EINT4_7v:	
		case INTOFFSET_EINT8_23v:	
			occurInterrupt = interrupt_status_eintpend_get();
			break;
		case INTOFFSET_INT_CAMv:	
			occurInterrupt =interrupt_type_INT_CAM;
			break;
		case INTOFFSET_INT_nBATT_FLTv:	
			occurInterrupt =interrupt_type_INT_nBATT_FLT;
			break;
		case INTOFFSET_INT_TICKv:	
			occurInterrupt =interrupt_type_INT_TICK;
			break;
		case INTOFFSET_INT_WDT_AC97v:	
			occurInterrupt =interrupt_type_INT_WDT_AC97;
			break;
		case INTOFFSET_INT_TIMER0v:	
			occurInterrupt =interrupt_type_INT_TIMER0;
			break;
		case INTOFFSET_INT_TIMER1v:	
			occurInterrupt =interrupt_type_INT_TIMER1;
			break;
		case INTOFFSET_INT_TIMER2v:	
			occurInterrupt =interrupt_type_INT_TIMER2;
			break;
		case INTOFFSET_INT_TIMER3v:	
			occurInterrupt =interrupt_type_INT_TIMER3;
			break;
		case INTOFFSET_INT_TIMER4v:	
			occurInterrupt =interrupt_type_INT_TIMER4;
			break;
		case INTOFFSET_INT_UART2v:	
			occurInterrupt =interrupt_type_INT_UART2;
			break;
		case INTOFFSET_INT_LCDv:	
			occurInterrupt =interrupt_type_INT_LCD;
			break;
		case INTOFFSET_INT_DMA0v:	
			occurInterrupt =interrupt_type_INT_DMA0;
			break;
		case INTOFFSET_INT_DMA1v:	
			occurInterrupt =interrupt_type_INT_DMA1;
			break;
		case INTOFFSET_INT_DMA2v:	
			occurInterrupt =interrupt_type_INT_DMA2;
			break;
		case INTOFFSET_INT_DMA3v:	
			occurInterrupt =interrupt_type_INT_DMA3;
			break;
		case INTOFFSET_INT_SDIv:	
			occurInterrupt =interrupt_type_INT_SDI;
			break;
		case INTOFFSET_INT_SPI0v:	
			occurInterrupt =interrupt_type_INT_SPI0;
			break;
		case INTOFFSET_INT_UART1v:	
			occurInterrupt =interrupt_type_INT_UART1;
			break;
		case INTOFFSET_INT_NFCONv:	
			occurInterrupt =interrupt_type_INT_NFCON;
			break;
		case INTOFFSET_INT_USBDv:	
			occurInterrupt =interrupt_type_INT_USBD;
			break;
		case INTOFFSET_INT_USBHv:	
			occurInterrupt =interrupt_type_INT_USBH;
			break;
		case INTOFFSET_INT_IICv:	
			occurInterrupt =interrupt_type_INT_IIC;
			break;
		case INTOFFSET_INT_UART0v:	
			occurInterrupt =interrupt_type_INT_UART0;
			break;
		case INTOFFSET_INT_SPI1v:	
			occurInterrupt =interrupt_type_INT_SPI1;
			break;
		case INTOFFSET_INT_RTCv:	
			occurInterrupt =interrupt_type_INT_RTC;
			break;
		case INTOFFSET_INT_ADCv:	
			occurInterrupt =interrupt_type_INT_ADC;
			break;
		default:	
			occurInterrupt =interrupt_type_MAX;
			break;
	}

	*real = occurInterrupt;
	
	/* 打印中断类型等信息 */
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "interrupt type:%s[%d]", 
		gInterruptTypeStr[occurInterrupt], occurInterrupt);

	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " INTMASKr  :[%x]", INTMASKr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " EINTMASKr :[%x]", EINTMASKr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " SRCPNDr   :[%x]", SRCPNDr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " INTPNDr   :[%x]", INTPNDr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " EINTPENDr :[%x]", EINTPENDr);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " INTOFFSETr:[%x]", INTOFFSETr);

	/* 产生的中断和查询的中断一致，则返回TRUE */	
	return ((type == occurInterrupt) ? TRUE : FALSE);
}

/* 
 * 中断状态清除,写1为clear,由于request时bit=1为产生中断,
 * 那么把原来的值写回去就相当于clear相关中断了
 * 注意:clear顺序为:EINTPENDr -> SRCPNDr -> INTPNDr
 * 虽然INTPND已经clear掉，如源头未clear，导致又一次向中断控制器request
 */
void interrupt_status_clear(interrupt_type_t type)
{
	uint32 srcpnd = SRCPNDr;
	uint32 intpnd = INTPNDr;
	uint32 eintpend = EINTPENDr;
	
#if 0
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL,
		"clear start SRCPNDr-INTPNDr-EINTPENDr:%x-%x-%x", 
		SRCPNDr, INTPNDr, EINTPENDr);
#endif

	EINTPENDr = eintpend;
	SRCPNDr = srcpnd;
	INTPNDr = intpnd;

#if 0
	/* 清除中断后，在下一次中断产生前，相关寄存器都会变为0 */
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL,
		"clear over SRCPNDr-INTPNDr-EINTPENDr:%x-%x-%x", 
		SRCPNDr, INTPNDr, EINTPENDr);
#endif
}

static void interrupt_controller_enable_set
(
	interrupt_type_t type, 
	BOOL enable
)
{
	uint32 setValue = (enable) ? (INTMASK_INT_ENABLE) : (INTMASK_INT_DISABLE);

	switch (type) {
		case interrupt_type_EXT_INT0:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, EINT0_START_BIT, setValue);
			break;
		case interrupt_type_EXT_INT1:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, EINT1_START_BIT, setValue);
			break;
		case interrupt_type_EXT_INT2:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, EINT2_START_BIT, setValue);
			break;
		case interrupt_type_EXT_INT3:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, EINT3_START_BIT, setValue);
			break;
		case interrupt_type_EXT_INT4:
		case interrupt_type_EXT_INT5:
		case interrupt_type_EXT_INT6:
		case interrupt_type_EXT_INT7:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, EINT4_7_START_BIT, setValue);
			break;
		case interrupt_type_EXT_INT8:
		case interrupt_type_EXT_INT9:
		case interrupt_type_EXT_INT10:
		case interrupt_type_EXT_INT11:
		case interrupt_type_EXT_INT12:
		case interrupt_type_EXT_INT13:
		case interrupt_type_EXT_INT14:
		case interrupt_type_EXT_INT15:
		case interrupt_type_EXT_INT16:
		case interrupt_type_EXT_INT17:
		case interrupt_type_EXT_INT18:
		case interrupt_type_EXT_INT19:
		case interrupt_type_EXT_INT20:
		case interrupt_type_EXT_INT21:
		case interrupt_type_EXT_INT22:
		case interrupt_type_EXT_INT23:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, EINT8_23_START_BIT, setValue);
			break;
		case interrupt_type_INT_CAM:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_CAM_START_BIT, setValue);
			break;
		case interrupt_type_INT_nBATT_FLT:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_nBATT_FLT_START_BIT, setValue);
			break;
		case interrupt_type_INT_TICK:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_TICK_START_BIT, setValue);
			break;
		case interrupt_type_INT_WDT_AC97:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_WDT_AC97_START_BIT, setValue);
			break;
		case interrupt_type_INT_TIMER0:
			SOC_S3C2440_REG_BIT_SET(INTMASKr,INT_TIMER0_START_BIT, setValue);
			break;
		case interrupt_type_INT_TIMER1:
			SOC_S3C2440_REG_BIT_SET(INTMASKr,INT_TIMER1_START_BIT, setValue);
			break;
		case interrupt_type_INT_TIMER2:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_TIMER2_START_BIT, setValue);
			break;
		case interrupt_type_INT_TIMER3:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_TIMER3_START_BIT, setValue);
			break;
		case interrupt_type_INT_TIMER4:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_TIMER4_START_BIT, setValue);
			break;
		case interrupt_type_INT_UART2:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_UART2_START_BIT, setValue);
			break;
		case interrupt_type_INT_LCD:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_LCD_START_BIT, setValue);
			break;
		case interrupt_type_INT_DMA0:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_DMA0_START_BIT, setValue);
			break;
		case interrupt_type_INT_DMA1:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_DMA1_START_BIT, setValue);
			break;
		case interrupt_type_INT_DMA2:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_DMA2_START_BIT, setValue);
			break;
		case interrupt_type_INT_DMA3:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_DMA3_START_BIT, setValue);
			break;
		case interrupt_type_INT_SDI:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_SDI_START_BIT, setValue);
			break;
		case interrupt_type_INT_SPI0:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_SPI0_START_BIT, setValue);
			break;
		case interrupt_type_INT_UART1:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_UART1_START_BIT, setValue);
			break;
		case interrupt_type_INT_NFCON:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_NFCON_START_BIT, setValue);
			break;
		case interrupt_type_INT_USBD:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_USBD_START_BIT, setValue);
			break;
		case interrupt_type_INT_USBH:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_USBH_START_BIT, setValue);
			break;
		case interrupt_type_INT_IIC:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_IIC_START_BIT, setValue);
			break;
		case interrupt_type_INT_UART0:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_UART0_START_BIT, setValue);
			break;
		case interrupt_type_INT_SPI1:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_SPI1_START_BIT, setValue);
			break;
		case interrupt_type_INT_RTC:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_RTC_START_BIT, setValue);
			break;
		case interrupt_type_INT_ADC:
			SOC_S3C2440_REG_BIT_SET(INTMASKr, INT_ADC_START_BIT, setValue);
			break;
		default:
			print_screen("\r\n Invalid interrupt type:%s[%d]", gInterruptTypeStr[type], type);
			break;
	}
}

/* 开启中断并注册回调函数 */
BOOL interrupt_register
(
	interrupt_type_t type, 
	interrupt_handle_hook interruptHandle
)
{
	if (type > interrupt_type_MAX || type < interrupt_type_EXT_INT0)
	{
		print_screen("\r\nInvalid interrupt type %d", type);
		return FALSE;
	}
	
	interrupt_controller_enable_set(type, TRUE);
	gInterruptHandleDrv[type] = interruptHandle;

	return TRUE;
}

/* 关闭中断，并清除中断注册回调函数 */
BOOL interrupt_unregister(interrupt_type_t type)
{
	if (type > interrupt_type_MAX || type < interrupt_type_EXT_INT0)
	{
		print_screen("\r\nInvalid interrupt type %d", type);
		return FALSE;
	}
	
	interrupt_controller_enable_set(type, FALSE);
	gInterruptHandleDrv[type] = NULL;
	
	return TRUE;
}

/* IRQ异常中断识别处理入口 */
void * interrupt_irq_deal_start(void * pArgv)
{
	interrupt_type_t intType;
	void * intteruptRet = NULL;
	
	/* 分辨中断源 */
	(void)interrupt_status_get(interrupt_type_MAX, &intType);

	if (interrupt_type_MAX > intType)
	{
		/* 根据中断源调用对应处理函数 */
		intteruptRet = interrupt_handle(intType, pArgv);
	}

	/* 清除中断 */
	(void)interrupt_status_clear(intType);

	return intteruptRet;
}

