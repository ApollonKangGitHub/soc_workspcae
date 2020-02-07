#include <test.h>
#include <tools.h>

int soc_s3c2440_main(void)
{
	uint8 test_choose = TEST_RELOCATION_GREATER_THAN_4K;

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
		case TEST_RELOCATION_LESS_THAN_4K:
			test_relocation_less_than_4k();
			break;
		#endif
		case TEST_RELOCATION_GREATER_THAN_4K:
			test_relocation_greater_than_4k();
			break;
		default:
			uart_init();
			print_screen("No have to test operation!\n\r");
			break;
	}
	return 0;
}
