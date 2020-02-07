#include <test.h>
#include <led.h>
#include <uart.h>
#include <tools.h>
#include <memCtrl.h>
#include <log.h>
#include <soc_s3c2440.h>

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
	
	len = uart_puts("\r\nHello S3C2440!");
	pStr = tool_itoa(len, (char *)(strLen));
	uart_puts("\r\nstring len = ");
	uart_puts(pStr);
	uart_puts("\r\n");
	
	print_screen("\r\ntest1:%d, test2:%d", 100, 0);
	print_screen("\r\nTest at [%s - %X] %c Please Enter:", __FUNCTION__, __LINE__, 43);
	print_screen("\r\nTest at [%s - %X] %X Please Enter:", __FUNCTION__, __LINE__, 65536);

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
		
		choose = tool_atoui(get_word(str, sizeof(str)));
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

			led_cycle_times = tool_atoui(get_word(str, sizeof(str)));
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
/* 
 * SDRAM 测试中大量存在取余取整，因此THUMB指令集暂时不测试SRAM，
 * 用的基本都是ARM指令集，也没必要 在THUMB指令集时测试SRAM
 */
#if (SOC_S3C2440_THUMB_INTERRUPT_TEST == TRUE)
	print_screen("\r\nuse thumb interrupt set!");/* do nothing */
#else
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
		 * lib1funcs.S文件有对于这些指令的支持，但是编译器对应版本编译有问题，暂时先不管
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
#endif
	/* read sdram */
}

static char gChar_A = 'A';
const char gChar_B = 'B';
static char gChar_C = 'C';
static char gChar_D = 'D';
static char gChar_E = 'E';
static char gChar_F = 'F';
static char gChar_G = 0;
static char gChar_H = 0;
static char gChar_I = 0;
static char gChar_J = 0;
static char gChar_K = 0;

void test_relocation_greater_than_4k(void)
{
	/* 
	 * SDRAM重定位内存初始化、代码重定位、BSS段初始化、UART串口初始化
	 * 注意：bl指令是跳转到（CPU当前指令地址 + offset，offset由链接器计算）
	 * 如 bl 0x30000478 (程序开始地址为0x30000478时，PC指针为 0x3000005C)
	 * 实际的跳转地址offset = 0x30000478 - 0x3000005C = 0x41C,
	 * 由于代码实际是从Nor启动的，即从0地址开始执行的，CPU当前指令地址为0x5C，
	 * 因此"bl 0x30000478"实际为"bl 0x478"
	 * 即代码里的跳转需要是位置无关代码，位置无关代码：使用b、bl进行跳转,
	 * B/BL后面的地址，只是方便反汇编查看，并非实际的绝对地址，
	 * 绝对跳转即使用"ldr pc, =function addr"来实现
	 * 例如：当重定位、BSS初始化放在start.S中即main函数之前做的话，
	 * 则statrt.S中除了main以外的跳转都用b/BL、而跳转到main执行的语句用ldr，如下：
	 * bl uart_init
	 * bl sdram_init
	 * bl code_relocation
	 * bl bss_clear
	 * ldr pc, =main  //若使用"bl main"，则程序还是在NOR falsh上运行，运行速度会很慢
	 * 总结:重定位之前不可使用绝对地址，不可访问全局/静态变量，不可读有初始值的C数组，
	 * 因为C的全局变量、静态变量、数组数据成员的初始值（放在.rodata）都是通过绝对地址访问的
	 * 重定位后则可以访问绝对地址，根据反汇编来看是否访问绝对地址，以及访问是否已经重定位
	 * 注意：上面所讲绝对地址指的是内存，不包括寄存器
	 */
	 
	uart_init();

	sdram_init(SOC_MEMCTRL_BANK_SDRAM_ALL);	

	code_relocation();
	print_screen("\r\n code relocate init succeed!");
	
	bss_clear();
	print_screen("\r\n BSS section init succeed!");

	/* 
	 * 测试Nor启动和Nand启动以及代码重定位
	 * 说明:
	 * 	SRAM 只有4K 大小，SDRAM、SRAM、NOR Flash 对于CPU可以直接访问，但是Nand Flash不可以
	 *  SDRAM 首地址为 0x3000,0000
	 * Nand Flash启动:
	 * 设置为Nand启动时，SRAM起始地址为0，上电后硬件直接将Nand Flash的bin文件的前
	 * 4K字节复制到SRAM（0~0x7ff），然后CPU从0地址开始执行，若bin文件大于4K，则前4K
	 * 的代码就需要做大于4K字节代码的复制工作，复制到SDRAM上，复制完成后重定位CPU指针到
	 * 复制目的地SDRAM的地址。
	 * Nor Flash启动：
	 * 设置为Nor Flash启动时，0地址为Nor Flash的首地址，此时SRAM首地址为0x4000,0000
	 * 虽然CPU可以直接访问Nor Flash，但是由于Nor只能读不能写，因此存在于Nor上的全局变量/静态变量，
	 * 不能直接执行写操作，而需要前4K代码将整个Nor上的超过4K的代码复制到SDRAM 上后再执行SDRAM上的代码。
	 *
	 * 因此不管是Nand启动的大于4K的代码处理，还是Nor启动的全局/静态变量的处理，都需要借助SDRAM
	 * 即所谓的代码重定位
	 *
	 */
	while(1){
		/* Nor启动时， gChar_A++ 不起作用，但是如果重定位了数据地址到SDRAM，则可以起作用 */
		print_screen("\r\ngChar_A:%c(%d), gChar_B:%c(%d), &gChar_A:%x, &gChar_B:%x",
			gChar_A, gChar_A, gChar_B, gChar_B, &gChar_A, &gChar_B);
		gChar_A++;
		tool_dealy(1);
	}
}

/* thumb指令集测试 */
void test_thumb_interrupt(void)
{
	uart_init();
	sdram_init(SOC_MEMCTRL_BANK_SDRAM_ALL);	
	
	code_relocation();
	print_screen("\r\n code relocate init succeed!");
	
	bss_clear();
	print_screen("\r\n BSS section init succeed!");

	print_screen("\r\n THUMB interrupt test[%s-%s-%d]!", __FILE__, __FUNCTION__, __LINE__);
	tool_dealy(2);
	uart_test();
}

