#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <tools.h>

#define INTERRUPT_TYPE(intName)		interrupt_type_##intName

/* 按照手册INTOFFSETr的值顺序定义 */
typedef enum interrupt_type_e{
	/* 外部中断 */
	INTERRUPT_TYPE(EXT_INT0),
	INTERRUPT_TYPE(EXT_INT1),
	INTERRUPT_TYPE(EXT_INT2),
	INTERRUPT_TYPE(EXT_INT3),
	INTERRUPT_TYPE(EXT_INT4),
	INTERRUPT_TYPE(EXT_INT5),
	INTERRUPT_TYPE(EXT_INT6),
	INTERRUPT_TYPE(EXT_INT7),
	INTERRUPT_TYPE(EXT_INT8),
	INTERRUPT_TYPE(EXT_INT9),
	INTERRUPT_TYPE(EXT_INT10),
	INTERRUPT_TYPE(EXT_INT11),
	INTERRUPT_TYPE(EXT_INT12),
	INTERRUPT_TYPE(EXT_INT13),
	INTERRUPT_TYPE(EXT_INT14),
	INTERRUPT_TYPE(EXT_INT15),
	INTERRUPT_TYPE(EXT_INT16),
	INTERRUPT_TYPE(EXT_INT17),
	INTERRUPT_TYPE(EXT_INT18),
	INTERRUPT_TYPE(EXT_INT19),
	INTERRUPT_TYPE(EXT_INT20),
	INTERRUPT_TYPE(EXT_INT21),
	INTERRUPT_TYPE(EXT_INT22),
	INTERRUPT_TYPE(EXT_INT23),
	
	/* 内部中断 */
	INTERRUPT_TYPE(INT_CAM),
	INTERRUPT_TYPE(INT_nBATT_FLT),
	INTERRUPT_TYPE(INT_TICK),
	INTERRUPT_TYPE(INT_WDT_AC97),
	INTERRUPT_TYPE(INT_TIMER0),
	INTERRUPT_TYPE(INT_TIMER1),
	INTERRUPT_TYPE(INT_TIMER2),
	INTERRUPT_TYPE(INT_TIMER3),
	INTERRUPT_TYPE(INT_TIMER4),
	INTERRUPT_TYPE(INT_UART2),
	INTERRUPT_TYPE(INT_LCD),
	INTERRUPT_TYPE(INT_DMA0),
	INTERRUPT_TYPE(INT_DMA1),
	INTERRUPT_TYPE(INT_DMA2),
	INTERRUPT_TYPE(INT_DMA3),
	INTERRUPT_TYPE(INT_SDI),
	INTERRUPT_TYPE(INT_SPI0),
	INTERRUPT_TYPE(INT_UART1),
	INTERRUPT_TYPE(INT_NFCON),
	INTERRUPT_TYPE(INT_USBD),
	INTERRUPT_TYPE(INT_USBH),
	INTERRUPT_TYPE(INT_IIC),
	INTERRUPT_TYPE(INT_UART0),
	INTERRUPT_TYPE(INT_SPI1),
	INTERRUPT_TYPE(INT_RTC),
	INTERRUPT_TYPE(INT_ADC),

	/* last please */
	INTERRUPT_TYPE(MAX)
}interrupt_type_t;

/* 中断处理函数指针结构体 */
typedef struct interrupt_drv_s {
	/* 中断驱动描述 */
	char * pDrvDesciption;	

	/* 中断初始化 */
	void (*interrupt_init)(void);

	/* 外部中断处理函数 */
	void (*interrupt_handle_EXT_INT0)(void);
	void (*interrupt_handle_EXT_INT1)(void);
	void (*interrupt_handle_EXT_INT2)(void);
	void (*interrupt_handle_EXT_INT3)(void);
	void (*interrupt_handle_EXT_INT4)(void);
	void (*interrupt_handle_EXT_INT5)(void);
	void (*interrupt_handle_EXT_INT6)(void);
	void (*interrupt_handle_EXT_INT7)(void);
	void (*interrupt_handle_EXT_INT8)(void);
	void (*interrupt_handle_EXT_INT9)(void);
	void (*interrupt_handle_EXT_INT10)(void);
	void (*interrupt_handle_EXT_INT11)(void);
	void (*interrupt_handle_EXT_INT12)(void);
	void (*interrupt_handle_EXT_INT13)(void);
	void (*interrupt_handle_EXT_INT14)(void);
	void (*interrupt_handle_EXT_INT15)(void);
	void (*interrupt_handle_EXT_INT16)(void);
	void (*interrupt_handle_EXT_INT17)(void);
	void (*interrupt_handle_EXT_INT18)(void);
	void (*interrupt_handle_EXT_INT19)(void);
	void (*interrupt_handle_EXT_INT20)(void);
	void (*interrupt_handle_EXT_INT21)(void);
	void (*interrupt_handle_EXT_INT22)(void);
	void (*interrupt_handle_EXT_INT23)(void);
	
	/* 内部中断处理函数 */
	void (*interrupt_handle_INT_CAM)(void);
	void (*interrupt_handle_INT_nBATT_FLT)(void);
	void (*interrupt_handle_INT_TICK)(void);
	void (*interrupt_handle_INT_WDT_AC97)(void);
	void (*interrupt_handle_INT_TIMER0)(void);
	void (*interrupt_handle_INT_TIMER1)(void);
	void (*interrupt_handle_INT_TIMER2)(void);
	void (*interrupt_handle_INT_TIMER3)(void);
	void (*interrupt_handle_INT_TIMER4)(void);
	void (*interrupt_handle_INT_UART2)(void);
	void (*interrupt_handle_INT_LCD)(void);
	void (*interrupt_handle_INT_DMA0)(void);
	void (*interrupt_handle_INT_DMA1)(void);
	void (*interrupt_handle_INT_DMA2)(void);
	void (*interrupt_handle_INT_DMA3)(void);
	void (*interrupt_handle_INT_SDI)(void);
	void (*interrupt_handle_INT_SPI0)(void);
	void (*interrupt_handle_INT_UART1)(void);
	void (*interrupt_handle_INT_NFCON)(void);
	void (*interrupt_handle_INT_USBD)(void);
	void (*interrupt_handle_INT_USBH)(void);
	void (*interrupt_handle_INT_IIC)(void);
	void (*interrupt_handle_INT_UART0)(void);
	void (*interrupt_handle_INT_SPI1)(void);
	void (*interrupt_handle_INT_RTC)(void);
	void (*interrupt_handle_INT_ADC)(void);

	/* last please */
	void (*interrupt_handle_MAX)(void);
}interrupt_drv_t;

#define INTERRUPT_DRV_TYPE_ENUM(type) interrupt_drv_type_##type

/* 中断驱动类型 */
typedef enum interrupt_drv_type_e {
	interrupt_drv_type_test_soc_s3c2440,
	interrupt_drv_type_max
}interrupt_drv_type_t;

extern void interrupt_init(void);
extern void interrupt_irq_deal_start(void);

#endif /* _INTERRUPT_H_ */
