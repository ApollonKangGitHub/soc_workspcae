#include <test.h>
#include <led.h>
#include <uart.h>
#include <tools.h>
#include <memCtrl.h>
#include <log.h>
#include <interrupt.h>
#include <timer.h>
#include <nor_flash.h>
#include <nand_flash.h>
#include <nand_flash_lib.h>
#include <lcd_common.h>
#include <frameBuffer.h>
#include <geometry.h>
#include <font.h>
#include <font_8x16.h>
#include <paletee.h>
#include <adc.h>
#include <touchScreenLib.h>
#include <i2c.h>
#include <at24cxx.h>
#include <oled.h>
#include <mmu.h>
#include <photosensitive.h>
#include <spi_flash.h>
#include <dht11.h>
#include <ds18b20.h>
#include <hs0038.h>
#include <necDataQueue.h>
#include <soc_s3c2440_init.h>
#include <soc_s3c2440_public.h>

#if (FALSE == SOC_S3C2440_SPI_CONTROLLER)
#include <gpio_spi.h>
#else
#include <spi_controller_s3c2440.h>
#endif

/*
 * 这些模块测试对象宏，
 * 主要用来在source Insight符号栏中合并打开测试对象，方便查看 
 * 此外可以用来屏蔽相应代码直接不编译
 */
#define TEST_OBJ_INTERRUPT
#define TEST_OBJ_UART
#define TEST_OBJ_LED
#define TEST_OBJ_LED_UART
#define TEST_OBJ_SDRAM
#define TEST_OBJ_RELOCATION
#define TEST_OBJ_THUMB_INSTRUCTION
#define TEST_OBJ_NOR_FLASH
#define TEST_OBJ_NAND_FLASH
#define TEST_OBJ_LCD
/* 没有外部扩展插座的话没办法测 */
#define TEST_OBJ_ADC_VOLTAGE	(TRUE)
#define TEST_OBJ_TOUCH_SCREEN
#define TEST_OBJ_IIC_EEPROM
#define TEST_OBJ_SPI_OLED_FLASH
#define TEST_OBJ_CACHE_MMU
#define TEST_OBJ_SENSORS

/* 一次最多擦除1M */
#define TEST_FLASH_EARSE_MAX	(1 << 20)
#define TEST_FLASH_OP_LEN_MAX 	(1024)
static uint8 gDataBuf[TEST_FLASH_OP_LEN_MAX];	

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

/* 设置Nor Flash 的时钟频率/周期，基于HCLK（高速总线时钟） */
static int test_nor_flash_start_squence_set(uint32 choose)
{
	SOC_DEBUG_PRINT(SOC_DBG_NORMAL, "choose is %d", choose);
	switch(choose)
	{
		case MEMCTRL_BANKCON_TACC_CLOCK_1:	
		case MEMCTRL_BANKCON_TACC_CLOCK_2:
		case MEMCTRL_BANKCON_TACC_CLOCK_3:
		case MEMCTRL_BANKCON_TACC_CLOCK_4:
		case MEMCTRL_BANKCON_TACC_CLOCK_6:
		case MEMCTRL_BANKCON_TACC_CLOCK_8:
		case MEMCTRL_BANKCON_TACC_CLOCK_10:
		case MEMCTRL_BANKCON_TACC_CLOCK_14:
			/*
			 * 修改NOR flash的访问周期TACC时序，指定对象为BANKCON0r寄存器的相应位(0~7)
   		     * 以增强访问效率，但是必须大于对应Nor flash需要的最小TACC
			 */
			BANK_CONFN_TACC_SET(BANKCON0r, choose);
			break;
		default:
			BANK_CONFN_TACC_SET(BANKCON0r, MEMCTRL_BANKCON_TACC_CLOCK_14);
			break;
	}
	return 0;
}

static int test_nor_flash_clk_init(void)
{
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "start init nor flash!");

	/* DW0 Indicate data bus width for bank 0 (read only). */
	/* BWSCONr not use and can't set */
	if(0x01 == (BWSCONr >> 1) & 0x03){
		print_screen("\n\rNor flash data width is 16bits!");
	}
	else if(0x02 == (BWSCONr >> 1) & 0x03){
		print_screen("\n\rNor flash data width is 32bits!");
	}
	else{
		print_screen("\n\rNor flash data width is unavail!");
	}
	/* 设置HCLK=100MHZ时的最小TACC以快速访问 */
	test_nor_flash_start_squence_set(MEMCTRL_BANKCON_TACC_CLOCK_8);

	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "start init nor succeed!");
}

void test_led_uart(void)
{
	/* 测试uart、LED、输入输出、Nor时钟频率修改等基本功能 */
	char str[_TOOL_GET_STRING_LEN_];
	uint32 led_cycle_times = 10;
	uint32 choose = -1;

	set_buffer(str, 0, sizeof(str));
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
				test_nor_flash_start_squence_set(choose);
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
		test_nor_flash_clk_init();
		#endif
	}
}
#endif

#ifdef TEST_OBJ_SDRAM
void test_sdram(void)
{
	int i = 0;
	BOOL test_flag = TRUE;
	uint32 error_read_count = 0;
		
	/* 注：对于裸机编译，需要声明字符串数组长度，否则会报错：  undefined reference to 'memcpy' */
	char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@";
	
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
		 * 2020年2月16日15:55:30，已经解决该编译问题，另外使用编译器自带除法库即可
		 */
		 if(i % SDRAM_PRINT_CYCLE == 0){
			print_screen("[write sdram <%d MB>, <%d KB>,   \r", 
				i / 0x100000, (i % 0x100000) / 0x400, i / (SDRAM_OPERATOR_TEST_TIMES/100), i, i);
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
		tool_delay(1);
	}
}
#endif /* TEST_OBJ_RELOCATION */

#ifdef TEST_OBJ_THUMB_INSTRUCTION
/* thumb指令集测试 */
void test_thumb_instruction(void)
{
	print_screen("\r\n THUMB instruction test[%s-%s-%d]!", __FILE__, __FUNCTION__, __LINE__);
	
	tool_delay(1);
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
	
	times++;

	/* 定时器中断频率为10ms，此处1s打印一次 */
	if (0 == (times % TIMER0_FREQUENCY))
	{
		print_screen("\r\n ----  timer0 %d second!  ----", times / TIMER0_FREQUENCY);
	}
	
	return;
}


/* 按键中断初始化，全部设置为下降沿触发 */
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
		EXTINT0_CON_EINT0_START_BIT, EXTINT_EINT_MASK, EXTINT_CON_FALL_EDGE_TRIGGER);
	/* s3下降沿触发中断 */
	SOC_S3C2440_REG_BITS_SET(EXTINT0r, \
		EXTINT0_CON_EINT2_START_BIT, EXTINT_EINT_MASK, EXTINT_CON_FALL_EDGE_TRIGGER);
	/* s4下降沿触发中断 */
	SOC_S3C2440_REG_BITS_SET(EXTINT1r, \
		EXTINT1_CON_EINT11_START_BIT, EXTINT_EINT_MASK, EXTINT_CON_FALL_EDGE_TRIGGER);
	/* s5下降沿触发中断 */
	SOC_S3C2440_REG_BITS_SET(EXTINT2r, \
		EXTINT2_CON_EINT19_START_BIT, EXTINT_EINT_MASK, EXTINT_CON_FALL_EDGE_TRIGGER);
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
	timer_register("test internal interrupt timer 0", test_interrupt_handle_timer0);
}

#endif	/* TEST_OBJ_INTERRUPT */

#if (defined TEST_OBJ_NOR_FLASH) || (defined TEST_OBJ_NAND_FLASH)

