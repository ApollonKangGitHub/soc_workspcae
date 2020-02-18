#include <test.h>
#include <tools.h>
#include <soc_s3c2440_public.h>
#define ENUM_TYPE_TO_STR(type) #type

/*
 * 2020-2-16 02:40:51
 * switch case 的case条件命名满足，但是就是每次都进的是default
 * 用print_screen打印出来获取的输入字符也是满足case条件的，但是就是进不了case
 * 后来发现是串口问题，换了个串口就好了，很奇怪，难道是串口驱动有问题？
 */


/* 测试用例名称，和枚举soc_test_t结构保持一致 */
static char *gTestObjStr[] = {
	ENUM_TYPE_TO_STR(TEST_LED),
	ENUM_TYPE_TO_STR(TEST_UART),
	ENUM_TYPE_TO_STR(TEST_LED_UART),
	ENUM_TYPE_TO_STR(TEST_SDRAM),
	ENUM_TYPE_TO_STR(TEST_RELOCATION_LESS_THAN_4K),
	ENUM_TYPE_TO_STR(TEST_RELOCATION_GREATER_THAN_4K),
	ENUM_TYPE_TO_STR(TEST_THUMB_INSTRUCTION),
	ENUM_TYPE_TO_STR(TEST_UDF_INSTRUCTION_EXCEPTION),
	ENUM_TYPE_TO_STR(TEST_SWI_EXCEPTION),
	ENUM_TYPE_TO_STR(TEST_EXT_KEY_INTERRUPT),
	ENUM_TYPE_TO_STR(TEST_INT_TIMER_INTERRUPT),
	ENUM_TYPE_TO_STR(TEST_NOR_FLASH),
	ENUM_TYPE_TO_STR(TEST_NAND_FLASH),
	ENUM_TYPE_TO_STR(TEST_LCD),
	ENUM_TYPE_TO_STR(TEST_MAX)
};
	
int soc_s3c2440_main(void)
{
	uint8 test_choose = TEST_MAX;
	
#if (TRUE == SOC_S3C2440_THUMB_INSTRUCTION_TEST)
	test_choose = TEST_THUMB_INSTRUCTION;
#elif (TRUE == SOC_S3C2440_UND_INSTRUCTION_TEST)
	test_choose = TEST_UDF_INSTRUCTION_EXCEPTION;
#elif (TRUE == SOC_S3C2440_SWI_EXCEPTION)
	test_choose = TEST_SWI_EXCEPTION;
#endif

	test_choose = TEST_LCD;

	print_screen("\r\n---------------------------------------------------------------------------");
	print_screen("\r\nSOC S3C2440 MAIN TEST %s[%d] START!!", gTestObjStr[test_choose], test_choose);
	print_screen("\r\n---------------------------------------------------------------------------");

	switch(test_choose){
		case TEST_LED:
			test_led();
			break;
		case TEST_UART:
			test_uart();
			break;
		case TEST_LED_UART:
			test_led_uart();
			break;
		case TEST_SDRAM:
			test_sdram();
			break;
		#if 0
		case TEST_RELOCATION_LESS_THAN_4K:
			test_relocation_less_than_4k();
			break;
		#endif
		case TEST_RELOCATION_GREATER_THAN_4K:
			test_relocation_greater_than_4k();
			break;
		case TEST_THUMB_INSTRUCTION:
			test_thumb_instruction();
			break;
		case TEST_UDF_INSTRUCTION_EXCEPTION:
		case TEST_SWI_EXCEPTION:
			print_screen("\r\nCpu mode change and exceptinon test...");
			break;
		case TEST_EXT_KEY_INTERRUPT:
			test_interrupt_ext_key_init();
			print_screen("\r\nPress key to occour key interrupt!");
			break;
		case TEST_INT_TIMER_INTERRUPT:
			test_interrupt_ext_key_init();
			test_interrupt_int_timer_init();
			print_screen("\r\nExternal key interrupt and internal timer interrupt init succeed!");
			while(TRUE) {
				timer_0_debug_print();
				tool_dealy(2);
			}
			break;
		case TEST_NOR_FLASH:
		case TEST_NAND_FLASH:
			test_flash();
			break;
		case TEST_LCD:
			//test_flash();
			test_lcd();
			break;
		default:
			print_screen("\r\nNo have to test operation!");
			break;
	}
	
	return 0;
}

void soc_s3c2440_over(void)
{
	print_screen("\r\nSOC_S3C2440 is over!");
}

