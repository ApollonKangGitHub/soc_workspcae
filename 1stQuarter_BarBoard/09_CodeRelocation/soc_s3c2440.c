#include <test.h>
#include <tools.h>

int soc_s3c2440_main(void)
{
	uint8 test_choose = TEST_RELOCATION_LESS_THAN_4K;
/*
 * 注意：此处的重定位测试有编译其他模块，bin文件可能大于4K，但是我们没有编写大于4K代码的拷贝指令，
 * 因此，测试全局变量在Nor上写行为时，需要确保其bin文件大小小于4K，不编译其他模块，此处调用的代码也注释掉
 * 如果还是过大，则可以把tools.c里不相关代码先屏蔽掉
 */
	switch(test_choose){
		#if 0
		case TEST_LED:
			led_test();
			break;
		case TEST_UART:
			uart_test();
			break;
		case TEST_LED_UART:
			test_led_uart();
			break;
		case TEST_SDRAM:
			test_sdram();
			break;
		#endif
		case TEST_RELOCATION_LESS_THAN_4K:
			test_relocation_less_than_4k();
			break;
		#if 0
		case TEST_RELOCATION_GREATER_THAN_4K:
			test_relocation_greater_than_4k();
			break;
		#endif
		default:
			uart_init();
			print_screen("No have to test operation!\n\r");
			break;
	}
	return 0;
}
