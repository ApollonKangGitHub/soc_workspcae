#include <test.h>
#include <tools.h>

int soc_s3c2440_main(void)
{
	uint8 test_choose = TEST_RELOCATION_GREATER_THAN_4K;
	
#if (TRUE == SOC_S3C2440_THUMB_INSTRUCTION_TEST)
	test_choose = TEST_THUMB_INSTRUCTION;
#elif (TRUE == SOC_S3C2440_UNDEFINED_INSTRUCTION_TEST)
	test_choose = TEST_UDF_INSTRUCTION_EXCEPTION;
#endif

	print_screen("\r\n---------------------------------------------");
	print_screen("\r\nSOC S3C2440 MAIN TEST START!!");
	print_screen("\r\n---------------------------------------------");

	switch(test_choose){
		#if 0
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
			print_screen("\r\nundefined instruction exceptinon test...");
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

