/*
 * FileName：interrupt.c
 * Description:用于控制中断处理
 * Time：2020年2月8日18:33:55
 * Author：Kangruojin
 * Version：V1.0
*/

#include <interrupt.h>
#include <tools.h>
#include <log.h>
#include <soc_s3c2440_public.h>

#define INTERRUPT_TYPE_ENUM_STR(intName)	#intName
#define INTERRUPT_HANLE_FUNCTION_NULL_RETURN(intType) \
	do { \
		print_screen("\r\nNo have deal function for interrupt %s[%d]", \
			gInterruptTypeStr[intType], intType); \
		return -1; \
	} while(0)

static char * gInterruptTypeStr[INTERRUPT_TYPE(MAX) + 1] = {
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

/* 中断测试驱动 */
extern interrupt_drv_t gSocTest_InterruptHandleDrv;

static interrupt_drv_t * gInterruptHandleDrv[] = {
	&gSocTest_InterruptHandleDrv,	/* interrupt_drv_type_test_soc_s3c2440 */
	NULL							/* interrupt_drv_type_max */
};

static interrupt_drv_type_t interrupt_get_drv_type(void)
{
	return interrupt_drv_type_test_soc_s3c2440;
}

BOOL interrupt_drv_type_valid(interrupt_drv_type_t type)
{
	int drvNum = sizeof(gInterruptHandleDrv) / sizeof(interrupt_drv_t *);
	
	if ((type < drvNum) && (NULL != gInterruptHandleDrv[type]))
	{
		return TRUE;
	}
	else 
	{
		print_screen("\r\nInvalid type %d", type);
		return FALSE;
	}
}

/***************************************** 驱动函数Start *****************************************/
void interrupt_init(void)
{
	interrupt_drv_type_t __type__ = interrupt_get_drv_type();
	
	if (interrupt_drv_type_valid(__type__)
		&& (NULL != gInterruptHandleDrv[__type__]->interrupt_init))
	{
		gInterruptHandleDrv[__type__]->interrupt_init();
		print_screen("\r\n interruptinit succeed!!");
	}
	else 
	{
		print_screen("\r\nInvalid function:%s-%d", __FUNCTION__, __LINE__);
	}
}

int interrupt_handle_ext_int0(void)
{
	interrupt_drv_type_t __type__ = interrupt_get_drv_type();
	
	if (interrupt_drv_type_valid(__type__)
		&& (NULL != gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT0))
	{
		gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT0();
	}
	else 
	{
		print_screen("\r\nInvalid function:%s-%d", __FUNCTION__, __LINE__);
	}
}

int  interrupt_handle_ext_int2(void)
{
	interrupt_drv_type_t __type__ = interrupt_get_drv_type();
	
	if (interrupt_drv_type_valid(__type__)
		&& (NULL != gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT2))
	{
		gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT2();
	}	
	else 
	{
		print_screen("\r\nInvalid function:%s-%d", __FUNCTION__, __LINE__);
	}
}

int  interrupt_handle_ext_int4(void)
{
	interrupt_drv_type_t __type__ = interrupt_get_drv_type();
	
	if (interrupt_drv_type_valid(__type__)
		&& (NULL != gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT4))
	{
		gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT4();
	}	
	else
	{
		print_screen("\r\nInvalid function:%s-%d", __FUNCTION__, __LINE__);
	}
}

int  interrupt_handle_ext_int11(void)
{
	interrupt_drv_type_t __type__ = interrupt_get_drv_type();
	
	if (interrupt_drv_type_valid(__type__)
		&& (NULL != gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT11))
	{
		gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT11();
	}
	else
	{
		print_screen("\r\nInvalid function:%s-%d", __FUNCTION__, __LINE__);
	}
}

int  interrupt_handle_ext_int19(void)
{
	interrupt_drv_type_t __type__ = interrupt_get_drv_type();
	
	if (interrupt_drv_type_valid(__type__)
		&& (NULL != gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT19))
	{
		gInterruptHandleDrv[__type__]->interrupt_handle_EXT_INT19();
	}	
	else
	{
		print_screen("\r\nInvalid function:%s-%d", __FUNCTION__, __LINE__);
	}
}
/***************************************** 驱动函数End *****************************************/

interrupt_type_t interrupt_status_eintpend_get(void)
{
	uint32 eintPend = 0x0;
	interrupt_type_t occurInterrupt = INTERRUPT_TYPE(MAX);
	eintPend = EINTPENDr;
	
	switch (eintPend) {			
		case EINTPEND_EINT4v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT4);
			break;
		case EINTPEND_EINT5v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT5);
			break;
		case EINTPEND_EINT6v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT6);
			break;
		case EINTPEND_EINT7v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT7);
			break;
		case EINTPEND_EINT8v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT8);
			break;
		case EINTPEND_EINT9v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT9);
			break;
		case EINTPEND_EINT10v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT10);
			break;
		case EINTPEND_EINT11v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT11);
			break;
		case EINTPEND_EINT12v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT12);
			break;
		case EINTPEND_EINT13v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT13);
			break;
		case EINTPEND_EINT14v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT14);
			break;
		case EINTPEND_EINT15v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT15);
			break;
		case EINTPEND_EINT16v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT16);
			break;
		case EINTPEND_EINT17v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT17);
			break;
		case EINTPEND_EINT18v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT18);
			break;
		case EINTPEND_EINT19v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT19);
			break;
		case EINTPEND_EINT20v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT20);
			break;
		case EINTPEND_EINT21v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT21);
			break;
		case EINTPEND_EINT22v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT22);
			break;
		case EINTPEND_EINT23v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT23);
			break;
		default:
			occurInterrupt = INTERRUPT_TYPE(MAX);
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
	interrupt_type_t occurInterrupt = INTERRUPT_TYPE(MAX);
	
	offset = INTOFFSETr;
	
	switch (offset) {
		case INTOFFSET_EINT0v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT0);
			break;
		case INTOFFSET_EINT1v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT1);
			break;
		case INTOFFSET_EINT2v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT2);
			break;
		case INTOFFSET_EINT3v:	
			occurInterrupt = INTERRUPT_TYPE(EXT_INT3);
			break;
		case INTOFFSET_EINT4_7v:	
		case INTOFFSET_EINT8_23v:	
			occurInterrupt = interrupt_status_eintpend_get();
			break;
		case INTOFFSET_INT_CAMv:	
			occurInterrupt = INTERRUPT_TYPE(INT_CAM);
			break;
		case INTOFFSET_INT_nBATT_FLTv:	
			occurInterrupt = INTERRUPT_TYPE(INT_nBATT_FLT);
			break;
		case INTOFFSET_INT_TICKv:	
			occurInterrupt = INTERRUPT_TYPE(INT_TICK);
			break;
		case INTOFFSET_INT_WDT_AC97v:	
			occurInterrupt = INTERRUPT_TYPE(INT_WDT_AC97);
			break;
		case INTOFFSET_INT_TIMER0v:	
			occurInterrupt = INTERRUPT_TYPE(INT_TIMER0);
			break;
		case INTOFFSET_INT_TIMER1v:	
			occurInterrupt = INTERRUPT_TYPE(INT_TIMER1);
			break;
		case INTOFFSET_INT_TIMER2v:	
			occurInterrupt = INTERRUPT_TYPE(INT_TIMER2);
			break;
		case INTOFFSET_INT_TIMER3v:	
			occurInterrupt = INTERRUPT_TYPE(INT_TIMER3);
			break;
		case INTOFFSET_INT_TIMER4v:	
			occurInterrupt = INTERRUPT_TYPE(INT_TIMER4);
			break;
		case INTOFFSET_INT_UART2v:	
			occurInterrupt = INTERRUPT_TYPE(INT_UART2);
			break;
		case INTOFFSET_INT_LCDv:	
			occurInterrupt = INTERRUPT_TYPE(INT_LCD);
			break;
		case INTOFFSET_INT_DMA0v:	
			occurInterrupt = INTERRUPT_TYPE(INT_DMA0);
			break;
		case INTOFFSET_INT_DMA1v:	
			occurInterrupt = INTERRUPT_TYPE(INT_DMA1);
			break;
		case INTOFFSET_INT_DMA2v:	
			occurInterrupt = INTERRUPT_TYPE(INT_DMA2);
			break;
		case INTOFFSET_INT_DMA3v:	
			occurInterrupt = INTERRUPT_TYPE(INT_DMA3);
			break;
		case INTOFFSET_INT_SDIv:	
			occurInterrupt = INTERRUPT_TYPE(INT_SDI);
			break;
		case INTOFFSET_INT_SPI0v:	
			occurInterrupt = INTERRUPT_TYPE(INT_SPI0);
			break;
		case INTOFFSET_INT_UART1v:	
			occurInterrupt = INTERRUPT_TYPE(INT_UART1);
			break;
		case INTOFFSET_INT_NFCONv:	
			occurInterrupt = INTERRUPT_TYPE(INT_NFCON);
			break;
		case INTOFFSET_INT_USBDv:	
			occurInterrupt = INTERRUPT_TYPE(INT_USBD);
			break;
		case INTOFFSET_INT_USBHv:	
			occurInterrupt = INTERRUPT_TYPE(INT_USBH);
			break;
		case INTOFFSET_INT_IICv:	
			occurInterrupt = INTERRUPT_TYPE(INT_IIC);
			break;
		case INTOFFSET_INT_UART0v:	
			occurInterrupt = INTERRUPT_TYPE(INT_UART0);
			break;
		case INTOFFSET_INT_SPI1v:	
			occurInterrupt = INTERRUPT_TYPE(INT_SPI1);
			break;
		case INTOFFSET_INT_RTCv:	
			occurInterrupt = INTERRUPT_TYPE(INT_RTC);
			break;
		case INTOFFSET_INT_ADCv:	
			occurInterrupt = INTERRUPT_TYPE(INT_ADC);
			break;
		default:	
			occurInterrupt = INTERRUPT_TYPE(MAX);
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

