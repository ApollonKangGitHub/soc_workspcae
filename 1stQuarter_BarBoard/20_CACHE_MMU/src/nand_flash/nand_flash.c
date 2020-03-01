#include <uart.h>
#include <tools.h>
#include <nand_flash.h>
#include <log.h>
#include <soc_s3c2440_public.h>

#define NAND_FLASH_READ_BYTE()		(NFDATAr)
#define NAND_FLASH_WRITE_BYTE(val)	(NFDATAr = (volatile uint8)(val))
#define	NAND_FALSH_SELECT()			(NFCONTr &= ~(NAND_nFCE_CTRL_DISABLE << 1))
#define NAND_FALSH_DESELECT()		(NFCONTr |= (NAND_nFCE_CTRL_DISABLE << 1))

/* NFSTATr寄存器的 "RnB input pin." 不为高电平则处于忙状态 */
#define NAND_FALSH_WAIT_READY(void) \
	while(NFSTAT_RnB_STAT_READY_STATUS != (NFSTATr & NFSTAT_RnB_STAT_BUSY_BITSf));

static void nand_flash_cmd_byte(uint8 cmd)
{
	volatile uint8 i = 0;
	
	NFCMDr = cmd; 
	for(i = 0; i < 10; i++);	/* 延时保证数据稳定 */
}

static void nand_flash_addr_byte(uint8 addr)
{
	volatile uint8 i = 0;
	
	NFADDRr = addr; 
	for(i = 0; i < 10; i++);	/* 延时保证数据稳定 */
}

