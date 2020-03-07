#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <tools.h>

/* 按照手册INTOFFSETr的值顺序定义 */
typedef enum interrupt_type_e{
	/* 外部中断 */
	interrupt_type_EXT_INT0,
	interrupt_type_EXT_INT1,
	interrupt_type_EXT_INT2,
	interrupt_type_EXT_INT3,
	interrupt_type_EXT_INT4,
	interrupt_type_EXT_INT5,
	interrupt_type_EXT_INT6,
	interrupt_type_EXT_INT7,
	interrupt_type_EXT_INT8,
	interrupt_type_EXT_INT9,
	interrupt_type_EXT_INT10,
	interrupt_type_EXT_INT11,
	interrupt_type_EXT_INT12,
	interrupt_type_EXT_INT13,
	interrupt_type_EXT_INT14,
	interrupt_type_EXT_INT15,
	interrupt_type_EXT_INT16,
	interrupt_type_EXT_INT17,
	interrupt_type_EXT_INT18,
	interrupt_type_EXT_INT19,
	interrupt_type_EXT_INT20,
	interrupt_type_EXT_INT21,
	interrupt_type_EXT_INT22,
	interrupt_type_EXT_INT23,
	
	/* 内部中断 */
	interrupt_type_INT_CAM,
	interrupt_type_INT_nBATT_FLT,
	interrupt_type_INT_TICK,
	interrupt_type_INT_WDT_AC97,
	interrupt_type_INT_TIMER0,
	interrupt_type_INT_TIMER1,
	interrupt_type_INT_TIMER2,
	interrupt_type_INT_TIMER3,
	interrupt_type_INT_TIMER4,
	interrupt_type_INT_UART2,
	interrupt_type_INT_LCD,
	interrupt_type_INT_DMA0,
	interrupt_type_INT_DMA1,
	interrupt_type_INT_DMA2,
	interrupt_type_INT_DMA3,
	interrupt_type_INT_SDI,
	interrupt_type_INT_SPI0,
	interrupt_type_INT_UART1,
	interrupt_type_INT_NFCON,
	interrupt_type_INT_USBD,
	interrupt_type_INT_USBH,
	interrupt_type_INT_IIC,
	interrupt_type_INT_UART0,
	interrupt_type_INT_SPI1,
	interrupt_type_INT_RTC,
	interrupt_type_INT_ADC,
	interrupt_type_INT_TSC,
	/* last please */
	interrupt_type_MAX
}interrupt_type_t;

typedef void * (*interrupt_handle_hook)(void *);

void * interrupt_irq_deal_start(void * pArgv);

extern BOOL interrupt_register(interrupt_type_t type, interrupt_handle_hook interruptHandle);
extern BOOL interrupt_unregister(interrupt_type_t type);
extern void interrupt_sleep(char * modName);
extern void interrupt_wake_up(char * modName);

#endif /* _INTERRUPT_H_ */
