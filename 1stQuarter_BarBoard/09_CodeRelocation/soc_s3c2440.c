#include <test.h>
#include <tools.h>

int soc_s3c2440_main(void)
{
	uint8 test_choose = TEST_RELOCATION;
	
	switch(test_choose){
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
		case TEST_RELOCATION:
			test_relocation();
			break;
		default:
			uart_init();
			print_screen("No have to test operation!\n\r");
			break;
	}
	return 0;
}
