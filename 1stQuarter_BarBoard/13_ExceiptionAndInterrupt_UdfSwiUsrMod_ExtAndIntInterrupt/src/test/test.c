#include <test.h>
#include <led.h>
#include <uart.h>
#include <tools.h>
#include <memCtrl.h>
#include <log.h>
#include <interrupt.h>
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

/*
 * 1、SRCPND[R/W] 属于中断控制器的一部分，用来指示哪个中断产生了(值为1)，处理后清除对应位
 * 由SRCPND可以判定EINT0~EINT3，大于EINT3的中断需要EINTPEND进一步判定
 * 
 * 2、INTMASK[R/W] 寄存器也是中断控制器的一部分，和中断源的EINTMASK 寄存器类似(1表示屏蔽)，
 * INTMASK 用来在中断控制器上屏蔽中断，EINTMASK用来在中断源处屏蔽中断
 * 
 * 3、INTPND[R/W] 用来指示哪个中断正在被CPU处理
 * 4、INTOFFSET[RO] 用来指示INTPND中哪一位被设置为1，
 * 	  如EINT4~EINT7正在被处理(bit4=1)，则INTOFFSET=4，而INTPND=2^4=16
 *    INTOFFSET为只读的，当INTPND被清除时，INTOFFSET会自动同步
 * 
 * EINTPEND 寄存器和 EINTMASK寄存器都不控制EINT0~EINT3，
 * EINT4~EINT23T都单独有一个控制bit
 *
 * SRCPND 寄存器、 INTPND 和 INTMASK寄存器对于EINT0~EINT3单独控制，
 * EINT4~EINT7、EINT8~EINT23T公用一个控制bit:
 * bit0~bit3分别对应EINT0~EINT3
 * bit4由EINT4~EINT7公用
 * bit5由EINT8~EINT23公用
 */

/* 中断控制器初始化 */
void test_interrupt_controller_init(void)
{
	volatile uint32 regValue = 0x0;

	/* 在中断控制器处使能S2~S4按键中断 */
	regValue = INTMASKr;
	
	regValue &= ~(EINT0_BITSf | EINT2_BITSf | EINT8_23_BITSf);
	regValue |= (INTMASK_INT_ENABLE << EINT0_START_BIT);
	regValue |= (INTMASK_INT_ENABLE << EINT2_START_BIT);
	regValue |= (INTMASK_INT_ENABLE << EINT8_23_START_BIT);

	INTMASKr = regValue;
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
	
	/* 2、设置中断触发方式：双边沿触发 */
	EXTINT0r &= ~(EXTINT0_CON_EINT0_BITSf | EXTINT0_CON_EINT2_BITSf);
	EXTINT1r &= ~EXTINT1_CON_EINT11_BITSf;
	EXTINT2r &= ~EXTINT2_CON_EINT19_BITSf;
	
	EXTINT0r |= (EXTINT_CON_FALL_EDGE_TRIGGER << EXTINT0_CON_EINT0_START_BIT);	/* s2下降沿触发中断 */
	EXTINT0r |= (EXTINT_CON_FALL_EDGE_TRIGGER << EXTINT0_CON_EINT2_START_BIT);	/* s3下降沿触发中断 */
	EXTINT1r |= (EXTINT_CON_FALL_EDGE_TRIGGER << EXTINT1_CON_EINT11_START_BIT);	/* s4下降沿触发中断 */
	EXTINT2r |= (EXTINT_CON_FALL_EDGE_TRIGGER << EXTINT2_CON_EINT19_START_BIT);	/* s5下降沿触发中断 */

	/* 3、设置EINTMASK使能中断 */
	EINTMASKr &= ~(EINTMASK_EINT11_BITSf | EINTMASK_EINT19_BITSf);
	
	EINTMASKr |= (EINTMASK_EINT_ENABLE << EINTMASK_EINT11_START_BIT);
	EINTMASKr |= (EINTMASK_EINT_ENABLE << EINTMASK_EINT19_START_BIT);
}

/* 中断初始化 */
void test_interrupt_init(void)
{
	test_interrupt_key_init();
	test_interrupt_controller_init();
}

/* debug开关测试 */
void test_interrupt_handle_key_s2(void)
{
	print_screen("\r\n ----  key_s2[debug switch]!  ----");
	gSysDebugFlag = !gSysDebugFlag;
	print_screen("\r\n gSysDebugFlag:%d", gSysDebugFlag);
	return;
}