void test_nor_flash_scan(void)
{
	/* 打印厂家ID、设备ID、容量等信息 */
	print_screen("\r\n\r\n -------------------------------------------------------------");
	nor_flash_info_display();
	print_screen("\r\n -------------------------------------------------------------\r\n");

}

void test_nor_flash_earse(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 startAddr = 0x0;
	uint32 earseLen = 0;	

	/* 获取地址 */
	print_screen("\r\n Enter start address for earse[addr format 0x0~%x]:", NOR_FLASH_MAX_ADDR);
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &startAddr))
	{
		/* 获取长度 */
		print_screen("\r\n Enter earse bytes number[1~%d]:", TEST_FLASH_EARSE_MAX);
		set_buffer(str, 0, sizeof(str));
		(void)get_word(str, sizeof(str));
		
		if (tool_atoui((const char *)str, &earseLen))
		{
			print_screen("\r\n Input startAddr:%x, earseLen:%d", startAddr, earseLen);
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
	print_screen("\r\n -------------------------------------------------------------");
	nor_flash_earse_multi((uint32 *)startAddr, earseLen);
	print_screen("\r\n -------------------------------------------------------------");
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

void test_nor_flash_read(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 startAddr = 0x0;
	uint32 readLen = 0;	
	
	set_buffer(gDataBuf, 0, sizeof(gDataBuf));

	/* 获取地址 */
	print_screen("\r\n Enter start address for read[addr format 0x0~%x]:", NOR_FLASH_MAX_ADDR);
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &startAddr))
	{
		/* 获取长度 */
		print_screen("\r\n Enter read bytes number[1~%d]:", sizeof(gDataBuf));
		set_buffer(str, 0, sizeof(str));
		(void)get_word(str, sizeof(str));
		
		if (tool_atoui((const char *)str, &readLen))
		{
			print_screen("\r\n Input startAddr:%x, readLen:%d", startAddr, readLen);
			if (readLen > sizeof(gDataBuf)) {
				print_screen("\r\n Read up to %d bytes!!", sizeof(gDataBuf));
				goto err_ret;
			}
			nor_flash_read_multi(gDataBuf, readLen, (uint32*)startAddr);
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
	
	print_hexStr_multiple(gDataBuf, readLen, startAddr);
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

void test_nor_flash_write(void )
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 writeAddr = 0x0;
	uint16 writeVal = 0;	
	int writeLen = 0;
	int i = 0;
	int j = 1;
	
	/* 获取地址 */
	print_screen("\r\n Enter start address for write[addr format 0x0~%x]:", NOR_FLASH_MAX_ADDR);
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &writeAddr))
	{
		/* 获取要写的值 */
		print_screen("\r\n Enter string for write(max len %d):", sizeof(gDataBuf));
		set_buffer(gDataBuf, 0, sizeof(gDataBuf));
		(void)get_line(gDataBuf, sizeof(gDataBuf));
		writeLen = tool_strlen(gDataBuf);
		print_screen("\r\n write %s len is %d", gDataBuf, writeLen);
	}
	else
	{
		goto err_ret;
	}

	while ((gDataBuf[i] != '\0') && (gDataBuf[j] != '\0'))
	{
		/* 构造16位数据 */
		writeVal = gDataBuf[i] | (gDataBuf[j] << 8);
		nor_flash_write_word((uint32 *)writeAddr, writeVal);
		writeAddr += 2;
		i += 2;
		j += 2;
	}

	/* 写入最后一个字节'\0' */
	writeVal = gDataBuf[i];
	nor_flash_write_word((uint32 *)writeAddr, writeVal);
	
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

/* Nor flash测试，保证中断均屏蔽没有打开，或者即使打开也不会产生中断 */
void test_nor_flash(void)
{
	char selectOption = '\n';
	BOOL isFirst = TRUE;
	BOOL ismenuChoose = FALSE;
	
	while(TRUE) {

		if (isFirst || ismenuChoose)
		{
			isFirst = FALSE;
			ismenuChoose = FALSE;
			/* 
			 * 打印测试项菜单
			 * 1.识别Nor Flash
			 * 2.擦除Nor Flash
			 * 3.读Nor Flash
			 * 4.写Nor Flash
			 * 5.菜单信息
			 * 6.退出
			 */
			print_screen("\r\n -------------------------------------------------------------");
			print_screen("\r\n NOR FLASH TEST OBJ OPTIONALS");
			print_screen("\r\n -------------------------------------------------------------");
			print_screen("\r\n [s]scan nor flash.");
			print_screen("\r\n [e]erase nor flash test.");
			print_screen("\r\n [r]read nor flash test.");
			print_screen("\r\n [w]write nor flash test.");
			print_screen("\r\n [?]Menu info.");
			print_screen("\r\n [h]Menu info.");
			print_screen("\r\n [q]quit.");
			print_screen("\r\n -------------------------------------------------------------");
			
			print_screen("\r\n Enter selection: ");
			selectOption = tool_getChar();
		}
		else 
		{
			print_screen("\r\n Enter selection: ");
			selectOption = tool_getChar();
		}

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
				test_nor_flash_earse();
				break;

			case 'r':
			case 'R':
				test_nor_flash_read();
				break;
			
			case 'w':
			case 'W':
				test_nor_flash_write();
				break;

			case '?':
			case 'h':
				ismenuChoose = TRUE;
				break;
			
			case 'q':
			case 'Q':
				return;

			default:
				if ((selectOption >= 0x20) && (selectOption <= 0xFF))
				{
					print_screen("\r\n Select optional [ %c ] is invalid!!!", 
						selectOption, selectOption);
				}
				break;
		}
	}
	
	return;
}

/* 由于Nand启动时SRAM代码不宜过多，因此对于Nand Test，display、坏块检查入口函数都放在这里 */
static void nand_flash_info_display(void)
{
	nand_flash_info_t info;
	uint32 gbbyte; 
	uint32 mbbyte; 
	uint32 kbbyte;
	uint32 byte;

	set_buffer((uint8*)(&info), 0, sizeof(info));
	(void)nand_flash_get_mem_info(&info);

	/* 打印相关信息 */
	print_screen("\r\n Maker Id code            :0x");	print_byteHex(info.idInfo[_NAND_FLASH_ID_MAKER_]);
	print_screen("\r\n Device Id code           :0x");	print_byteHex(info.idInfo[_NAND_FLASH_ID_DEVICE_]);
	print_screen("\r\n 3rd Id Byte              :0x");	print_byteHex(info.idInfo[_NAND_FLASH_ID_3rd_]);
	print_screen("\r\n 4th Id Byte              :0x");	print_byteHex(info.idInfo[_NAND_FLASH_ID_4th_]);
	print_screen("\r\n 5th Id Byte              :0x");	print_byteHex(info.idInfo[_NAND_FLASH_ID_5th_]);

	print_screen("\r\n Plane number             :%d", info.planeNum);
	print_screen("\r\n Block number             :%d", info.blkNum);
	print_screen("\r\n Per block page number    :%d", info.pagePerBlk);

	tool_calc_mem_size(info.planeSize, &gbbyte, &mbbyte, &kbbyte, &byte);
	print_screen("\r\n Plane Size               :%dGB-%dMB", gbbyte, mbbyte);
	
	tool_calc_mem_size(info.totalSize, &gbbyte, &mbbyte, &kbbyte, &byte);
	print_screen("\r\n Total Size               :%dGB-%dMB", gbbyte, mbbyte);
	
	tool_calc_mem_size(info.pageSize, &gbbyte, &mbbyte, &kbbyte, &byte);
	print_screen("\r\n Page Size                :%dMB-%dKB", mbbyte, kbbyte);
	
	tool_calc_mem_size(info.blkSize, &gbbyte, &mbbyte, &kbbyte, &byte);
	print_screen("\r\n Block Size               :%dMB-%dKB", mbbyte, kbbyte);
}

