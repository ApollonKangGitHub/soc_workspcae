#include <uart.h>
#include <tools.h>
#include <nand_flash.h>
#include <log.h>
#include <soc_s3c2440_public.h>

extern void nand_flash_cmd_byte(uint8 cmd);
extern void nand_flash_addr_byte(uint8 addr);

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