int  interrupt_handle(interrupt_type_t type)
{
	switch (type) {
		case INTERRUPT_TYPE(EXT_INT0):
			return interrupt_handle_ext_int0();
	
		case INTERRUPT_TYPE(EXT_INT1):
			INTERRUPT_HANLE_FUNCTION_NULL_RETURN(type);
			
		case INTERRUPT_TYPE(EXT_INT2):
			return interrupt_handle_ext_int2();
			
		case INTERRUPT_TYPE(EXT_INT3):
			INTERRUPT_HANLE_FUNCTION_NULL_RETURN(type);
			
		case INTERRUPT_TYPE(EXT_INT4):
			return interrupt_handle_ext_int4();
			
		case INTERRUPT_TYPE(EXT_INT5):
		case INTERRUPT_TYPE(EXT_INT6):
		case INTERRUPT_TYPE(EXT_INT7):
		case INTERRUPT_TYPE(EXT_INT8):
		case INTERRUPT_TYPE(EXT_INT9):
		case INTERRUPT_TYPE(EXT_INT10):
			INTERRUPT_HANLE_FUNCTION_NULL_RETURN(type);
		
		case INTERRUPT_TYPE(EXT_INT11):
			return interrupt_handle_ext_int11();
			
		case INTERRUPT_TYPE(EXT_INT12):
		case INTERRUPT_TYPE(EXT_INT13):
		case INTERRUPT_TYPE(EXT_INT14):
		case INTERRUPT_TYPE(EXT_INT15):
		case INTERRUPT_TYPE(EXT_INT16):
		case INTERRUPT_TYPE(EXT_INT17):
		case INTERRUPT_TYPE(EXT_INT18):
			INTERRUPT_HANLE_FUNCTION_NULL_RETURN(type);

		case INTERRUPT_TYPE(EXT_INT19):
			return interrupt_handle_ext_int19();
			
		case INTERRUPT_TYPE(EXT_INT20):
		case INTERRUPT_TYPE(EXT_INT21):
		case INTERRUPT_TYPE(EXT_INT22):
		case INTERRUPT_TYPE(EXT_INT23):
		case INTERRUPT_TYPE(INT_CAM):
		case INTERRUPT_TYPE(INT_nBATT_FLT):
		case INTERRUPT_TYPE(INT_TICK):
		case INTERRUPT_TYPE(INT_WDT_AC97):
		case INTERRUPT_TYPE(INT_TIMER0):
		case INTERRUPT_TYPE(INT_TIMER1):
		case INTERRUPT_TYPE(INT_TIMER2):
		case INTERRUPT_TYPE(INT_TIMER3):
		case INTERRUPT_TYPE(INT_TIMER4):
		case INTERRUPT_TYPE(INT_UART2):
		case INTERRUPT_TYPE(INT_LCD):
		case INTERRUPT_TYPE(INT_DMA0):
		case INTERRUPT_TYPE(INT_DMA1):
		case INTERRUPT_TYPE(INT_DMA2):
		case INTERRUPT_TYPE(INT_DMA3):
		case INTERRUPT_TYPE(INT_SDI):
		case INTERRUPT_TYPE(INT_SPI0):
		case INTERRUPT_TYPE(INT_UART1):
		case INTERRUPT_TYPE(INT_NFCON):
		case INTERRUPT_TYPE(INT_USBD):
		case INTERRUPT_TYPE(INT_USBH):
		case INTERRUPT_TYPE(INT_IIC):
		case INTERRUPT_TYPE(INT_UART0):
		case INTERRUPT_TYPE(INT_SPI1):
		case INTERRUPT_TYPE(INT_RTC):
		case INTERRUPT_TYPE(INT_ADC):
		default:
			INTERRUPT_HANLE_FUNCTION_NULL_RETURN(type);
	}
	
	return;
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
	
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL,
		"clear start SRCPNDr-INTPNDr-EINTPENDr:%x-%x-%x", 
		SRCPNDr, INTPNDr, EINTPENDr);
	
	EINTPENDr = eintpend;
	SRCPNDr = srcpnd;
	INTPNDr = intpnd;

	SYS_DEBUG_PRINT(SOC_DBG_NORMAL,
		"clear over SRCPNDr-INTPNDr-EINTPENDr:%x-%x-%x", 
		SRCPNDr, INTPNDr, EINTPENDr);
}

/* IRQ异常中断识别处理入口 */
void interrupt_irq_deal_start(void)
{
	interrupt_type_t intType;
	uint32 intValue = INTPNDr;

	/* 分辨中断源 */
	(void)interrupt_status_get(INTERRUPT_TYPE(MAX), &intType);

	if (INTERRUPT_TYPE(MAX) > intType)
	{
		/* 根据中断源调用对应处理函数 */
		(void)interrupt_handle(intType);
	}

	/* 清除中断 */
	(void)interrupt_status_clear(intType);
}

