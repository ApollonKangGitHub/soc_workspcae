/*
 * Description:
 *     操作UART
 * Time:2019年4月7日11:16:19
 * Author:Kangruojin
 * Version:1.0
*/

#include <uart.h>
#include <soc_s3c2440_public.h>

void uart_init()
{
	int delay = 1000;
	
	/* 设置多功能引脚用于串口(TXD0/GPH2、RXD0/GPH3) */
	GPHCONr &= ~(GPHCON_GPH2_CONF_BITSf | GPHCON_GPH3_CONF_BITSf);
	GPHCONr |= ((GPHCON_GPH2_TXD0 << GPIOCON_PIN2_START_BIT) | (GPHCON_GPH3_RXD0 << GPIOCON_PIN3_START_BIT));


	/* 设置GPHUP上拉,默认是0:上拉,所以可以不用重复设置 */
	GPHUPr &= ~(GPHDAT_GPH2_DATA_BITSf | GPHDAT_GPH3_DATA_BITSf);

	/* 设置UCON0寄存器，先清空，然后选择PCLK（默认0b'00）、TX和RX模式选择中断请求/poll模式 */
	UCON0r &= ~(UCON_CLOCK_SECTION_BITSf | UCON_TRANS_MODE_BITSf | UCON_RECV_MODE_BITSf);
	UCON0r |= ((UCON_TRANS_MODE_INTREQ_OR_POLL << UCON_TRANS_MODE_START) | (UCON_RECV_MODE_INTREQ_OR_POLL << UCON_RECV_MODE_START));


	/* 
	 * 设置波特率,初始化波特率设为115200,后期考虑可切换波特率
	 * 通过将源时钟（PCLK，FCLK / n或UEXTCLK）除以16
	 * 和UART波特率除数寄存器:UBRDIVn中指定的16位除数来生成波特率时钟
	 * UBRDIVn = (int)( UART clock / ( buad rate x 16) ) –1
	 * UBRDIVn = (int)((50,000,000)/(115200 x 16)) - 1 = 26.12673611... = 26
	 */
	UBRDIV0r &= 0x0;		/* 只有一个UBRDIV字段,且默认是初始化为0的 */
	UBRDIV0r |= 26;			/* when PCLK=50MHz, config UBRDIV0 is 26, then BaudRate is 115200 */
	
	/* 
	 * 设置数据格式:
	 * 无校验位（0）、非红外模式（0）、1个停止位（0）、8个数据位（0b'11）
	 * (这是传输率最大化的配置) 
	 */
	ULCON0r &= ~(ULCON_INFRARED_MODE_BITSf | ULCON_NUM_OF_STOP_BITSf | ULCON_WORD_LEN_BITSf | ULCON_PARITY_MODE_BITSf); 		/* 默认初始化为0 */
	ULCON0r |= ((ULCON_NUM_OF_STOP_1 << ULCON_NUM_OF_STOP_START) | (ULCON_WORD_LEN_8 << ULCON_WORD_LEN_START));		/* 设置停止位和数据位大小 */
	
	/* 其他数据格式 */
	while(delay--);
}

/* 非FIFO buffer版 */
void uart_putchar(int ch)
{
	/* 先确保移位寄存器已经发送完毕，为empty */
	while(!_TEST_BIT_IS_ONE(UTRSTAT0r, UTRSTAT_TRANS_EMTPY_BITSf));

	/* 当移位寄存器发送完毕后，就可以给发送数据寄存器中放数据了 */
	UTXH0Lr = (unsigned char)(ch);
}

/* 非FIFO buffer版 */
int uart_getchar(void)
{
	/* 先确保接收buffer不为空，才可以去取buffer的值 */
	while(!_TEST_BIT_IS_ONE(UTRSTAT0r, UTRSTAT_RECV_BUFF_EMPTY_BITSf));
	/* 从接收buffer中取值，并返回 */
	return URXH0Lr;      
}

int uart_puts(const char *s)
{
	const char * ps = s;
	int i = 0;
	while (ps[i])
	{
		uart_putchar(ps[i]);
		i++;
	}
	return i;
}

int uart_puts_test1(void)
{
	uart_putchar('\r');
	uart_putchar('\n');
	uart_putchar('A');
	uart_putchar('A');
	uart_putchar('A');
}

int uart_puts_test2(void)
{
	uart_putchar('\r');
	uart_putchar('\n');
	uart_putchar('B');
	uart_putchar('B');
	uart_putchar('B');
}

int uart_puts_test3(void)
{
	uart_putchar('\r');
	uart_putchar('\n');
	uart_putchar('C');
	uart_putchar('C');
	uart_putchar('C');
}