void test_interrupt_handle_key_s3(void)
{
	print_screen("\r\n ----  key_s3!  ----");
	return;
}

void test_interrupt_handle_key_s4(void)
{
	print_screen("\r\n ----  key_s4!  ----");
	return;
}

void test_interrupt_handle_key_s5(void)
{
	print_screen("\r\n ----  key_s5!  ----");
	return;
}

interrupt_drv_t gSocTest_InterruptHandleDrv = {
		/* 中断驱动描述		*/
		"SOC S342440 interruppt TEST drviver",	/* .pDrvDesciption;  */
	
		/* 中断初始化 */
		test_interrupt_init,					/* .interrupt_init; */
	
		/* 外部中断 */
		test_interrupt_handle_key_s2,			/* .interrupt_handle_EXT_INT0 */
		NULL,									/* .interrupt_handle_EXT_INT1 */
		test_interrupt_handle_key_s3,			/* .interrupt_handle_EXT_INT2 */
		NULL,									/* .interrupt_handle_EXT_INT3 */
		NULL,									/* .interrupt_handle_EXT_INT4 */
		NULL,									/* .interrupt_handle_EXT_INT5 */
		NULL,									/* .interrupt_handle_EXT_INT6 */
		NULL,									/* .interrupt_handle_EXT_INT7 */
		NULL,									/* .interrupt_handle_EXT_INT8 */
		NULL,									/* .interrupt_handle_EXT_INT9 */
		NULL,									/* .interrupt_handle_EXT_INT10 */
		test_interrupt_handle_key_s4,			/* .interrupt_handle_EXT_INT11 */
		NULL,									/* .interrupt_handle_EXT_INT12 */
		NULL,									/* .interrupt_handle_EXT_INT13 */
		NULL,									/* .interrupt_handle_EXT_INT14 */
		NULL,									/* .interrupt_handle_EXT_INT15 */
		NULL,									/* .interrupt_handle_EXT_INT16 */
		NULL,									/* .interrupt_handle_EXT_INT17 */
		NULL,									/* .interrupt_handle_EXT_INT18 */
		test_interrupt_handle_key_s5,			/* .interrupt_handle_EXT_INT19 */
		NULL,									/* .interrupt_handle_EXT_INT20 */
		NULL,									/* .interrupt_handle_EXT_INT21 */
		NULL,									/* .interrupt_handle_EXT_INT22 */
		NULL,									/* .interrupt_handle_EXT_INT23 */
	
		/* 内部中断 */
		NULL,									/* .interrupt_handle_INT_CAM */
		NULL,									/* .interrupt_handle_INT_nBATT_FLT */
		NULL,									/* .interrupt_handle_INT_TICK */
		NULL,									/* .interrupt_handle_INT_WDT_AC97 */
		NULL,									/* .interrupt_handle_INT_TIMER0 */
		NULL,									/* .interrupt_handle_INT_TIMER1 */
		NULL,									/* .interrupt_handle_INT_TIMER2 */
		NULL,									/* .interrupt_handle_INT_TIMER3 */
		NULL,									/* .interrupt_handle_INT_TIMER4 */
		NULL,									/* .interrupt_handle_INT_UART2 */
		NULL,									/* .interrupt_handle_INT_LCD */
		NULL,									/* .interrupt_handle_INT_DMA0 */
		NULL,									/* .interrupt_handle_INT_DMA1 */
		NULL,									/* .interrupt_handle_INT_DMA2 */
		NULL,									/* .interrupt_handle_INT_DMA3 */
		NULL,									/* .interrupt_handle_INT_SDI */
		NULL,									/* .interrupt_handle_INT_SPI0 */
		NULL,									/* .interrupt_handle_INT_UART1 */
		NULL,									/* .interrupt_handle_INT_NFCON */
		NULL,									/* .interrupt_handle_INT_USBD */
		NULL,									/* .interrupt_handle_INT_USBH */
		NULL,									/* .interrupt_handle_INT_IIC */
		NULL,									/* .interrupt_handle_INT_UART0 */
		NULL,									/* .interrupt_handle_INT_SPI1 */
		NULL,									/* .interrupt_handle_INT_RTC */
		NULL,									/* .interrupt_handle_INT_ADC */
	
		/* last and NULL please */
		NULL,									/* .interrupt_handle_MAX  */
};
#endif