void test_nand_flash_scan(void)
{
	/* 打印厂家ID、设备ID、页大小、块大小等信息 */
	print_screen("\r\n\r\n -------------------------------------------------------------");
	nand_flash_info_display();
	print_screen("\r\n -------------------------------------------------------------\r\n");
}

void test_nand_flash_data_read(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 startAddr = 0x0;
	uint32 readLen = 0;	
	
	set_buffer(gDataBuf, 0, sizeof(gDataBuf));

	/* 获取地址 */
	print_screen("\r\n Enter start address for read[addr format 0x0~0xFFFFFFFF]:");
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &startAddr))
	{
		/* 获取长度 */
		print_screen("\r\n Enter read bytes number[1~%d]:", sizeof(gDataBuf));
		set_buffer(str, 0, sizeof(str));
		(void)get_word(str, sizeof(str));
		
		if (tool_atoui((const char *)str, &readLen))
		{
			print_screen("\r\n Input startAddr:%x, readLen:%d", startAddr, readLen);
			if (readLen > sizeof(gDataBuf)) {
				print_screen("\r\n Read up to %d bytes!!", sizeof(gDataBuf));
				goto err_ret;
			}
			nand_flash_data_read(startAddr, gDataBuf, readLen);
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
	
	print_hexStr_multiple(gDataBuf, readLen, startAddr);
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

void test_nand_flash_oob_read(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 startAddr = 0x0;
	uint32 readLen = 0;	
	uint32 index = 0;
	uint32 printLen = 0;
	uint32 surplusLen = 0;
	uint32 curPos = 0;
	
	set_buffer(gDataBuf, 0, sizeof(gDataBuf));

	/* 获取地址 */
	print_screen("\r\n Enter start address for oob[addr format 0x0~0xFFFFFFFF]:");
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &startAddr))
	{
		/* 获取长度 */
		print_screen("\r\n Enter read bytes number[1~%d]:", sizeof(gDataBuf));
		set_buffer(str, 0, sizeof(str));
		(void)get_word(str, sizeof(str));
		
		if (tool_atoui((const char *)str, &readLen))
		{
			print_screen("\r\n Input startAddr:%x, readLen:%d", startAddr, readLen);
			if (readLen > sizeof(gDataBuf)) {
				print_screen("\r\n Read up to %d bytes!!", sizeof(gDataBuf));
				goto err_ret;
			}
			nand_flash_oob_read(startAddr, gDataBuf, readLen);
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

	/* 打印OOB数据 */
	for (index = 0; index < readLen; index++)
	{
		curPos = startAddr + index;
		if ((0 == (curPos % NAND_FLASH_OOB_SIZE)) || (curPos == startAddr))
		{
			surplusLen = readLen - index;
			printLen = (surplusLen >= NAND_FLASH_OOB_SIZE) 
					? (NAND_FLASH_OOB_SIZE - (curPos % NAND_FLASH_OOB_SIZE)) 
					: (surplusLen);

			print_screen("\r\n [page %d OOB:] ", curPos / NAND_FLASH_OOB_SIZE);
			print_hexStr_multiple(&gDataBuf[index], printLen, curPos);
		}
	}
	
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

/* flash坏块检查 */
static void test_nand_flash_check_all_blk(void)
{
	uint32 blkIndex = 0;
	BOOL isBadBlk = FALSE;
	BOOL isBad = FALSE;
	nand_flash_info_t info;

	set_buffer((uint8*)(&info), 0, sizeof(info));
	nand_flash_get_mem_info(&info);
	print_screen("\r\n Total have %d blocks, Per block have %d pages.", info.blkNum, info.pagePerBlk);
	
	for (blkIndex = 0; blkIndex < info.blkNum; blkIndex++)
	{
		isBadBlk = test_nand_flash_bad_blk_check(blkIndex, info.blkNum);
		if (isBadBlk) {
			print_screen("\r\n Block %d is bad block.", blkIndex);
			isBad = TRUE;
		}
	}
	
	if (!isBad) 
	{
		print_screen("\r\n All blocks are ok.", blkIndex);
	}
}


/* 检查坏块测试 */
void test_nand_flash_check(void)
{
	print_screen("\r\n\r\n -------------------------------------------------------------");
	test_nand_flash_check_all_blk();
	print_screen("\r\n\r\n -------------------------------------------------------------");
}

void test_nand_flash_earse(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 startAddr = 0x0;
	uint32 earseLen = 0;	
	int ret = OK;

	/* 获取地址 */
	print_screen("\r\n Enter start address for earse[addr format 0x0~0xFFFFFFFF]:");
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &startAddr))
	{
		/* 获取长度 */
		print_screen("\r\n Enter earse bytes number[1~%d]:", TEST_FLASH_EARSE_MAX);
		set_buffer(str, 0, sizeof(str));
		(void)get_word(str, sizeof(str));
		
		if (tool_atoui((const char *)str, &earseLen))
		{
			print_screen("\r\n Input startAddr:%x, earseLen:%d", startAddr, earseLen);
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

	ret = nand_flash_earse(startAddr, earseLen);
	if (ret != OK)
	{
		print_screen("\r\n Earse Failed!!");
	}
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}


void test_nand_flash_write(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 writeAddr = 0x0;
	uint16 writeVal = 0;	
	int writeLen = 0;
	int i = 0;
	int j = 1;
	
	/* 获取地址 */
	print_screen("\r\n Enter start address for write[addr format 0x0~0xFFFFFFFF]:");
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &writeAddr))
	{
		/* 获取要写的值 */
		print_screen("\r\n Enter string for write(max len %d):", sizeof(gDataBuf));
		set_buffer(gDataBuf, 0, sizeof(gDataBuf));
		(void)get_line(gDataBuf, sizeof(gDataBuf));
		writeLen = tool_strlen(gDataBuf);
		print_screen("\r\n write %s len is %d", gDataBuf, writeLen);
	}
	else
	{
		goto err_ret;
	}

	nand_flash_write(writeAddr, gDataBuf, writeLen);
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}


/* 注意，由于此时还不支持NAND flash的代码重定位，因此下载到Nor中去操作Nandflash */
void test_nand_flash(void)
{
	char selectOption = '\n';
	BOOL isFirst = TRUE;
	BOOL ismenuChoose = FALSE;

	if (isBootFromNorFlash())
	{
		/* Nand Flash init */
		nand_flash_init();
		print_screen("\r\n Nor Flash Start and Nand Flash Test init!");
	}

	print_screen("\r\n Nand Flash init succeed!");
	
	while(TRUE) {
		if (isFirst || ismenuChoose)
		{
			isFirst = FALSE;
			ismenuChoose = FALSE;
			/* 
			 * 打印测试项菜单
			 * 1.识别Nand Flash
			 * 2.读Nand Flash Data
			 * 3.读Nand Flash OOB
			 * 4.Nand Flash检查
			 * 5.擦除Nand Flash
			 * 6.写Nand Flash
			 * 7.菜单信息
			 * 8.退出
			 */
			print_screen("\r\n -------------------------------------------------------------");
			print_screen("\r\n NAND FLASH TEST OBJ OPTIONALS");
			print_screen("\r\n -------------------------------------------------------------");
			print_screen("\r\n [s]scan nand flash.");
			print_screen("\r\n [d]read nand flash data test.");
			print_screen("\r\n [o]read nand flash OOB test.");
			print_screen("\r\n [c]OOB check bad block.");
			print_screen("\r\n [e]erase nand flash test.");
			print_screen("\r\n [w]write nand flash test.");
			print_screen("\r\n [?]Menu info.");
			print_screen("\r\n [h]Menu info.");
			print_screen("\r\n [q]quit.");
			print_screen("\r\n -------------------------------------------------------------");
			
			print_screen("\r\n Enter selection: ");
			selectOption = tool_getChar();
		}
		else 
		{
			print_screen("\r\n Enter selection: ");
			selectOption = tool_getChar();
		}

		if ((selectOption >= 0x20) && (selectOption <= 0xFF))
		{
			print_screen("\r\n Select optional is [%c]", selectOption);
		}

		switch (selectOption) {
			case 's':
			case 'S':
				test_nand_flash_scan();
				break;

			case 'd':
			case 'D':
				test_nand_flash_data_read();
				break;
			
			case 'o':
			case 'O':
				test_nand_flash_oob_read();
				break;
			
			case 'c':
			case 'C':
				test_nand_flash_check();
				break;
			
			case 'e':
			case 'E':
				test_nand_flash_earse();
				break;
			
			case 'w':
			case 'W':
				test_nand_flash_write();
				break;
						
			case '?':
			case 'h':
				ismenuChoose = TRUE;
				break;
			
			case 'q':
			case 'Q':
				return;

			default:
				if ((selectOption >= 0x20) && (selectOption <= 0xFF))
				{
					print_screen("\r\n Select optional [ %c ] is invalid!!!", 
						selectOption, selectOption);
				}
				break;
		}
	}
	
	return;
}

void test_flash(void)
{
	char selectOption = '\n';
	BOOL isFirst = TRUE;
	BOOL ismenuChoose = FALSE;
	
	while(TRUE) {

		if (isFirst || ismenuChoose)
		{
			isFirst = FALSE;
			ismenuChoose = FALSE;

			print_screen("\r\n -------------------------------------------------------------");
			print_screen("\r\n NOR/NAND FLASH TEST OBJ OPTIONALS");
			print_screen("\r\n -------------------------------------------------------------");
			print_screen("\r\n [a]Nand flash test.");
			print_screen("\r\n [o]Nor flash test.");
			print_screen("\r\n [?]Menu info.");
			print_screen("\r\n [h]Menu info.");
			print_screen("\r\n [q]quit.");
			print_screen("\r\n -------------------------------------------------------------");
			
			print_screen("\r\n Enter selection: ");
			selectOption = tool_getChar();
		}
		else 
		{
			print_screen("\r\n Enter selection: ");
			selectOption = tool_getChar();
		}

		if ((selectOption >= 0x20) && (selectOption <= 0xFF))
		{
			print_screen("\r\n Select optional is [%c]", selectOption);
		}
		
		switch (selectOption) {
			case 'a':
			case 'A':
				test_nand_flash();
				isFirst = TRUE;
				break;
			
			case 'o':
			case 'O':
				/*
				 * Nand启动不允许测试Nor，
				 * 因为Nand启动0地址读写的是SRAM而不是Nor Flash
				 * 但是Nor启动可以测试Nand
				 */
				if (isBootFromNorFlash()){
					test_nor_flash();
				}
				else{
					print_screen("\r\n Current is boot from Nand Flash.");
					print_screen("\r\n Address 0x0 at SRAM, Can't running Nor Flash Test");
				}
				isFirst = TRUE;
				break;

			case '?':
			case 'h':
				ismenuChoose = TRUE;
				break;
			
			case 'q':
			case 'Q':
				return;

			default:
				if ((selectOption >= 0x20) && (selectOption <= 0xFF))
				{
					print_screen("\r\n Select optional [ %c ] is invalid!!!", 
						selectOption, selectOption);
				}
				break;
		}
	}

	return;
}

#endif /* #if (defined TEST_OBJ_NOR_FLASH) || (defined TEST_OBJ_NAND_FLASH) */

#ifdef TEST_OBJ_LCD
extern const rgb_256_info_t gRgb256Info[PALETEE_TYPE_NUM];

void test_lcd(void)
{
	lcd_common_para_t lcdPara;
	uint32 fb_base = 0;
	uint32 x_res = 0;
	uint32 y_res = 0;
	bpp_type_t bppType = bpp_type_max;
	draw_point_coordinate circle_center;
	draw_point_coordinate start;
	draw_point_coordinate end;
	draw_point_coordinate point;
	uint32 radius = 30;
	uint32 paletee = 0;
	
	int x, y;
	uint16 * pBpp16 = 0x0;
	uint32 * pBpp32 = 0x0;
	
	/* 初始化LCD */
	(void)lcd_common_init(lcd_type_4_3, lcd_controller_soc_s3c2440);

	/* 使能LCD */
	(void)lcd_common_enable(TRUE);

	/* frameBuffer init */
	(void)frameBuffer_init();

	/* 获取lcd参数：fb_base、xres、yres、bpp等 */
	(void)lcd_common_para_get(&lcdPara);
	fb_base = lcdPara._fb_base;
	x_res = lcdPara._x_res;
	y_res = lcdPara._y_res;
	bppType = lcdPara._bpp;

	frameBuffer_clear();
	print_screen_lcd(0,0,"\r\n bppType:%d, fb_base:%x, x_res:%d, y_res:%d", bppType, fb_base, x_res, y_res);
	tool_delay(1);

	/* 调色板测试（只有8bit用到了调色板） */
	if (bpp_type_8bits == bppType) 
	{
		frameBuffer_clear();
		print_screen_lcd(0,0,"\r\n paletee test!");
		tool_delay(1);

		/* 轮训调色板部分颜色 */
		x = 0; 
		y = 0; 
		paletee = paletee_256_type_Black_SYSTEM;
		while (paletee < paletee_256_type_Max)
		{
			/* 上半屏提示信息区清除 */
			for (y = 0; y < FONT_COL_MAX; y++)
				for (x = 0; x < x_res; x++)
					paletee_print_color_point(x, y, paletee_256_type_Black_SYSTEM);
				
			print_screen_lcd(0,0,"Top paletee traveral %d, %s.", paletee, gRgb256Info[paletee].colorName);

			/* 上半屏指定颜色（四列不同色） */	
			for (y = FONT_COL_MAX; y < (y_res / 2); y++)
				for (x = 0; x < x_res; x++)
					if (x < x_res / 4)
						paletee_print_color_point(x, y, paletee);
					else if (x < x_res / 2)
						paletee_print_color_point(x, y, paletee + 1);
					else if (x < x_res / 4 * 3)
						paletee_print_color_point(x, y, paletee + 2);
					else if (x < x_res)
						paletee_print_color_point(x, y, paletee + 3);
					
			paletee += 4;
					
			/* 下半屏提示信息区清除 */
			for ( ; y < (y_res / 2) + FONT_COL_MAX; y++)
				for (x = 0; x < x_res; x++)
					paletee_print_color_point(x, y, paletee_256_type_Black_SYSTEM);

			print_screen_lcd(0, (y_res / 2), "Bottom paletee traveral %d, %s.", paletee, gRgb256Info[paletee].colorName);	

			/* 下半屏指定颜色（四列不同色） */
			for ( ; y < y_res; y++)
				for (x = 0; x < x_res; x++)
					if (x < x_res / 4)
						paletee_print_color_point(x, y, paletee);
					else if (x < x_res / 2)
						paletee_print_color_point(x, y, paletee + 1);
					else if (x < x_res / 4 * 3)
						paletee_print_color_point(x, y, paletee + 2);
					else if (x < x_res)
						paletee_print_color_point(x, y, paletee + 3);
		
			paletee += 4;
		}

		frameBuffer_clear();
		
		/* 输出调色板的所有值 */
		x = 0;
		y = 0; 
		paletee = paletee_256_type_Black_SYSTEM;
		while (paletee < paletee_256_type_Max)
		{
			if (x > (x_res - PALETEE_COLOR_WIDTH))
			{
				x = 0;
				y += PALETEE_COLOR_WIDTH;
			}
			paletee_print_color_select(x, y, paletee);
			paletee++;
			x += PALETEE_COLOR_WIDTH;
		}

		tool_delay(1);
		frameBuffer_clear();
	}
	else
	{
		/* 真彩色测试 */
		frameBuffer_clear();
		print_screen_lcd(0,0,"\r\n rgb888 true color test start.");
		tool_delay(1);
		
		frameBuffer_fullScreen(RGB888_TRUE_COLOR | 0x00FF0000);	/* 红 */
		frameBuffer_fullScreen(RGB888_TRUE_COLOR | 0x0000FF00);	/* 绿 */
		frameBuffer_fullScreen(RGB888_TRUE_COLOR | 0x000000FF);	/* 蓝 */
		frameBuffer_fullScreen(RGB888_TRUE_COLOR | 0x00FFFF00);	/* 黄 */
		frameBuffer_fullScreen(RGB888_TRUE_COLOR | 0x00FF00FF);	/* 紫 */
		frameBuffer_fullScreen(RGB888_TRUE_COLOR | 0x0000FFFF);	/* 靛 */
	}
	
	/* 写圆、环测试 */
	{
		frameBuffer_clear();
		print_screen_lcd(0,0,"\r\n draw circle and annulus.");
		tool_delay(1);

		circle_center.x = radius;
		circle_center.y = radius;
		geometry_draw_circle_full(circle_center, radius, paletee_256_type_White_SYSTEM);
		circle_center.x = radius;
		circle_center.y = y_res / 2;
		geometry_draw_circle_empty(circle_center, radius, paletee_256_type_White_SYSTEM);
		circle_center.x = radius;
		circle_center.y = y_res - radius;
		geometry_draw_circle_full(circle_center, radius, paletee_256_type_White_SYSTEM);

		circle_center.x = x_res - radius;
		circle_center.y = radius;
		geometry_draw_circle_full(circle_center, radius, paletee_256_type_White_SYSTEM);
		circle_center.x = x_res - radius;
		circle_center.y = y_res / 2;
		geometry_draw_circle_empty(circle_center, radius, paletee_256_type_White_SYSTEM);
		circle_center.x = x_res - radius;
		circle_center.y = y_res - radius;
		geometry_draw_circle_full(circle_center, radius, paletee_256_type_White_SYSTEM);
		
		circle_center.x = x_res / 2;
		circle_center.y = y_res / 2;
		geometry_draw_annulus_full(circle_center, radius + 10, radius + 50, paletee_256_type_White_SYSTEM);
		geometry_draw_annulus_full(circle_center, 0, radius, paletee_256_type_White_SYSTEM);
	}
	
	/* 画线测试 */
	{
		frameBuffer_clear();
		print_screen_lcd(0,0,"\r\n draw line.");
		tool_delay(1);

		start.x = 0;
		start.y = 0;
		end.x = x_res - 1;
		end.y = y_res - 1;
		geometry_draw_line(start, end, paletee_256_type_White_SYSTEM);

		start.x = 0;
		start.y = y_res - 1;
		end.x = x_res - 1;
		end.y = 0;
		geometry_draw_line(start, end, paletee_256_type_White_SYSTEM);

		start.x = x_res / 2 - 1;
		start.y = 0;
		end.x = x_res / 2 - 1;
		end.y = y_res - 1;
		geometry_draw_line(start, end, paletee_256_type_White_SYSTEM);
		
		start.x = 0;
		start.y = y_res / 2 - 1;
		end.x = x_res - 1;
		end.y = y_res / 2 - 1;
		geometry_draw_line(start, end, paletee_256_type_White_SYSTEM);
	}

	/* 写字符串测试 */
	{
		frameBuffer_clear();
		print_screen_lcd(0,0,"\r\n font write test start.");
		tool_delay(1);
		
		frameBuffer_clear();
		font_print_string(0, 0, paletee_256_type_Red_SYSTEM, \
			"\r\n     Kangruojin love xxxxxx, Forever, Whenever, Wherever and Whatever." \
			"\r\n     Today is 2020-02-21 and 22:25.");
	}
}

#endif	/* TEST_OBJ_LCD */

#ifdef TEST_OBJ_ADC_VOLTAGE
void test_adc_voltage(void)
{
	uint32 val = 0;
	double vol = 0.0;
	uint32 integralPart = 0;
	uint32 fractionaPart = 0.0;

	/* 初始化LCD和frmaeBuffer */
	(void)lcd_common_init(lcd_type_4_3, lcd_controller_soc_s3c2440);
	(void)lcd_common_enable(TRUE);
	(void)frameBuffer_init();

	/* 初始化ADC */
	adc_init();

	print_screen("\r\n adc test voltage!!! \n\r");
	print_screen_lcd(0,0,"\r\n adc test voltage!!! \n\r");
	
#if (TRUE == TEST_OBJ_ADC_VOLTAGE)	
	while (TRUE)
	{
		val = adc_read_channel(adc_mux_type_ain0);
		vol = (double)val / ADC_VAL_MASK * 3.3;

		/* 整数和小数拆分 */
		integralPart = (uint32)vol;
		fractionaPart = (uint32)((vol - integralPart) * 10000);

		/* 串口打印 */
		print_screen("\r read ADC val:%d. ADC read Voltage: %d.%04d (V).    ", 
			val, integralPart, fractionaPart);
		
		/* LCD打印 */
		print_screen_lcd(0,16, "\r read ADC val:%d. ADC read Voltage: %d.%04d (V).    ", 
			val, integralPart, fractionaPart);
	}
#else
	print_screen("\r\n adc test voltage invalid!!! \n\r");
	print_screen_lcd(0,16,"\n\r adc test voltage invalid!!! \n\r");
#endif
}

#endif	/* TEST_OBJ_ADC_VOLTAGE */

#ifdef TEST_OBJ_TOUCH_SCREEN

void test_adc_touch_screen(void)
{
	draw_point_coordinate point;
	BOOL pressure = FALSE;
	int ret = OK;

	print_screen("\r\n adc test touch screen!!! \n\r");
		
	/* 定时器、触摸屏初始化 */
	timer_0_init();
	touchScreen_init();

	/* LCD初始化参数获取 */
	(void)lcd_common_init(lcd_type_4_3, lcd_controller_soc_s3c2440);
	(void)lcd_common_enable(TRUE);
	(void)frameBuffer_init();

	/* 触摸屏校准 */
	print_screen_lcd(70, 70, "\r\n Touch Screen Calibration!!");
	touchScreen_calibration();

	/* 清屏 */
	frameBuffer_clear();
	print_screen_lcd(70, 70, "\r\n Touch Screen draw line test!!");
	
	while(TRUE)
	{
		/* 触摸屏移动划线 */
		ret = touchScreen_get_point(&point.x, &point.y, &pressure);
		print_screen("\r\n ret=%d, pressure=%d, x=%d, y=%d", ret, pressure, point.x, point.y);
		if ((OK == ret) && pressure)
		{
			geometry_draw_point(point, paletee_256_type_Red_SYSTEM);
		}
	}
}

#endif	/* TEST_OBJ_TOUCH_SCREEN */

#ifdef TEST_OBJ_IIC_EEPROM 

#define AT24CXX_E2PROM_MAX_ADDR	(256)

void test_at24cxx_e2prom_i2c_read(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 offset = 0x0;
	uint32 readLen = 0;	
	int ret = 0;
	
	set_buffer(gDataBuf, 0, sizeof(gDataBuf));

	/* 获取地址 */
	print_screen("\r\n Enter start address for read[addr format 0x0~%x]:", AT24CXX_E2PROM_MAX_ADDR);
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &offset))
	{
		/* 获取长度 */
		print_screen("\r\n Enter read bytes number[1~%d]:", sizeof(gDataBuf));
		set_buffer(str, 0, sizeof(str));
		(void)get_word(str, sizeof(str));
		
		if (tool_atoui((const char *)str, &readLen))
		{
			print_screen("\r\n Input startAddr:%x, readLen:%d", offset, readLen);
			if (readLen > sizeof(gDataBuf)) {
				print_screen("\r\n Read up to %d bytes!!", sizeof(gDataBuf));
				goto err_ret;
			}
			ret = at24cxx_read_random((uint8)offset, gDataBuf, readLen);
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
	
	print_hexStr_multiple(gDataBuf, readLen, offset);
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

void test_at24cxx_e2prom_i2c_write(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 offset = 0x0;
	int writeLen = 0;
	int ret = 0;
	
	/* 获取地址 */
	print_screen("\r\n Enter start address for write[addr format 0x0~%x]:", AT24CXX_E2PROM_MAX_ADDR);
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &offset))
	{
		/* 获取要写的值 */
		print_screen("\r\n Enter string for write(max len %d):", sizeof(gDataBuf));
		set_buffer(gDataBuf, 0, sizeof(gDataBuf));
		(void)get_line(gDataBuf, sizeof(gDataBuf));
		writeLen = tool_strlen(gDataBuf);
		print_screen("\r\n write %s len is %d", gDataBuf, writeLen);
	}
	else
	{
		goto err_ret;
	}

	ret = at24cxx_write_bytes((uint8)offset, gDataBuf, writeLen);
	if (ret != OK)
	{
		print_screen("\r\n write offset:%d, writeLen:%d, failed!", offset, writeLen);		
	}
	
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

void test_i2c_e2prom(void)
{
	char selectOption = '\n';
	BOOL isFirst = TRUE;
	BOOL ismenuChoose = FALSE;

	/* 初始化 */
	i2c_init();
	
	while(TRUE) {

		if (isFirst || ismenuChoose)
		{
			isFirst = FALSE;
			ismenuChoose = FALSE;
			print_screen("\r\n -------------------------------------------------------------");
			print_screen("\r\n AT24CXX E2PROM TEST OBJ OPTIONALS");
			print_screen("\r\n -------------------------------------------------------------");
			print_screen("\r\n [r]read AT24CXX E2PROM test.");
			print_screen("\r\n [w]write AT24CXX E2PROM test.");
			print_screen("\r\n [?]Menu info.");
			print_screen("\r\n [h]Menu info.");
			print_screen("\r\n [q]quit.");
			print_screen("\r\n -------------------------------------------------------------");
			
			print_screen("\r\n Enter selection: ");
			selectOption = tool_getChar();
		}
		else 
		{
			print_screen("\r\n Enter selection: ");
			selectOption = tool_getChar();
		}

		if ((selectOption >= 0x20) && (selectOption <= 0xFF))
		{
			print_screen("\r\n Select optional is [%c]", selectOption);
		}
		
		switch (selectOption) {
			case 'r':
			case 'R':
				test_at24cxx_e2prom_i2c_read();
				break;
			
			case 'w':
			case 'W':
				test_at24cxx_e2prom_i2c_write();
				break;

			case '?':
			case 'h':
				ismenuChoose = TRUE;
				break;
			
			case 'q':
			case 'Q':
				return;

			default:
				if ((selectOption >= 0x20) && (selectOption <= 0xFF))
				{
					print_screen("\r\n Select optional [ %c ] is invalid!!!", 
						selectOption, selectOption);
				}
				break;
		}
	}
	
	return;
}

#endif	/* TEST_OBJ_IIC_EEPROM */

#ifdef TEST_OBJ_SPI_OLED_FLASH
#define SPI_FLASH_MAX_ADDR		(4096)

void test_spi_flash_earse(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 startAddr = 0x0;

	/* 获取地址 */
	print_screen("\r\n Enter start address for earse[addr format 0x0~%x]:", SPI_FLASH_MAX_ADDR);
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &startAddr))
	{
		print_screen("\r\n Input startAddr:%x.", startAddr);
	}
	else
	{
		goto err_ret;
	}
	print_screen("\r\n -------------------------------------------------------------");
	spi_flash_earse(startAddr);
	print_screen("\r\n -------------------------------------------------------------");
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

