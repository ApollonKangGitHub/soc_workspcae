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
#include <soc_s3c2440_init.h>
#include <soc_s3c2440_public.h>

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

/* 依次最多擦除1M */
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
