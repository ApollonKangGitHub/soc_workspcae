#include <test.h>
#include <led.h>
#include <uart.h>
#include <tools.h>
#include <memCtrl.h>
#include <log.h>
#include <interrupt.h>
#include <timer.h>
#include <nor_flash.h>
#include <soc_s3c2440.h>
#include <soc_s3c2440_public.h>

/* 该宏只用来在source Insight符号栏中合并打开测试对象，方便查看 */
#define TEST_OBJ_INTERRUPT
#define TEST_OBJ_UART
#define TEST_OBJ_LED
#define TEST_OBJ_LED_UART
#define TEST_OBJ_SDRAM
#define TEST_OBJ_RELOCATION
#define TEST_OBJ_THUMB_INSTRUCTION
#define TEST_OBJ_NOR_FLASH

/* 调试函数 */
void test_start(void)
{
	print_screen("\r\n ---- start[%s-%d] ----", __FUNCTION__, __LINE__);
}
void test_end(void)
{
	print_screen("\r\n ---- over[%s-%d] ----", __FUNCTION__, __LINE__);
}

#ifdef TEST_OBJ_LED
void test_led(void)
{
	/* 点灯逻辑测试 */
	led_init();
	led_running();
}
#endif

#ifdef TEST_OBJ_UART
void test_uart(void)
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

#endif

