#include <uart.h>
#include <tools.h>
#include <nand_flash.h>
#include <log.h>
#include <soc_s3c2440_public.h>

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

void nand_flash_info_display(void)
{
	uint8 dataBuf[5];
	uint8 pageSizeSelect = 0;
	uint8 blkSizeSelect = 0;
	uint8 planeNumSelect = 0;
	uint8 planeSizeSelect = 0;
	
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
	
	/* 根据ID信息计算页信息、块信息 */
	pageSizeSelect = (dataBuf[_NAND_FLASH_ID_4th_] >> _NAND_FLASH_4th_PAGE_SIZE_SHIFT_) & _NAND_FLASH_4th_PAGE_SIZE_MASK_;
	blkSizeSelect = (dataBuf[_NAND_FLASH_ID_4th_] >> _NAND_FLASH_4th_PAGE_SIZE_SHIFT_) & _NAND_FLASH_4th_BLK_SIZE_MASK_;
	planeNumSelect = (dataBuf[_NAND_FLASH_ID_5th_] >> _NAND_FLASH_5th_PLANE_NUM_SHIFT_) & _NAND_FLASH_5th_PLANE_NUM_MASK_;
	planeSizeSelect = (dataBuf[_NAND_FLASH_ID_5th_] >> _NAND_FLASH_5th_PLANE_SIZE_SHIFT_) & _NAND_FLASH_5th_PLANE_SIZE_MASK_;

	/* 禁用片选 */
	nand_flash_select(FALSE);

	/* 打印相关信息 */
	print_screen("\r\n Maker Id code  :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_MAKER_]);
	print_screen("\r\n Device Id code :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_DEVICE_]);
	print_screen("\r\n 3rd Id Byte    :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_3rd_]);
	print_screen("\r\n 4th Id Byte    :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_4th_]);
	print_screen("\r\n 5th Id Byte    :0x");	print_byteHex(dataBuf[_NAND_FLASH_ID_5th_]);
	print_screen("\r\n Page Size      :%dKB", _NAND_FLASH_4th_PAGE_SIZE_BASE_ << pageSizeSelect);
	print_screen("\r\n Block Size     :%dKB", _NAND_FLASH_4th_BLK_SIZE_BASE_ << blkSizeSelect);
	print_screen("\r\n Plane number   :%d",  _NAND_FLASH_5th_PLANE_NUM_BASE_ << planeNumSelect);
	print_screen("\r\n Plane Size     :%dMB",  _NAND_FLASH_5th_PLANE_SIZE_BASE_ << planeSizeSelect);
}

/* NFSTATr寄存器的 "RnB input pin." 不为高电平则处于忙状态 */
void nand_flash_wait_ready(void)
{
	while(NFSTAT_RnB_STAT_READY_STATUS != (NFSTATr & NFSTAT_RnB_STAT_BUSY_BITSf)){
		continue;
	}
}


/*
 * Nand Flash结构
 * 
 * Page   Page Data(2048Bytes)   OOB区(64Bytes)
 * ...           ...                  ...
 *  3   |0 1 2 3 ... 2046 2047 | 2048 ... 2111 |
 *  2   |0 1 2 3 ... 2046 2047 | 2048 ... 2111 |
 *  1   |0 1 2 3 ... 2046 2047 | 2048 ... 2111 |
 *  0   |0 1 2 3 ... 2046 2047 | 2048 ... 2111 |
 *
 * 由于Nand Flash读取的时候可能会发生位反转，OOB存放的是写每一个Page时的校验码
 * 因此读取每一页的时候如果发现位反转，则与校验码不匹配就可以进行数据的校验
 * 校验是Nand Flash硬件为位反转的缺陷而设计的，也是由硬件完成校验计算和修正的，
 * CPU/软件操作不关心OOB，OOB对CPU也不可见。OOB:Out Of Bank
 * 发一次读命令最多读取1页的数据，多出一页的内容需要再次发出读命令
 * 注意：NFDATAr寄存器在发出读命令、地址、以及确认命令之后，再去读，
 * 每读一次，NFDATAr寄存器返回当前地址数据，并且自动加载下一个地址数据
 */
void  nand_flash_data_read(uint32 addr, uint8 * buf, uint32 len)
{
	uint32 index = 0;
	uint32 page = addr / NAND_FLASH_PAGE_SIZE;
	uint32 offset = addr % NAND_FLASH_PAGE_SIZE;

	/* 启用片选 */
	nand_flash_select(TRUE);

	while (index < len)
	{
		/* 发出读命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_CMD__);
		
		/* 发出地址,3Byte offset Addr，2Byte Page Addr */

		/* offset Addr */
		nand_flash_addr_byte(offset & 0xFF);
		nand_flash_addr_byte((offset >> 8) & 0xFF);
		
		/* Page Addr */
		nand_flash_addr_byte(page & 0xFF);
		nand_flash_addr_byte((page >> 8) & 0xFF);
		nand_flash_addr_byte((page >> 16) & 0xFF);
		
		/* 发出读数据地址确认命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_ADDR_CMD__);
		
		/* 等待数据 */
		nand_flash_wait_ready();

		/* 读page的所有数据数据 */
		while ((offset < NAND_FLASH_PAGE_SIZE) && (index < len))
		{
			buf[index] = nand_flash_data_byte();
			offset++;
			index++;
		}

		/* 一个page读取完毕读取下一个Page */
		offset = 0;
		page++;
	}

	/* 禁用片选 */
	nand_flash_select(FALSE);
}

void nand_flash_oob_read(uint32 addr, uint8 * buf, uint32 len)
{
	uint32 index = 0;
	uint32 page = addr / NAND_FLASH_OOB_SIZE;
	uint32 offset = (addr % NAND_FLASH_OOB_SIZE) + NAND_FLASH_PAGE_SIZE;

	/* 启用片选 */
	nand_flash_select(TRUE);

	while (index < len)
	{
		/* 发出读命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_CMD__);
		
		/* 发出地址,3Byte offset Addr，2Byte Page Addr */

		/* offset Addr */
		nand_flash_addr_byte(offset & 0xFF);
		nand_flash_addr_byte((offset >> 8) & 0xFF);
		
		/* Page Addr */
		nand_flash_addr_byte(page & 0xFF);
		nand_flash_addr_byte((page >> 8) & 0xFF);
		nand_flash_addr_byte((page >> 16) & 0xFF);
		
		/* 发出读数据地址确认命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_ADDR_CMD__);
		
		/* 等待数据 */
		nand_flash_wait_ready();

		/* 读page的所有OOB数据 */
		while ((offset < NAND_FLASH_PAGE_END) && (index < len))
		{
			buf[index] = nand_flash_data_byte();
			offset++;
			index++;
		}

		/* 一个page读取完毕读取下一个Page */
		offset = NAND_FLASH_PAGE_SIZE;
		page++;
	}

	/* 禁用片选 */
	nand_flash_select(FALSE);
}

/* 获取块个数和每个块的页个数 */
static void nand_flash_get_blk_page_num(uint32 *blkNum, uint32* pageNum)
{
	uint8 dataBuf[5];
	uint8 pageSizeSelect = 0;
	uint8 blkSizeSelect = 0;
	uint8 planeNumSelect = 0;
	uint8 planeSizeSelect = 0;
	uint32 totalSize = 0;
	uint32 blkSize= 0;
	uint32 pageSize= 0;

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
	
	/* 根据ID信息计算页信息、块信息 */
	pageSizeSelect = (dataBuf[_NAND_FLASH_ID_4th_] >> _NAND_FLASH_4th_PAGE_SIZE_SHIFT_) & _NAND_FLASH_4th_PAGE_SIZE_MASK_;
	blkSizeSelect = (dataBuf[_NAND_FLASH_ID_4th_] >> _NAND_FLASH_4th_PAGE_SIZE_SHIFT_) & _NAND_FLASH_4th_BLK_SIZE_MASK_;
	planeNumSelect = (dataBuf[_NAND_FLASH_ID_5th_] >> _NAND_FLASH_5th_PLANE_NUM_SHIFT_) & _NAND_FLASH_5th_PLANE_NUM_MASK_;
	planeSizeSelect = (dataBuf[_NAND_FLASH_ID_5th_] >> _NAND_FLASH_5th_PLANE_SIZE_SHIFT_) & _NAND_FLASH_5th_PLANE_SIZE_MASK_;

	/* 禁用片选 */
	nand_flash_select(FALSE);	

	/* 计算总大小，根据总大下和块大小、页大小计算块个数和每个块里页个数 */
	totalSize = _NAND_FLASH_PLANE_CELL_ * (_NAND_FLASH_5th_PLANE_SIZE_BASE_ << planeSizeSelect);
	blkSize = _NAND_FLASH_BLOCK_CELL_ * (_NAND_FLASH_4th_BLK_SIZE_BASE_ << blkSizeSelect);
	pageSize = _NAND_FLASH_PAGE_CELL_ * (_NAND_FLASH_4th_PAGE_SIZE_BASE_ << pageSizeSelect);

	/* 计算块个数和每个块的页个数 */
	*blkNum = tool_get_binary_multiple(totalSize, blkSize);
	*pageNum = tool_get_binary_multiple(blkSize, pageSize);
}

/* 检查某一页是否坏掉 */
static BOOL nand_flash_bad_page_check(uint32 pageIdx)
{
	uint8 buf[NAND_FLASH_OOB_SIZE];
	uint32 start = 0;

	set_buffer(buf, 0, NAND_FLASH_OOB_SIZE);
	start = pageIdx * NAND_FLASH_OOB_SIZE;
	nand_flash_oob_read(start, buf, NAND_FLASH_OOB_SIZE);
	if (buf[NAND_FLASH_OOB_CHECK_POS_OFFSET] == NAND_FLASH_OOB_CHECK_VALUE)
	{
		return FALSE;
	}
	
	return TRUE;
}

/* 检查指定块是否坏块 */
static BOOL nand_flash_bad_blk_check(uint32 blkIdx, uint32 pageNum)
{
	uint32 pageIndex = 0;
	BOOL isBadPage = FALSE;
	BOOL isBadBlk = FALSE;
	uint32 blkStartPage = 0;

	blkStartPage = blkIdx * pageNum;
	for (pageIndex = 0; pageIndex < pageNum; pageIndex++)
	{
		isBadPage = nand_flash_bad_page_check(blkStartPage + pageIndex);
		if (isBadPage) {
			//print_screen("\r\n Block %d page %d is bad page.", blkIdx, pageIndex);
			isBadBlk = TRUE;
			break;
		}
	}
	
	return isBadBlk;
}

/* flash坏块检查 */
void nand_flash_check(void)
{
	uint32 blkNum = 0;
	uint32 pageNum = 0;
	uint32 blkIndex = 0;
	BOOL isBadBlk = FALSE;
	BOOL isBad = FALSE;

	nand_flash_get_blk_page_num(&blkNum, &pageNum);
	print_screen("\r\n Total have %d blocks, Per block have %d pages.", blkNum, pageNum);
	
	for (blkIndex = 0; blkIndex < blkNum; blkIndex++)
	{
		isBadBlk = nand_flash_bad_blk_check(blkIndex, pageNum);
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
