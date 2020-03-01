#include <tools.h>
#include <soc_s3c2440_public.h>

#define GPIO_SPI_CLK_HIGH	(1)
#define GPIO_SPI_CLK_LOW	(0)

/* 用GPIO模式SPI协议 */

/* 设置时钟SPICLK */
static void spi_set_clk(uint8 val)
{
    if (val)
        GPGDATr |= GPGDAT_GPG7_DATA_BITSf;
    else
        GPGDATr &= ~GPGDAT_GPG7_DATA_BITSf;
}

/* 设置数据SPIMOSI输出（发送） */
static void spi_set_do_bit(uint8 val)
{
    if (val)
        GPGDATr |= GPGDAT_GPG6_DATA_BITSf;
    else
        GPGDATr &= ~GPGDAT_GPG6_DATA_BITSf;
}

/* 获取数据SPIMISO输入（接收） */
static BOOL spi_get_di_bit(void)
{
    if (GPGDATr & GPGDAT_GPG5_DATA_BITSf)
        return TRUE;
    else
        return FALSE;
}

/* SPI写操作 */
void spi_send_byte(uint8 data)
{
	uint8 bit = 0;
	
	for (bit = 0; bit < 8; bit++)
	{
		spi_set_clk(GPIO_SPI_CLK_LOW);
		spi_set_do_bit(data & 0x80);
		spi_set_clk(GPIO_SPI_CLK_HIGH);	/* 产生上升沿 */
		data <<= 1;
	}
}

/* SPI读操作 */
uint8 spi_receive_byte(void)
{
	uint8 bit = 0;
	uint8 val = 0;
	
	for (bit = 0; bit < 8; bit++)
	{
		val <<= 1;
		spi_set_clk(GPIO_SPI_CLK_LOW);
		if (spi_get_di_bit()) {
			val |= 0x1;
		}
		spi_set_clk(GPIO_SPI_CLK_HIGH);	/* 产生上升沿 */
	}

	return val;
}


void gpio_spi_init(void)
{
	/*
	 * 外部接插件连接关系：spi_jz2440.pdf
     * OLED_CSn		:Output (GPF1)
     * FLASH_CSn	:Output (GPG2)
     * OLED_DC		:Output (GPG4) Data/Command引脚，高电平数据，低电平命令
     * SPIMISO		:Input	(GPG5)
     * SPIMOSI		:Output	(GPG6)
	 * SPICLK		:Output	(GPG7)
     */

    GPFCONr &= ~GPFCON_GPF1_CONF_BITSf;							/* clear */
    GPFCONr |= GPFCON_OUTPUT << GPIOCON_PIN1_START_BIT;			/* GPF1 OLED_CSn output */
    GPFDATr |= GPGDAT_GPG1_DATA_BITSf;							/* OLED片选禁用 */
     
	GPGCONr &= ~(GPGCON_GPG2_CONF_BITSf \
			| GPGCON_GPG4_CONF_BITSf \
			| GPGCON_GPG5_CONF_BITSf \
			| GPGCON_GPG6_CONF_BITSf \
			| GPGCON_GPG7_CONF_BITSf);							/* clear */
	
 	GPGCONr |= ((GPFCON_OUTPUT << GPGCON_GPG2_CONF_START) \
			| (GPFCON_OUTPUT << GPGCON_GPG4_CONF_START) \
			| (GPFCON_OUTPUT << GPGCON_GPG6_CONF_START) \
			| (GPFCON_OUTPUT << GPGCON_GPG7_CONF_START));		/* GPG 2/4/6/7 output pin */

	GPGDATr |= GPGDAT_GPG2_DATA_BITSf;							/* FLASH片选禁用 */
}

void spi_init(void)
{
	/* GPIO模拟SPI引脚初始化 */
	gpio_spi_init();

	print_screen("\r\n SPI gpio init succeed!");
}

