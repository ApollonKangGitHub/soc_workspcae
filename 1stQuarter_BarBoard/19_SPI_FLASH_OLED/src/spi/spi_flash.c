#include <tools.h>
#include <gpio_spi.h>
#include <soc_s3c2440_public.h>

#define SPI_FLASH_READ_ID_CMD			(0x90)
#define SPI_FLASH_READ_ID_ADDR			(0x0)

#define SPI_FLASH_WR_ENABL_CMD			(0x06)
#define SPI_FLASH_WR_DISABLE_CMD		(0x04)

#define SPI_FLASH_WR_STATUS_REG_CMD		(0x01)
#define SPI_FLASH_RD_STATUS_REG1_CMD	(0x05)
#define SPI_FLASH_RD_STATUS_REG2_CMD	(0x35)

#define SPI_FLASH_EARSE_SECTOR_CMD		(0x20)
#define SPI_FLASH_WRITE_CMD				(0x02)
#define SPI_FLASH_READ_CMD				(0x03)

/* 状态寄存器写保护标志位，清零为解除写保护 */
#define SPI_FLASH_STATUS_REG1_PROTECT_BIT	(7)
#define SPI_FLASH_STATUS_REG2_PROTECT_BIT	(0)
#define SPI_FLASH_DATA_PROTECT_REG1_BP2_BIT	(4)
#define SPI_FLASH_DATA_PROTECT_REG1_BP1_BIT	(3)
#define SPI_FLASH_DATA_PROTECT_REG1_BP0_BIT	(2)
#define SPI_FLASH_STATUS_REG1_BUSY_BIT		(0)
#define SPI_FLASH_DATA_PROTECT_REG2_CMP_BIT	(6)

#define SPI_FLASH_CS_SELECT				(0)
#define SPI_FLASH_CS_DESELECT			(1)

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

/* 写使能 */
static void spi_flash_wr_enable(BOOL enable)
{
	if (enable)
	{
		spi_flash_set_cs(SPI_FLASH_CS_SELECT);
		spi_flash_send_byte(SPI_FLASH_WR_ENABL_CMD);
		spi_flash_set_cs(SPI_FLASH_CS_DESELECT);
	}
	else
	{
		spi_flash_set_cs(SPI_FLASH_CS_SELECT);
		spi_flash_send_byte(SPI_FLASH_WR_DISABLE_CMD);
		spi_flash_set_cs(SPI_FLASH_CS_DESELECT);
	}
}

/* 读状态寄存器1 */
static uint8 spi_flash_status_reg1_get(void)
{
	uint8 reg1 = 0x0;
	spi_flash_set_cs(SPI_FLASH_CS_SELECT);
	spi_flash_send_byte(SPI_FLASH_RD_STATUS_REG1_CMD);
	reg1 = spi_flash_receive_byte();
	spi_flash_set_cs(SPI_FLASH_CS_DESELECT);

	return reg1;
}

/* 读状态寄存器2 */
static uint8 spi_flash_status_reg2_get(void)
{
	uint8 reg2 = 0x0;
	spi_flash_set_cs(SPI_FLASH_CS_SELECT);
	spi_flash_send_byte(SPI_FLASH_RD_STATUS_REG2_CMD);
	reg2 = spi_flash_receive_byte();
	spi_flash_set_cs(SPI_FLASH_CS_DESELECT);

	return reg2;
}

/* 写数据（擦除）忙等待 */
static void spi_flash_wr_wait_when_busy(void)
{
	volatile uint8 reg1 = 0;
	reg1 = spi_flash_status_reg1_get();
	
	while(reg1 & (0x1 << SPI_FLASH_STATUS_REG1_BUSY_BIT)){
		reg1 = spi_flash_status_reg1_get();
	}
}

/* 修改状态寄存器 */
static void spi_flash_write_status_reg(uint8 reg1, uint8 reg2)
{
	spi_flash_wr_enable(TRUE);
	spi_flash_set_cs(SPI_FLASH_CS_SELECT);
	spi_flash_send_byte(SPI_FLASH_WR_STATUS_REG_CMD);
	spi_flash_send_byte(reg1);
	spi_flash_send_byte(reg2);
	spi_flash_set_cs(SPI_FLASH_CS_DESELECT);	

	spi_flash_wr_wait_when_busy();
}

/* 清除状态寄存器的保护 */
static void spi_flash_clean_protect_for_status_reg(void)
{
	uint8 reg1, reg2;

	reg1 = spi_flash_status_reg1_get();
	reg2 = spi_flash_status_reg2_get();

	reg1 &= ~(0x1 << SPI_FLASH_STATUS_REG1_PROTECT_BIT);
	reg2 &= ~(0x1 << SPI_FLASH_STATUS_REG2_PROTECT_BIT);

	spi_flash_write_status_reg(reg1, reg2);
}

/* 清除数据区间的的保护 */
static void spi_flash_clean_protect_for_data(void)
{
	uint8 reg1, reg2;

	reg1 = spi_flash_status_reg1_get();
	reg2 = spi_flash_status_reg2_get();

	/* cmp=0、bp2~bp0=0 */
	reg1 &= ~((0x1 << SPI_FLASH_DATA_PROTECT_REG1_BP2_BIT)
			| (0x1 << SPI_FLASH_DATA_PROTECT_REG1_BP1_BIT)
			| (0x1 << SPI_FLASH_DATA_PROTECT_REG1_BP0_BIT));
	reg2 &= ~(0x1 << SPI_FLASH_DATA_PROTECT_REG2_CMP_BIT);

	spi_flash_write_status_reg(reg1, reg2);
}

/* 擦除扇区（4K） */
void spi_flash_earse(uint32 addr)
{
	spi_flash_wr_enable(TRUE);
	spi_flash_set_cs(SPI_FLASH_CS_SELECT);
	spi_flash_send_byte(SPI_FLASH_EARSE_SECTOR_CMD);
	spi_flash_send_addr(addr);
	spi_flash_set_cs(SPI_FLASH_CS_DESELECT);	
	
	spi_flash_wr_wait_when_busy();
}

/* 写数据 */
void spi_flash_write(uint32 addr, uint8 * buf, int len)
{
	int i = 0;

	spi_flash_wr_enable(TRUE);
	spi_flash_set_cs(SPI_FLASH_CS_SELECT);
	spi_flash_send_byte(SPI_FLASH_WRITE_CMD);
	spi_flash_send_addr(addr);

	/* 发送出写命令和写地址之后，按字节发送数据 */
	for (i = 0; i < len; i++)
	{
		spi_flash_send_byte(buf[i]);
	}
	
	spi_flash_set_cs(SPI_FLASH_CS_DESELECT);	
	
	spi_flash_wr_wait_when_busy();
}

/* 读数据 */
void spi_flash_read(uint32 addr, uint8 * buf, int len)
{
	int i = 0;

	spi_flash_wr_enable(TRUE);
	spi_flash_set_cs(SPI_FLASH_CS_SELECT);
	spi_flash_send_byte(SPI_FLASH_READ_CMD);
	spi_flash_send_addr(addr);

	/* 发送出读命令和读地址之后，按字节读取数据 */
	for (i = 0; i < len; i++)
	{
		buf[i] = spi_flash_receive_byte();
	}
	
	spi_flash_set_cs(SPI_FLASH_CS_DESELECT);	
}


/* SPI flash初始化 */
void spi_flash_init(void)
{
	spi_flash_clean_protect_for_status_reg();
	spi_flash_clean_protect_for_data();
}