/* 发出地址(读操作、写操作) */
static void nand_flash_addr_page_offset(uint32 page, uint32 offset)
{
	/* 2Byte offset Addr,3Byte Page Addr */
	
	/* offset Addr */
	nand_flash_addr_byte(offset & 0xFF);
	nand_flash_addr_byte((offset >> 8) & 0xFF);
	
	/* Page Addr */
	nand_flash_addr_byte(page & 0xFF);
	nand_flash_addr_byte((page >> 8) & 0xFF);
	nand_flash_addr_byte((page >> 16) & 0xFF);
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


/* 获取块个数和每个块的页个数 */
void nand_flash_get_mem_info(nand_flash_info_t * info)
{
	uint8 * pData = NULL;
	uint8 pageSizeSelect = 0;
	uint8 blkSizeSelect = 0;
	uint8 planeNumSelect = 0;
	uint8 planeSizeSelect = 0;
	pData = (uint8 *)(info->idInfo);

	/* 使能片选 */
	NAND_FALSH_SELECT();

	/* 写命令：读ID */
	nand_flash_cmd_byte(__NAND_FLASH_READ_ID_CMD__);

	/* 写地址 */
	nand_flash_addr_byte(__NAND_FLASH_READ_ID_ADDR__);

	/* 读数据 */
	set_buffer(pData, 0, sizeof(pData));
	pData[_NAND_FLASH_ID_MAKER_] = NAND_FLASH_READ_BYTE();
	pData[_NAND_FLASH_ID_DEVICE_] = NAND_FLASH_READ_BYTE();
	pData[_NAND_FLASH_ID_3rd_] = NAND_FLASH_READ_BYTE();
	pData[_NAND_FLASH_ID_4th_] = NAND_FLASH_READ_BYTE();
	pData[_NAND_FLASH_ID_5th_] = NAND_FLASH_READ_BYTE();
	
	/* 根据ID信息计算页信息、块信息 */
	pageSizeSelect = (pData[_NAND_FLASH_ID_4th_] >> _NAND_FLASH_4th_PAGE_SIZE_SHIFT_) & _NAND_FLASH_4th_PAGE_SIZE_MASK_;
	blkSizeSelect = (pData[_NAND_FLASH_ID_4th_] >> _NAND_FLASH_4th_PAGE_SIZE_SHIFT_) & _NAND_FLASH_4th_BLK_SIZE_MASK_;
	planeNumSelect = (pData[_NAND_FLASH_ID_5th_] >> _NAND_FLASH_5th_PLANE_NUM_SHIFT_) & _NAND_FLASH_5th_PLANE_NUM_MASK_;
	planeSizeSelect = (pData[_NAND_FLASH_ID_5th_] >> _NAND_FLASH_5th_PLANE_SIZE_SHIFT_) & _NAND_FLASH_5th_PLANE_SIZE_MASK_;

	/* 禁用片选 */
	NAND_FALSH_DESELECT();	

	/* 计算总大小，根据总大下和块大小、页大小计算块个数和每个块里页个数 */
	info->planeNum = _NAND_FLASH_5th_PLANE_NUM_BASE_ << planeNumSelect;
	info->planeSize = _NAND_FLASH_PLANE_CELL_ * (_NAND_FLASH_5th_PLANE_SIZE_BASE_ << planeSizeSelect);
	info->blkSize = _NAND_FLASH_BLOCK_CELL_ * (_NAND_FLASH_4th_BLK_SIZE_BASE_ << blkSizeSelect);
	info->pageSize = _NAND_FLASH_PAGE_CELL_ * (_NAND_FLASH_4th_PAGE_SIZE_BASE_ << pageSizeSelect);
	info->totalSize = info->planeNum * info->planeSize;
		
	/* 计算块个数和每个块的页个数 */
	info->blkNum = info->totalSize / info->blkSize;
	info->pagePerBlk = info->blkSize / info->pageSize;
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
	NAND_FALSH_SELECT();

	while (index < len)
	{
		/* 发出读命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_CMD__);

		/* 发出页地址和偏移地址 */
		nand_flash_addr_page_offset(page, offset);
		
		/* 发出读数据地址确认命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_ADDR_CMD__);
		
		/* 等待数据 */
		NAND_FALSH_WAIT_READY();

		/* 读page的所有数据数据 */
		while ((offset < NAND_FLASH_PAGE_SIZE) && (index < len))
		{
			buf[index] = NAND_FLASH_READ_BYTE();
			offset++;
			index++;
		}

		/* 一个page读取完毕读取下一个Page */
		offset = 0;
		page++;
	}

	/* 禁用片选 */
	NAND_FALSH_DESELECT();
}

void nand_flash_oob_read(uint32 addr, uint8 * buf, uint32 len)
{
	uint32 index = 0;
	uint32 page = addr / NAND_FLASH_OOB_SIZE;
	uint32 offset = (addr % NAND_FLASH_OOB_SIZE) + NAND_FLASH_PAGE_SIZE;

	/* 启用片选 */
	NAND_FALSH_SELECT();

	while (index < len)
	{
		/* 发出读命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_CMD__);

		/* 发出页地址和偏移地址 */
		nand_flash_addr_page_offset(page, offset);
		
		/* 发出读数据地址确认命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_ADDR_CMD__);
		
		/* 等待数据 */
		NAND_FALSH_WAIT_READY();

		/* 读page的所有OOB数据 */
		while ((offset < NAND_FLASH_PAGE_END) && (index < len))
		{
			buf[index] = NAND_FLASH_READ_BYTE();
			offset++;
			index++;
		}

		/* 一个page读取完毕读取下一个Page */
		offset = NAND_FLASH_PAGE_SIZE;
		page++;
	}

	/* 禁用片选 */
	NAND_FALSH_DESELECT();
}

/* 检查某一页是否坏掉 */
static BOOL test_nand_flash_bad_page_check(uint32 pageIdx)
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
BOOL test_nand_flash_bad_blk_check(uint32 blkIdx, uint32 pageNum)
{
	uint32 pageIndex = 0;
	BOOL isBadPage = FALSE;
	BOOL isBadBlk = FALSE;
	uint32 blkStartPage = 0;

	blkStartPage = blkIdx * pageNum;
	for (pageIndex = 0; pageIndex < pageNum; pageIndex++)
	{
		isBadPage = test_nand_flash_bad_page_check(blkStartPage + pageIndex);
		if (isBadPage) {
			//print_screen("\r\n Block %d page %d is bad page.", blkIdx, pageIndex);
			isBadBlk = TRUE;
			break;
		}
	}
	
	return isBadBlk;
}


/* Nand Flash是以Block为单位擦除的，即使len < sizeof(block) */
int nand_flash_earse(uint32 addr, int len)
{
	uint32 index = 0;
	uint32 page = 0;
	int earseLen = len;
	uint32 earseAddr = addr;
	nand_flash_info_t info;
	uint32 blkSize = 0;

	set_buffer((uint8*)(&info), 0, sizeof(info));
	nand_flash_get_mem_info(&info);
	blkSize = info.blkSize;

	/* 
	 * 如果不是每个块的开始地址，则返回错误
	 * 或者长度不是整个块大小的倍数返回错误
	 */
	if ((earseAddr % blkSize) || (earseLen % blkSize))
	{
		print_screen("\r\n Invalid addr[%d] or len[%d]!!!", earseAddr, earseLen);
		print_screen("\r\n Address and length must be a multiple of block size %d", blkSize);
		return ERROR;
	}

	/* 启用片选 */
	NAND_FALSH_SELECT();

	while (earseLen > 0)
	{
		page = earseAddr / NAND_FLASH_PAGE_SIZE;
		
		/* 发出读命令 */
		nand_flash_cmd_byte(__NAND_FLASH_EARSE_CMD__);
		
		/* 发出地址,3Byte Page Addr */
		nand_flash_addr_byte(page & 0xFF);
		nand_flash_addr_byte((page >> 8) & 0xFF);
		nand_flash_addr_byte((page >> 16) & 0xFF);
		
		/* 发出读数据地址确认命令 */
		nand_flash_cmd_byte(__NAND_FLASH_EARSE_ADDR_CMD__);
		
		/* 等待Nand解除busy状态 */
		NAND_FALSH_WAIT_READY();

		/* 一次擦除一个块 */
		earseLen -= blkSize;
		earseAddr +=blkSize;
	}

	/* 禁用片选 */
	NAND_FALSH_DESELECT();
	return OK;
}


/* Nand Flash读操作 */
void nand_flash_write(uint32 addr, uint8 *buf, uint32 len)
{
	uint32 index = 0;
	uint32 page = addr / NAND_FLASH_PAGE_SIZE;
	uint32 offset = addr % NAND_FLASH_PAGE_SIZE;

	/* 启用片选 */
	NAND_FALSH_SELECT();

	while (index < len)
	{
		/* 发出写命令 */
		nand_flash_cmd_byte(__NAND_FLASH_WRITE_CMD__);
		
		/* 发出页地址和偏移地址 */
		nand_flash_addr_page_offset(page, offset);

		/* 发出一页数据 */
		while ((offset < NAND_FLASH_PAGE_SIZE) && (index < len))
		{
			NAND_FLASH_WRITE_BYTE(buf[index]);
			offset++;
			index++;
		}
		
		/* 发出写数据结束命令 */
		nand_flash_cmd_byte(__NAND_FLASH_WRITE_DATA_OVER_CMD__);
		
		/* 等待Nand解除busy状态 */
		NAND_FALSH_WAIT_READY();

		/* 烧写下一页 */
		offset = 0;
		page++;
	}
	
	/* 禁用片选 */
	NAND_FALSH_DESELECT();
}

