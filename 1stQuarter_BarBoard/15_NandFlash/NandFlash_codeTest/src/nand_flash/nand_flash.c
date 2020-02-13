#include <uart.h>
#include <tools.h>
#include <nand_flash.h>
#include <log.h>
#include <soc_s3c2440_public.h>

void nand_flash_init(void)
{
	/* 
	 * 设置NADN FLASH时序,在HCLK为100MHZ的前提下（stat.S）
	 * 即HCLK的周期为10ns，因此TWRPH0 取最小保证值1，TWRPH1取最小保证0即可，
	 * 由 Nand Flash K9F2G08U0C 的时序图可知，TACLS设置为0即可
	 * 对应 SOC_S3C2440 寄存器 NFCONF 的配置:
	 * TACLS : [13:12] : CLE & ALE duration setting value (0~3), Duration = HCLK x TACLS
	 * TWRPH0: [10:8]  : TWRPH0 duration setting value (0~7) Duration = HCLK x ( TWRPH0 + 1 ) 
	 * TWRPH1: [6:4]   : TWRPH1 duration setting value (0~7) Duration = HCLK x ( TWRPH1 + 1 ) 
	 */
	NFCONFr = (NAND_K9F2G08U0C_TACLS << 12) 
			| (NAND_K9F2G08U0C_TWRPH0 << 8) 
			| (NAND_K9F2G08U0C_TWRPH1 << 4);


	/* 初始化ECC，禁止片选，使能Nand Flash控制器 */
	NFCONTr = (NAND_INIT_ECC_DECODER_ENCODER << 4) 
			| (NAND_nFCE_CTRL_DISABLE << 1) 
			| (NAND_NAND_CONTOLLER_ENABLE << 0);
}


static void nand_flash_select(BOOL enable)
{
	if (enable)
	{
		/* 使能片选 */
		NFCONTr &= ~(NAND_nFCE_CTRL_DISABLE << 1);
	}
	else
	{
		NFCONTr |= (NAND_nFCE_CTRL_DISABLE << 1);
	}
}

static void nand_flash_cmd_byte(uint8 cmd)
{
	uint8 delay = 10;
	volatile uint8 i = 0;
	
	NFCMDr = cmd; 
	for(i = 0; i < delay; i++);	/* 延时保证数据稳定 */
}

static void nand_flash_addr_byte(uint8 addr)
{
	uint8 delay = 10;
	volatile uint8 i = 0;
	
	NFADDRr = addr; 
	for(i = 0; i < delay; i++);	/* 延时保证数据稳定 */
}

/* 发送flash地址 */
static uint8 nand_flash_data_byte(void)
{
	volatile uint8 data = 0x0;
	data = NFDATAr;
	return data;
}

void nand_flash_info_display(void)
{
	uint8 dataBuf[5];
	uint8 pageSizeSelect = 0;
	uint8 blkSizeSelect = 0;
	
	/* 使能片选 */
	nand_flash_select(TRUE);

	/* 写命令：读ID */
	nand_flash_cmd_byte(__NAND_FLASH_READ_ID_CMD__);

	/* 写地址 */
	nand_flash_addr_byte(__NAND_FLASH_READ_ID_ADDR__);

	/* 读数据 */
	set_buffer(dataBuf, 0, sizeof(dataBuf));
	dataBuf[_NAND_FLASH_ID_MAKER_] = nand_flash_data_byte();
	dataBuf[_NAND_FLASH_ID_DEVICE_] = nand_flash_data_byte();
	dataBuf[_NAND_FLASH_ID_3rd_] = nand_flash_data_byte();
	dataBuf[_NAND_FLASH_ID_4th_] = nand_flash_data_byte();
	dataBuf[_NAND_FLASH_ID_5th_] = nand_flash_data_byte();

	/* 禁用片选 */
	nand_flash_select(TRUE);

	/* 打印ID信息 */
	print_screen("\r\n Maker Id code  :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_MAKER_]);
	print_screen("\r\n Device Id code :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_DEVICE_]);
	print_screen("\r\n 3rd Id Byte    :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_3rd_]);
	print_screen("\r\n 4th Id Byte    :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_4th_]);
	print_screen("\r\n 5th Id Byte    :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_5th_]);

	/* 根据ID信息计算并打印页信息、块信息 */
	pageSizeSelect = (dataBuf[_NAND_FLASH_ID_4th_] >> _NAND_FLASH_4th_PAGE_SIZE_SHIFT_) & _NAND_FLASH_4th_PAGE_SIZE_MASK_;
	blkSizeSelect = (dataBuf[_NAND_FLASH_ID_4th_] >> _NAND_FLASH_4th_PAGE_SIZE_SHIFT_) & _NAND_FLASH_4th_BLK_SIZE_MASK_;

	print_screen("\r\n Page Size      :%d KB", _NAND_FLASH_4th_PAGE_SIZE_BASE_ << pageSizeSelect);
	print_screen("\r\n Block Size     :%d KB", _NAND_FLASH_4th_BLK_SIZE_BASE_ << blkSizeSelect);	
}