void test_spi_flash_read(void)
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 startAddr = 0x0;
	uint32 readLen = 0;	
	
	set_buffer(gDataBuf, 0, sizeof(gDataBuf));

	/* 获取地址 */
	print_screen("\r\n Enter start address for read[addr format 0x0~%x]:", SPI_FLASH_MAX_ADDR);
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &startAddr))
	{
		/* 获取长度 */
		print_screen("\r\n Enter read bytes number[1~%d]:", sizeof(gDataBuf));
		set_buffer(str, 0, sizeof(str));
		(void)get_word(str, sizeof(str));
		
		if (tool_atoui((const char *)str, &readLen))
		{
			print_screen("\r\n Input startAddr:%x, readLen:%d", startAddr, readLen);
			if (readLen > sizeof(gDataBuf)) {
				print_screen("\r\n Read up to %d bytes!!", sizeof(gDataBuf));
				goto err_ret;
			}
			spi_flash_read(startAddr, gDataBuf, readLen);
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
	
	print_hexStr_multiple(gDataBuf, readLen, startAddr);
	oled_clear();	
	print_screen_oled(0, 0, "read data: %s",  gDataBuf);
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

void test_spi_flash_write(void )
{
	char str[_TOOL_GET_STRING_LEN_];
	uint32 writeAddr = 0x0;
	uint16 writeVal = 0;	
	int writeLen = 0;
	int i = 0;
	int j = 1;
	
	/* 获取地址 */
	print_screen("\r\n Enter start address for write[addr format 0x0~%x]:", SPI_FLASH_MAX_ADDR);
	set_buffer(str, 0, sizeof(str));
	(void)get_word(str, sizeof(str));
	
	if (tool_atoux((const char *)str, &writeAddr))
	{
		/* 获取要写的值 */
		print_screen("\r\n Enter string for write(max len %d):", sizeof(gDataBuf));
		set_buffer(gDataBuf, 0, sizeof(gDataBuf));
		(void)get_line(gDataBuf, sizeof(gDataBuf));
		writeLen = tool_strlen(gDataBuf);
		print_screen("\r\n write %s len is %d", gDataBuf, writeLen);
	}
	else
	{
		goto err_ret;
	}

	spi_flash_write(writeAddr, gDataBuf, writeLen);
	oled_clear();
	print_screen_oled(0, 0, "write data: %s",  gDataBuf);
	return;
	
err_ret:
	print_screen("\r\n Invalid input %s!!", str);
	return;
}

void test_spi_oled_flash(void)
{
	uint8 MID = 0;
	uint8 DID = 0;

	/* OLED显示测试 */
	spi_init();
	oled_init();
	oled_print(0, 0, "I love C language and Embedded system.");

	tool_delay(1);

	/* SPI FLASH Read ID测试 */
	oled_clear();
	spi_flash_read_id(&MID, &DID);
	print_screen_oled(0, 0, "MID:%x, DID:%x.", MID, DID);

	/* SPI读写测试 */
	{
		char selectOption = '\n';
		BOOL isFirst = TRUE;
		BOOL ismenuChoose = FALSE;
		
		spi_flash_init();
		
		while(TRUE) {
	
			if (isFirst || ismenuChoose)
			{
				isFirst = FALSE;
				ismenuChoose = FALSE;

				print_screen("\r\n -------------------------------------------------------------");
				print_screen("\r\n SPI FLASH TEST OBJ OPTIONALS");
				print_screen("\r\n -------------------------------------------------------------");
				print_screen("\r\n [e]erase spi flash test.");
				print_screen("\r\n [r]read spi flash test.");
				print_screen("\r\n [w]write spi flash test.");
				print_screen("\r\n [?]Menu info.");
				print_screen("\r\n [h]Menu info.");
				print_screen("\r\n [q]quit.");
				print_screen("\r\n -------------------------------------------------------------");
				
				print_screen("\r\n Enter selection: ");
				selectOption = tool_getChar();
			}
			else 
			{
				print_screen("\r\n Enter selection: ");
				selectOption = tool_getChar();
			}
	
			if ((selectOption >= 0x20) && (selectOption <= 0xFF))
			{
				print_screen("\r\n Select optional is [%c]", selectOption);
			}
			
			switch (selectOption) {
				case 'e':
				case 'E':
					test_spi_flash_earse();
					break;
	
				case 'r':
				case 'R':
					test_spi_flash_read();
					break;
				
				case 'w':
				case 'W':
					test_spi_flash_write();
					break;
	
				case '?':
				case 'h':
					ismenuChoose = TRUE;
					break;
				
				case 'q':
				case 'Q':
					return;
	
				default:
					if ((selectOption >= 0x20) && (selectOption <= 0xFF))
					{
						print_screen("\r\n Select optional [ %c ] is invalid!!!", 
							selectOption, selectOption);
					}
					break;
			}
		}
		
		return;
	}
}
#endif	/* TEST_OBJ_SPI_OLED_FLASH */

#ifdef TEST_OBJ_CACHE_MMU

/*
 * 可以修改Makefile的MMU_CACHE_ENABLE变量控制编译情况
 * 来对比开启指令缓存与地址缓存前后程序运行速率
 */
void test_cache_mmu(void)
{
	print_screen("\r\n mmu test start!");

	/* 开启指令cahce之后LCD测试 */
	test_lcd();
#if (TRUE == SOC_S3C2440_MMU_CACHE_ENABLE)
	(void)mmu_page_table_dump();
#endif
	while(TRUE);
}

#endif	/* TEST_OBJ_CACHE_MMU */

#ifdef TEST_OBJ_SENSORS

#define TEST_OBJ_SENSORS_MENU_QUIT	(5)
#define TEST_OBJ_SENSORS_HIGH_PRECISION_DELAY (20)
#define TEST_OBJ_SENSORS_MENU_SELECT(select, pos)	(((select) == (pos)) ? ('>') : (' '))
volatile static BOOL gIsBreakCur = FALSE;
volatile static BOOL gIsInit = TRUE;
volatile static BOOL gDelayIsOver = TRUE;
volatile static int gSelect = -1;

/* 光敏电阻测试 */
void test_photosensitive_resistor(void)
{
	uint32 val = 0;
	double vol = 0.0;
	uint32 integralPart = 0;
	uint32 fractionaPart = 0.0;
	uint32 testTimes = 10000;

	/* 初始化光敏电阻控制信息 */
	photosensitive_init();
	print_screen_lcd(0,16*14, "test photosensitive resistor:\r\n");

	while (!gIsBreakCur)
	{
		/* 读取ADC 0通道，即可调电阻电压 */
		val = adc_read_channel(adc_mux_type_ain0);
		vol = (double)val / ADC_VAL_MASK * 3.3;

		/* 整数和小数拆分 */
		integralPart = (uint32)vol;
		fractionaPart = (uint32)((vol - integralPart) * 10000);

		/* LCD打印 */
		print_screen_lcd(0,16*15, "\r read ain0 :%d. Adjustable resistor Voltage: %d.%04d(V).    ", 
			val, integralPart, fractionaPart);

		/* 读取ADC 1通道，即光敏电阻电压 */
		val = adc_read_channel(adc_mux_type_ain1);
		vol = (double)val / ADC_VAL_MASK * 3.3;

		/* 整数和小数拆分 */
		integralPart = (uint32)vol;
		fractionaPart = (uint32)((vol - integralPart) * 10000);

		/* LCD打印 */
		print_screen_lcd(0,16*16, "\r read ain1 :%d. photosensitive resistor Voltage: %d.%04d(V).   ", 
			val, integralPart, fractionaPart);

		tool_mdelay(100);
	}

	photosensitive_detach();
}

/* 温湿度传感器测试 */
void test_temperature_humidity_sensor(void)
{
	int ret = OK;
	uint32 humidity;
	uint32 temperature;
	led_info ledArr[3] = {
		{led_num_d10, led_light_off}, 
		{led_num_d11, led_light_on}, 
		{led_num_d12, led_light_off}
	};

	print_screen_lcd(0, 16*13, "\rDHT11 init running..., Please wait!");

	(void)dht11_init();

	while (!gIsBreakCur)
	{
		/* 指示一轮完成状态变化 */
		ledArr[0].ledLight = (ledArr[0].ledLight+1) % led_light_max;
		ledArr[1].ledLight = (ledArr[1].ledLight+1) % led_light_max;
		ledArr[2].ledLight = (ledArr[2].ledLight+1) % led_light_max;
		led_light_all(3, ledArr);
	
		ret = dht11_read(&humidity, &temperature);
		if (OK == ret)
		{
			print_screen_lcd(0,16*14, "\r current temperature is :%d.     ", temperature);	
			print_screen_lcd(0,16*15, "\r current humidity is    :%d.     ", humidity);	
		}
		else
		{
			/* 读取失败重新初始化 */
			print_screen_lcd(0,16*14, "\r thd11 read temperature failed!  ");
			print_screen_lcd(0,16*15, "\r thd11 read humidity failed!!    ");
			(void)dht11_init();
		}
	}
}

/* DS18b20温度传感器测试 */
void test_temperature_sensor(void)
{
	double temperature = 0.0;
	uint32 readLen = 0;
	uint32 integralPart = 0;
	uint32 fractionaPart = 0;
	int index = 0;
	int ret = OK;

	print_screen_lcd(0, 16*13, "\rDS18B20 init running..., Please wait!");

	ds18b20_init();

	/* 读取ROM测试 */
	readLen = sizeof(gDataBuf);
	set_buffer(gDataBuf, 0, readLen);
	ret = ds18b20_read_rom(gDataBuf, &readLen);
	if (OK == ret)
	{
		print_screen_lcd(0, 16*14, "\rDS18B20 ROM[0~%d]:", readLen);
		for (index = 0; index < readLen; index++)
		{
			print_screen_lcd((5 + index) * 32, 16*14, "%d", gDataBuf[index]);
		}
	}
	else
	{
		print_screen_lcd(0,16*14, "\rRead ds18b20 rom failed!");
	}
	
	/* 读取RAM测试 */
	readLen = sizeof(gDataBuf);
	set_buffer(gDataBuf, 0, readLen);
	ret = ds18b20_read_ram(gDataBuf, &readLen);
	if (OK == ret)
	{
		print_screen_lcd(0, 16*15, "\rDS18B20 RAM[0~%d]:", readLen);
		for (index = 0; index < readLen; index++)
		{
			print_screen_lcd((5 + index) * 32, 16*15, "%d", gDataBuf[index]);
		}
	}
	else
	{
		print_screen_lcd(0,16*15, "\rRead ds18b20 ram failed!");
	}

	/* 读温度测试 */
	while (!gIsBreakCur)
	{
		if (OK == ds18b20_read_temperature(&temperature))
		{
			/* 整数和小数拆分 */
			integralPart = (uint32)temperature;
			fractionaPart = (uint32)((temperature - integralPart) * 10000);

			/* 串口打印 */
			print_screen_lcd(0,16*16, "\rTemperature:%d.%04d (^C).", integralPart, fractionaPart);			
		}
		else
		{
			print_screen_lcd(0,16*16, "\rTemperature read failed!!   ");	
		}
	}
}

/* 基于HS0038的NEC红外协议测试 */
void test_infrared_nec_protocol_test(void)
{
	necReadRet_t ret = nec_read_ret_max;
	uint8 necAddr = 0x0;
	uint8 necData = 0x0;
	char * necErrTypeStr[nec_read_ret_max] = {NEC_READ_RET_STR_ARR};
	
	hs0038_init();
	necDataQUeue_clear_data();

	while (!gIsBreakCur)
	{
		ret = hs0038_read_nec_data(&necAddr, &necData);
		if (ret == nec_read_ret_ok)
		{
			print_screen(". read data : addr:%x-data:%x", necAddr, necData);
		}
		else if (ret == nec_read_ret_repl)
		{
			print_screen(". repl data : addr:%x-data:%x", necAddr, necData);
		}
		else
		{
			print_screen(". read nec data failed, return %s[%d]", necErrTypeStr[ret], ret);
		}
	}

	hs0038_detach();
}

void * sensors_menu_select_key_s3(void * pArgv)
{
	if (gDelayIsOver)
	{
		gIsBreakCur = TRUE;
		gSelect = (gSelect + 1) % TEST_OBJ_SENSORS_MENU_QUIT;
	}
	else
	{
		print_screen_lcd(0,16*15, "\r high precision running..., please wait a monment!");
	}

	return NULL;
}

/* Key2选择菜单，Key3退出菜单 */
void * sensors_menu_quit_key_s4(void * pArgv)
{
	if (gDelayIsOver)
	{
		gSelect = TEST_OBJ_SENSORS_MENU_QUIT;
		gIsBreakCur = TRUE;
	}
	else
	{
		print_screen_lcd(0,16*15, "\r high precision running..., please wait a monment!");
	}
}

/* 高精度延时函数测试，可修改makefile的MMU_CACHE_ENABLE
 * 来对比开启/关闭指令缓存和数据缓存delay函数的精度的变化
 * 试着看了下，差别不是特别大，可能是因为delay的结构比较简单数据量和指令量都比较小
 */
void test_high_precision_delay(void)
{
	gDelayIsOver = FALSE;
	print_screen_lcd(0,16*14, "\r delay %d (s) running...", TEST_OBJ_SENSORS_HIGH_PRECISION_DELAY);
	tool_delay(TEST_OBJ_SENSORS_HIGH_PRECISION_DELAY);
	frameBuffer_clear_lines(16*15, 16*15);
	print_screen_lcd(0,16*15, "\r delay %d (s) end!!", TEST_OBJ_SENSORS_HIGH_PRECISION_DELAY);
	gDelayIsOver = TRUE;
	
	/* 高精度延时完成自动触发下一个 */
	sensors_menu_select_key_s3(NULL);
}

/* 传感器测试 */
void test_sensors(void)
{
	sys_time_t sysTime;
	struct tm * curTime;
	int selectOption = -1;
	led_info ledArr[3] = {
		{led_num_d10, led_light_off}, 
		{led_num_d11, led_light_on}, 
		{led_num_d12, led_light_off}
	};
	
	/* 初始化LCD、frmaeBuffer、定时器、按键中断、ADC等 */
	(void)lcd_common_init(lcd_type_4_3, lcd_controller_soc_s3c2440);
	(void)lcd_common_enable(TRUE);	
	(void)frameBuffer_init();	/* LCD显示需要 */
	(void)timer_0_init();		/* delay函数需要 */
	(void)adc_init();			/* 传感器ADC转换 */
	(void)led_init();			/* 状态指示灯 */
	
#ifdef TEST_OBJ_INTERRUPT
	/* 注册菜单同步按键选择功能 */
	test_interrupt_key_init();
	interrupt_register(interrupt_type_EXT_INT0, test_interrupt_handle_key_s2);
	interrupt_register(interrupt_type_EXT_INT2, sensors_menu_select_key_s3);
	interrupt_register(interrupt_type_EXT_INT11, sensors_menu_quit_key_s4);
#endif

	led_light_all(3, ledArr);

	while(TRUE) {
		while (gIsBreakCur || gIsInit)
		{
			selectOption = gSelect;
			gIsBreakCur = FALSE;
			gIsInit = FALSE;
			break;
		}

		set_buffer((uint8 *)&sysTime, 0, sizeof(sys_time_t));
		timer_get_sys_time(&sysTime);
		curTime = &sysTime.__tm__;
		
		print_screen_lcd(0, 16*0, "\r ----------------------------------------------------------");
		print_screen_lcd(0, 16*1, "\r SENSOR TEST OBJ OPTIONALS [%d-%d-%d %d:%d:%d]             ", 
			curTime->tm_year, curTime->tm_mon, curTime->tm_mday, curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
		print_screen_lcd(0, 16*2, "\r ----------------------------------------------------------");
		print_screen_lcd(0, 16*3, "\r %c[0]Photosensitive resistor test.", TEST_OBJ_SENSORS_MENU_SELECT(selectOption, 0));
		print_screen_lcd(0, 16*4, "\r %c[1]Temperature and humidity sensor DHT11 test.", TEST_OBJ_SENSORS_MENU_SELECT(selectOption, 1));
		print_screen_lcd(0, 16*5, "\r %c[2]Temperature sensor DS18B20 test.", TEST_OBJ_SENSORS_MENU_SELECT(selectOption, 2));
		print_screen_lcd(0, 16*6, "\r %c[3]Infrared NEC protocol with sensor HS0038 test.", TEST_OBJ_SENSORS_MENU_SELECT(selectOption, 3));
		print_screen_lcd(0, 16*7, "\r %c[4]High precision delay test.", TEST_OBJ_SENSORS_MENU_SELECT(selectOption, 4));
		print_screen_lcd(0, 16*8, "\r %c[5]Quit.", TEST_OBJ_SENSORS_MENU_SELECT(selectOption, 5));
		print_screen_lcd(0, 16*9, "\r ----------------------------------------------------------");
		print_screen_lcd(0, 16*10,"\r Press Key S2 select and Press Key3 quit.                  ");
		print_screen_lcd(0, 16*11,"\r Select optional is [%d].                    ", selectOption);
		print_screen_lcd(0, 16*12,"\r ----------------------------------------------------------");
		frameBuffer_clear_lines(16*13, 16*17);
		
		switch (selectOption) {
			case 0:
				test_photosensitive_resistor();
				break;

			case 1:
				test_temperature_humidity_sensor();
				break;
			
			case 2:
				test_temperature_sensor();
				break;
				
			case 3:
				test_infrared_nec_protocol_test();
				break;

			case 4:
				test_high_precision_delay();
				break;

			case 5:
				frameBuffer_clear();
				print_screen_lcd(0, 0, "\r Test quit......");
				return;

			default:
				print_screen(0, 16*11, "\r Select optional [%d] is invalid!!!", selectOption);
				break;
		}
	}
	
	return;
}

#endif	/* TEST_OBJ_SENSORS */