#ifdef TEST_OBJ_LED_UART
void test_led_uart(void)
{
	/* 测试uart、LED、输入输出、Nor时钟频率修改等基本功能 */
	char str[_TOOL_GET_STRING_LEN_];
	uint32 led_cycle_times = 10;
	uint32 choose = -1;

	set_buffer(str, 0, sizeof(str));
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
		
		if (!tool_atoui(get_word(str, sizeof(str)), &choose))
		{
			print_screen("\r\n Invalid input %s!!!", str);
			continue;
		}
		
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
			set_buffer(str, 0, sizeof(str));
			print_screen("Please set led cycle times:");
			
			if (!tool_atoui(get_word(str, sizeof(str)), &led_cycle_times))
			{
				print_screen("\r\n Invalid input %s!!!", str);
				continue;
			}

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
#endif

#ifdef TEST_OBJ_SDRAM
void test_sdram(void)
{
/* 
 * SDRAM 测试中大量存在取余取整，因此THUMB指令集暂时不测试SRAM，
 * 用的基本都是ARM指令集，也没必要 在THUMB指令集时测试SRAM
 */
#if (SOC_S3C2440_THUMB_INSTRUCTION_TEST == TRUE)
	print_screen("\r\nuse thumb instruction set!");/* do nothing */
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
#endif /* TEST_OBJ_SDRAM */

#ifdef TEST_OBJ_RELOCATION
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
	 
	relocate_init();

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
#endif /* TEST_OBJ_RELOCATION */

#ifdef TEST_OBJ_THUMB_INSTRUCTION
/* thumb指令集测试 */
void test_thumb_instruction(void)
{
	print_screen("\r\n THUMB instruction test[%s-%s-%d]!", __FILE__, __FUNCTION__, __LINE__);
	
	tool_dealy(2);
	test_uart();
}
#endif	/* TEST_OBJ_THUMB_INSTRUCTION */

#ifdef TEST_OBJ_INTERRUPT

/* debug开关测试 */
void * test_interrupt_handle_key_s2(void * pArgv)
{
	print_screen("\r\n ----  key_s2[debug switch]!  ----");
	gSysDebugFlag = !gSysDebugFlag;
	print_screen("\r\n gSysDebugFlag:%d", gSysDebugFlag);
	return;
}

void * test_interrupt_handle_key_s3(void * pArgv)
{
	print_screen("\r\n ----  key_s3!  ----");
	return;
}

void * test_interrupt_handle_key_s4(void * pArgv)
{
	print_screen("\r\n ----  key_s4!  ----");
	return;
}

void * test_interrupt_handle_key_s5(void * pArgv)
{
	print_screen("\r\n ----  key_s5!  ----");
	return;
}

void * test_interrupt_handle_timer0(void * pArgv)
{
	static volatile int times = 0;
	print_screen("\r\n ----  timer0 counter:%d!  ----", times++);
	return;
}


/* 按键中断初始化，全部设置为上升沿下降沿双触发 */
void  test_interrupt_key_init(void)
{
	/* 
	 * 1、配置GPIO中断引脚
	 * 四个按键S2、S3、S4、S5对应EINT0、EINT2、EINT11、EINT19
	 * 即GPFCON和GPGCON两个寄存器
	 */

	/* GPFCON寄存器和寄存器GPGCON清空 */
	GPFCONr &= ~(GPFCON_GPF0_CONF_BITSf | GPFCON_GPF2_CONF_BITSf); 	
	GPGCONr &= ~(GPGCON_GPG3_CONF_BITSf | GPGCON_GPG11_CONF_BITSf); 
	
	/* 相关引脚配置为中断引脚 */
	GPFCONr |= (GPFCON_EINT << GPIOCON_PIN0_START_BIT);
	GPFCONr |= (GPFCON_EINT << GPIOCON_PIN2_START_BIT);
	GPGCONr |= (GPGCON_EINT << GPIOCON_PIN3_START_BIT);
	GPGCONr |= (GPGCON_EINT << GPIOCON_PIN11_START_BIT);

	/* s2下降沿触发中断 */
	SOC_S3C2440_REG_BITS_SET(EXTINT0r, \
		EXTINT0_CON_EINT0_START_BIT, EXTINT0_CON_EINT0_BITSf, EXTINT_CON_FALL_EDGE_TRIGGER);
	/* s3下降沿触发中断 */
	SOC_S3C2440_REG_BITS_SET(EXTINT0r, \
		EXTINT0_CON_EINT2_START_BIT, EXTINT0_CON_EINT2_BITSf, EXTINT_CON_FALL_EDGE_TRIGGER);
	/* s4下降沿触发中断 */
	SOC_S3C2440_REG_BITS_SET(EXTINT1r, \
		EXTINT1_CON_EINT11_START_BIT, EXTINT1_CON_EINT11_BITSf, EXTINT_CON_FALL_EDGE_TRIGGER);
	/* s5下降沿触发中断 */
	SOC_S3C2440_REG_BITS_SET(EXTINT2r, \
		EXTINT2_CON_EINT19_START_BIT, EXTINT2_CON_EINT19_BITSf, EXTINT_CON_FALL_EDGE_TRIGGER);
	
	/* 3、设置EINTMASK使能中断 */
	SOC_S3C2440_REG_BIT_SET(EINTMASKr, EINTMASK_EINT11_START_BIT, INTMASK_INT_ENABLE);
	SOC_S3C2440_REG_BIT_SET(EINTMASKr, EINTMASK_EINT19_START_BIT, INTMASK_INT_ENABLE);
}

/* 外部按键中断初始化 */
void test_interrupt_ext_key_init(void)
{
	test_interrupt_key_init();

	/* 注册回调函数，使能中断控制器 */
	interrupt_register(interrupt_type_EXT_INT0, test_interrupt_handle_key_s2);
	interrupt_register(interrupt_type_EXT_INT2, test_interrupt_handle_key_s3);
	interrupt_register(interrupt_type_EXT_INT11, test_interrupt_handle_key_s4);
	interrupt_register(interrupt_type_EXT_INT19, test_interrupt_handle_key_s5);
}

/* 内部定时器中断初始化 */
void test_interrupt_int_timer_init(void)
{
	timer_0_init();

	/* 注册回调函数，使能中断控制器 */
	interrupt_register(interrupt_type_INT_TIMER0, test_interrupt_handle_timer0);
}

#endif	/* TEST_OBJ_INTERRUPT */

#ifdef TEST_OBJ_NOR_FLASH

void test_nor_flash_scan(void)
{
	/* 打印厂家ID、设备ID、容量等信息 */
	print_screen("\r\n\r\n -------------------------------------------------------------");
	nor_flash_info_display();
	print_screen("\r\n -------------------------------------------------------------\r\n");

}

#define TEST_NOR_FLASH_READ_LEN_MAX 	(1024)

void test_nor_flash_read(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	static uint8 readData[TEST_NOR_FLASH_READ_LEN_MAX];	
	uint32 startAddr = 0x0;
	uint32 readLen = 0;	
	
	set_buffer(readData, 0, sizeof(readData));

	print_screen("\r\n Enter start address for read[addr format 0x0~%x]:", NOR_FLASH_MAX_ADDR);

	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &startAddr))
	{
		print_screen("\r\n Enter read bytes number[1~%d]:", sizeof(readData));
		
		set_buffer(str, 0, sizeof(str));
		(void)get_word(str, sizeof(str));
		
		if (tool_atoui((const char *)str, &readLen))
		{
			print_screen("\r\n Input startAddr:%x, readLen:%d", startAddr, readLen);
			if (readLen > sizeof(readData)) {
				print_screen("\r\n Read up to %d bytes!!", sizeof(readData));
				goto err_ret;
			}
			nor_flash_read_multi(readData, readLen, (uint32*)startAddr);
		}
		else 
		{
			goto err_ret;
		}
	}
	else
	{
		goto err_ret;
	}
	
	print_hexStr_multiple(readData, readLen, startAddr);
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

/* Nor flash测试，保证中断均屏蔽没有打开，或者即使打开也不会产生中断 */
void test_nor_flash(void)
{
	char selectOption = '\n';

	while(TRUE) {
		/* 
		 * 打印测试项菜单
		 * 1.识别Nor Flash
		 * 2.擦除Nor Flash
		 * 3.读Nor Flash
		 * 4.写Nor Flash
		 */
		print_screen("\r\n -------------------------------------------------------------");
		print_screen("\r\n NOR FLASH TEST OBJ OPTIONALS");
		print_screen("\r\n -------------------------------------------------------------");
		print_screen("\r\n [s]scan nor flash.");
		print_screen("\r\n [e]erase nor flash test.");
		print_screen("\r\n [r]read nor flash test.");
		print_screen("\r\n [w]write nor flash test.");
		print_screen("\r\n [q]quit.");
		print_screen("\r\n -------------------------------------------------------------");
		print_screen("\r\n Enter selection: ");

		selectOption = uart_getchar();
		if ((selectOption >= 0x20) && (selectOption <= 0xFF))
		{
			print_screen("\r\n Select optional is [%c]", selectOption);
		}
		
		switch (selectOption) {
			case 's':
			case 'S':
				test_nor_flash_scan();
				break;
			
			case 'e':
			case 'E':
				//test_nor_flash_earse();
				break;

			case 'r':
			case 'R':
				test_nor_flash_read();
				break;
			
			case 'w':
			case 'W':
				//test_nor_flash_write();
				break;

			case 'q':
			case 'Q':
				return;
				break;
			
			default:
				if ((selectOption >= 0x20) && (selectOption <= 0xFF))
				{
					print_screen("\r\n Select optional [#%c-%d#] is invalid!!!", 
						selectOption, selectOption);
				}
				break;
		}
	}
	
	return;
}

#endif	/* TEST_OBJ_NOR_FLASH */
