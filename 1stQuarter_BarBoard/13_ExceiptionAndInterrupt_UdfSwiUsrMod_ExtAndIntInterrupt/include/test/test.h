#ifndef __TEST_H__
#define __TEST_H__

#include <tools.h>

typedef enum{
	TEST_LED,					/* LED测试用例 */
	TEST_UART,					/* UART测试用例 */
	TEST_LED_UART,				/* LED和UART复合测试用例 */
	TEST_SDRAM,					/* SDRAM与UART复合测试用例 */
	TEST_RELOCATION_LESS_THAN_4K,		/* 代码重定位测试用例:小于4K的重定位测试 */
	TEST_RELOCATION_GREATER_THAN_4K,	/* 代码重定位测试用例:大于4K的重定位测试 */
	TEST_THUMB_INSTRUCTION,				/* THUMB指令集测试 */
	TEST_UDF_INSTRUCTION_EXCEPTION,		/* 未定义指令异常测试 */
	TEST_SWI_EXCEPTION, 				/* 软件中断异常测试，一般为usr模式切换到软件中断异常模式 */
	TEST_EXT_KEY_INTERRUPT,				/* 外部中断测试，以按键中断为例 */
	TEST_INT_TIMER_INTERRUPT,			/* 内部中断测试，以定时器中断为例*/
	TEST_MAX					/* lase please */
}soc_test_t;


/* SDRAM不超限测试 */
#define SDRAM_BASE_ADDR (0x30000000)
#define SDRAM_OPERATOR_TEST_TIMES	(1024 * 1024 * 32)

#define SDRAM_OPERATOR_LEN (64) 	/* 复制内存对象长度 */
#define SDRAM_PRINT_CYCLE  (512)	/* 刷新进度显示周期 */

extern void test_led(void);
extern void test_uart(void);
extern void test_led_uart(void);
extern void test_sdram(void);
extern void test_relocation_less_than_4k(void);
extern void test_relocation_greater_than_4k(void);
extern void test_dealy(uint16 sec);
extern void test_thumb_instruction(void);
extern void test_ext_interrupt(void);

#endif /* __TEST_H__ */