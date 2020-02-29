#include <tools.h>
#include <soc_s3c2440_public.h>

/* 
 * 用S3C2440的SPI控制器操作SPI，
 * 即GPIO相关寄存器bit配置为SPI功能引脚
 * 而不是认为片选、单bit单bit一一处理的输入输出控制引脚
 */

/* SPI写操作 */
void spi_send_byte(uint8 data)
{
    while (!(SPSTA1r & 0x1));
    SPTDAT1r = data;    
}

/* SPI读操作 */
uint8 spi_receive_byte(void)
{
    SPTDAT1r = 0xff;    
    while (!(SPSTA1r & 0x1));
    return SPRDAT1r;    
}

/* SPI功能引脚设置等 */
void spi_function_pin_init(void)
{
	/*
	 * 外部接插件连接关系：spi_jz2440.pdf
     * OLED_CSn		:Output (GPF1)
     * FLASH_CSn	:Output (GPG2)
     * OLED_DC		:Output (GPG4) Data/Command引脚，高电平数据，低电平命令
     * SPIMISO		:	(GPG5)
     * SPIMOSI		:	(GPG6)
	 * SPICLK		:	(GPG7)
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
			| (GPGCON_GPG5_SPIMISO1 << GPGCON_GPG5_CONF_START) \
			| (GPGCON_GPG6_SPIMOSI1 << GPGCON_GPG6_CONF_START) \
			| (GPGCON_GPG7_SPICLK1 << GPGCON_GPG7_CONF_START));		/* GPG 5/6/7 SPI功能引脚 */

	GPGDATr |= GPGDAT_GPG2_DATA_BITSf;							/* FLASH片选禁用 */
}

static void spi_controller_init(void)
{
    /* OLED : 100ns, 10MHz; SPI FLASH : 104MHz. 取最小10MHz
     * 10 = 50 / 2 / (Prescaler value + 1)
     * Prescaler value = 1.5 = 2
     * Baud rate = 50/2/3=8.3MHz
     */
    SPPRE0r = 2;
    SPPRE1r = 2;

    /* [6:5] : 00, polling mode
    * [4]   : 1 = enable 
    * [3]   : 1 = master
    * [2]   : 0
    * [1]   : 0 = format A
    * [0]   : 0 = normal mode
    */
    SPCON0r = (1 << 4) | (1 << 3);
    SPCON1r = (1 << 4) | (1 << 3);
}

void spi_init(void)
{
	/* SPI功能引脚初始化 */
	spi_function_pin_init();

	/* SPI控制器初始化 */
	spi_controller_init();

	print_screen("\r\n SPI controller init succeed!");
}

