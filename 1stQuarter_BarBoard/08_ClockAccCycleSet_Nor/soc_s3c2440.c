#include <led.h>
#include <uart.h>
#include <tools.h>
#include <memCtrl.h>
#include <log.h>
#include <soc_s3c2440.h>

typedef enum{
	TEST_LED,
	TEST_UART,
	TEST_FUNCTION,

	TEST_MAX		/* lase please */
}soc_test_t;

void led_test()
{
	/* 点灯逻辑测试 */
	led_init();
	led_running();
}

void uart_test()
{
	/* 串口逻辑测试 */
	char ch;
	char strLen[12] = {0};
	char * pStr = NULL;
	int len = 0;
	
	uart_init();
	len = uart_puts("Hello S3C2440!\r\n");
	pStr = uitoa(len, (char *)(strLen));
	uart_puts("string len = ");
	uart_puts(pStr);
	uart_puts("\r\n");
	
	print_screen("\r\ntest1:%d, test2:%d\r\n", 2147483647, 54552);
	print_screen("\r\nTest at [%s - %X] %c Please Enter:\r\n", __FUNCTION__, __LINE__, 43);
	print_screen("\r\nTest at [%s - %X] %X Please Enter:\r\n", __FUNCTION__, __LINE__, 65536);

	while(1){
		
		ch = uart_getchar();
		if('\r' == ch){
			uart_putchar('\n');
		}
		else if('\n' == ch){
			uart_putchar('\r');
		}
		uart_putchar(ch);
	}
}

void test_function()
{
	/* 测试uart、LED、输入输出、Nor时钟频率修改等基本功能 */
	char str[CHANGE_TO_STR_BUFF_LEN] = {0};
	uint32 led_cycle_times = 10;
	uint8 choose = -1;
	
	uart_init();
	led_init();
	/* 内存控制器与点灯逻辑、uart混合测试 */
	while(1){
		/* 设置Nor   flash的时钟周期 */
		print_screen("\r\n##################### Choose Menu #####################\n\r");
		    print_screen("# 0:set bank0 config tacc(nor flash clock) is 1  HCLK #\r\n");
			print_screen("# 1:set bank0 config tacc(nor flash clock) is 2  HCLK #\r\n");
			print_screen("# 2:set bank0 config tacc(nor flash clock) is 3  HCLK #\r\n");
			print_screen("# 3:set bank0 config tacc(nor flash clock) is 4  HCLK #\r\n");
			print_screen("# 4:set bank0 config tacc(nor flash clock) is 6  HCLK #\r\n");
			print_screen("# 5:set bank0 config tacc(nor flash clock) is 8  HCLK #\r\n");
			print_screen("# 6:set bank0 config tacc(nor flash clock) is 10 HCLK #\r\n");
			print_screen("# 7:set bank0 config tacc(nor flash clock) is 14 HCLK #\r\n");
			print_screen("######################## End ##########################\n\r");
		print_screen("Please enter your choose:");
		
		choose = atoui(get_word(str, sizeof(str)));
		if(choose >= 0 && choose <= 7){
			nor_flash_start_squence_set(choose);	/* 设置Nor的时钟频率 */
			
			/* 设置LED的循环次数 */
			set_buffer(str, 0, CHANGE_TO_STR_BUFF_LEN);
			print_screen("Please set led cycle times:");

			led_cycle_times = atoui(get_word(str, sizeof(str)));
			SOC_DEBUG_PRINT(SOC_DBG_LOG, "times:%d\r\n", led_cycle_times);

			print_screen("\n\rPlease observe the status of the LED lamp.\r\n");
			led_running(led_cycle_times);
		}
		else{
			print_screen("\n\rError!Please choose again(between 0~7):\r\n");
			continue;
		}
	}
}

int soc_s3c2440_main(void)
{
	uint8 test_choose = TEST_FUNCTION;
	switch(test_choose){
		case TEST_LED:
			led_test();
			break;
		case TEST_UART:
			uart_test();
			break;
		case TEST_FUNCTION:
			test_function();
			break;
		default:
			print_screen("No have to test operation!\n\r");
			break;
	}
	return 0;
}