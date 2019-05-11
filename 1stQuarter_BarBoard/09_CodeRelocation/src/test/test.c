#include <test.h>
#include <led.h>
#include <uart.h>
#include <tools.h>
#include <memCtrl.h>
#include <log.h>
#include <soc_s3c2440.h>

void test_dealy(uint16 sec)
{
	int delay = 400000;
	while(sec--){
		while(delay--){
			;/* nop */
		}
	}
}

void led_test(void)
{
	/* 点灯逻辑测试 */
	led_init();
	led_running();
}

void uart_test(void)
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

void test_led_uart(void)
{
	/* 测试uart、LED、输入输出、Nor时钟频率修改等基本功能 */
	char str[CHANGE_TO_STR_BUFF_LEN] = {0};
	uint32 led_cycle_times = 10;
	uint8 choose = -1;
	
	uart_init();
	led_init();
	/* 内存控制器与点灯逻辑、uart混合测试 */
	while(1){
		#if 1
		/* 设置Nor   flash的时钟周期，测试用例 */
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
			/* 设置Nor的TACC时间
			 * 默认是最大的14，可以最大程度保证访问Nor flash，我的板子大于70ns即可
   			 * 即需要正常工作，必须保证在5及其以上，否则Nor中的指令是无法正常读取的
			*/
			if(choose >= 5){
				nor_flash_start_squence_set(choose);
			}
			else{
				print_screen("\n\r当前NorFlash需要设置TACC大于4才可正常工作，请重新选择！\r\n");
				continue;
			}
			
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
		#else 
		/* 直接初始化nor flash访问的TACC */
		nor_flash_init();
		#endif
	}
}

void test_sdram(void)
{
	int i = 0;
	BOOL test_flag = TRUE;
	uint32 error_read_count = 0;
		
	/* 注：对于裸机编译，需要声明字符串数组长度，否则会报错：  undefined reference to 'memcpy' */
	char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@";
	
	uart_init();
	sdram_init(SOC_MEMCTRL_BANK_SDRAM_ALL);
	
	/* sdram base address */
	volatile uint8 * sdram_base_addr = (volatile uint8 *)(SDRAM_BASE_ADDR);

	SOC_DEBUG_PRINT(SOC_DBG_NORMAL, "\r\nWrite sdram test start...\r\n");
	volatile uint8 * arr = sdram_base_addr;
	/* write sdram */
	while(i < SDRAM_OPERATOR_TEST_TIMES){
		arr[i] = str[i % SDRAM_OPERATOR_LEN];
		/* 
		 * 注：
		 * arm-linux-gcc 编译器对'%'和'/'的支持不是很好，
		 * 在编译阶段取余或取整基数必须是常量（常量宏或整数都可以），
		 * 否则会报错：undefined reference to `__aeabi_idivmod'
		 */
		 if(i % SDRAM_PRINT_CYCLE == 0){
			print_screen("[write sdram <%d MB>, <%d KB>, [%%d]]      \r", 
				i / 0x100000, (i % 0x100000) / 0x400, i / (SDRAM_OPERATOR_TEST_TIMES/100));
		}
		i++;
	}
	print_screen("[write sdram <%d MB>, <%d KB>, [%%d]]      \r\n", 32, 0, 100);
	print_screen("[Total write sdram %d times]\n\r", i);

	
	SOC_DEBUG_PRINT(SOC_DBG_NORMAL, "\r\nRead sdram test start...\r\n");
	i = 0;
	arr =  (volatile uint8 *)(sdram_base_addr);
	/* read sdram and print to screen accross uart */
	while(i < SDRAM_OPERATOR_TEST_TIMES){
		if(arr[i] != str[i % SDRAM_OPERATOR_LEN]){
			test_flag = FALSE;
			error_read_count++;
			print_screen("\n\rerror:%c, real:%c", arr[i], str[i % SDRAM_OPERATOR_LEN]);
		}
		if(i % SDRAM_PRINT_CYCLE == 0){
			print_screen("[read sdram <%d MB>, <%d KB>, [%%d>]]      \r", 
				i / 0x100000, ((i % 0x100000) / 0x400),  i / (SDRAM_OPERATOR_TEST_TIMES/100));
		}
		i++;
	}
	print_screen("[write sdram <%d MB>, <%d KB>, [%%d]]      \r\n", 32, 0, 100);
	print_screen("[Total read sdram %d times]\n\r", i);

	SOC_DEBUG_PRINT(SOC_DBG_NORMAL,
		"\r\ntest sdram end, test result is %s,error count is %d!\r\n", 
		test_flag ? "succeed" : "failure", test_flag ? 0 : error_read_count);
	
	/* read sdram */
}

char gChar_A = 'A';
const char gChar_B = 'B';
void test_relocation(void)
{
	uart_init();

	/* 测试Nor启动和Nand启动以及代码重定位 */
	while(1){
		print_screen("%c", gChar_A++);
		test_dealy(1);
	}
}

