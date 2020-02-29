#include <tools.h>
#include <gpio_spi.h>
#include <soc_s3c2440_public.h>

#define SPI_FLASH_READ_ID_CMD	(0x90)
#define SPI_FLASH_READ_ID_ADDR	(0x0)

#define SPI_FLASH_CS_SELECT		(0)
#define SPI_FLASH_CS_DESELECT	(1)


/* 片选使能与禁用 */
static void spi_flash_set_cs(uint8 val)
{
	if (SPI_FLASH_CS_SELECT == val)
	{
		GPGDATr &= ~GPGDAT_GPG2_DATA_BITSf;
	}
	else 
	{
		GPGDATr |= GPGDAT_GPG2_DATA_BITSf;
	}
}

/* 发送字节 */
static void spi_flash_send_byte(uint8 val)
{
	spi_send_byte(val);
}

/* 	接收读取字节 */
static uint8 spi_flash_receive_byte(void)
{
	return spi_receive_byte();
}

/* 发送3byte的地址 */
static void spi_flash_send_addr(uint32 addr)
{
	spi_flash_send_byte((addr >> 16) & 0xff);
	spi_flash_send_byte((addr >> 8) & 0xff);
	spi_flash_send_byte(addr & 0xff);
}

/* 读取厂家ID和设备ID */
void spi_flash_read_id(uint8 * pMid, uint8 * pDid)
{
	spi_flash_set_cs(SPI_FLASH_CS_SELECT);
	spi_flash_send_byte(SPI_FLASH_READ_ID_CMD);
	spi_flash_send_addr(SPI_FLASH_READ_ID_ADDR);
	*pMid = spi_flash_receive_byte();
	*pDid = spi_flash_receive_byte();
	spi_flash_set_cs(SPI_FLASH_CS_DESELECT);
}

