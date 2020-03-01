#ifndef _NAND_FLASH_H_
#define _NAND_FLASH_H_

#include <tools.h>
#include <soc_s3c2440_public.h>

#define NAND_FLASH_PAGE_SIZE			(2048)
#define NAND_FLASH_OOB_SIZE				(64)
#define NAND_FLASH_PAGE_END				((NAND_FLASH_PAGE_SIZE) + (NAND_FLASH_OOB_SIZE) - 1)

/* 每个OOB区的第0个字节不等于0XFF则该Page为坏块 */
#define NAND_FLASH_OOB_CHECK_POS_OFFSET	(0)
#define NAND_FLASH_OOB_CHECK_VALUE		(0xFF)

#define NAND_K9F2G08U0C_TACLS	(0)
#define NAND_K9F2G08U0C_TWRPH0	(1)
#define NAND_K9F2G08U0C_TWRPH1	(0)

#define NAND_INIT_ECC_DECODER_ENCODER	(1)	/* 初始化ECC编码解码器 */
#define NAND_nFCE_CTRL_DISABLE			(1)	/* 高电平禁止，低电平有效，禁止片选 */
#define NAND_NAND_CONTOLLER_ENABLE		(1)	/* 使能Nand Flash控制器 */

#define NAND_FLASH_READ_BYTE()		(NFDATAr)
#define NAND_FLASH_WRITE_BYTE(val)	(NFDATAr = (volatile uint8)(val))
#define	NAND_FALSH_SELECT()			(NFCONTr &= ~(NAND_nFCE_CTRL_DISABLE << 1))
#define NAND_FALSH_DESELECT()		(NFCONTr |= (NAND_nFCE_CTRL_DISABLE << 1))

/* NFSTATr寄存器的 "RnB input pin." 不为高电平则处于忙状态 */
#define NAND_FALSH_WAIT_READY(void) \
	while(NFSTAT_RnB_STAT_READY_STATUS != (NFSTATr & NFSTAT_RnB_STAT_BUSY_BITSf));

/*
 * NFSTATr
 * RnB (Read-only) [0]
 * The status of RnB input pin. 
 * 	1: NAND Flash memory ready to operate
 *  0: NAND Flash memory busy 
 */
#define NFSTAT_RnB_STAT_BUSY_BITSf			(0x1)
#define NFSTAT_RnB_STAT_BUSY_STATUS			(0x0)
#define NFSTAT_RnB_STAT_READY_STATUS		(0x1)

/* 发送读ID命令 */
#define __NAND_FLASH_READ_ID_CMD__			(0x90)
/* 发送读数据命令 */	
#define __NAND_FLASH_READ_DATA_CMD__		(0x0)
/* 发出读数据地址确认命令 */
#define __NAND_FLASH_READ_DATA_ADDR_CMD__	(0x30)
/* 发出擦除命令 */
#define __NAND_FLASH_EARSE_CMD__			(0x60)
/* 发送擦除数据地址确认命令 */
#define __NAND_FLASH_EARSE_ADDR_CMD__		(0xD0)
/* 发出写数据命令 */
#define __NAND_FLASH_WRITE_CMD__			(0x80)
/* 发送写数据结束命令 */
#define __NAND_FLASH_WRITE_DATA_OVER_CMD__	(0x10)
/* 读ID的起始地址 */
#define __NAND_FLASH_READ_ID_ADDR__			(0x0)

/* ID各个字节含义 */
#define _NAND_FLASH_ID_MAKER_		(0)	/* 生产厂家ID */
#define _NAND_FLASH_ID_DEVICE_		(1)	/* 设备ID */
#define _NAND_FLASH_ID_3rd_			(2)
#define _NAND_FLASH_ID_4th_			(3)
#define _NAND_FLASH_ID_5th_			(4)
#define _NAND_FLASH_ID_BYTES_		(5)

/* 页大小 */
#define _NAND_FLASH_4th_PAGE_SIZE_BASE_		(1)
#define _NAND_FLASH_4th_PAGE_SIZE_MASK_		(0x3)
#define _NAND_FLASH_4th_PAGE_SIZE_SHIFT_	(0)
#define _NAND_FLASH_4th_PAGE_SIZE_1KB_		(0x0)
#define _NAND_FLASH_4th_PAGE_SIZE_2KB_		(0x1)
#define _NAND_FLASH_4th_PAGE_SIZE_4KB_		(0x2)
#define _NAND_FLASH_4th_PAGE_SIZE_8KB_		(0x3)

/* 块大小 */
#define _NAND_FLASH_4th_BLK_SIZE_BASE_		(64)
#define _NAND_FLASH_4th_BLK_SIZE_MASK_		(0x3)
#define _NAND_FLASH_4th_BLK_SIZE_SHIFT_		(4)
#define _NAND_FLASH_4th_BLK_SIZE_64KB_		(0x0)
#define _NAND_FLASH_4th_BLK_SIZE_128KB_		(0x1)
#define _NAND_FLASH_4th_BLK_SIZE_256KB_		(0x2)
#define _NAND_FLASH_4th_BLK_SIZE_512KB_		(0x3)

/* Plane大小和个数 */
#define _NAND_FLASH_5th_PLANE_NUM_BASE_		(1)
#define _NAND_FLASH_5th_PLANE_NUM_MASK_		(0x3)
#define _NAND_FLASH_5th_PLANE_NUM_SHIFT_	(2)
#define _NAND_FLASH_5th_PLANE_SIZE_BASE_	(64)
#define _NAND_FLASH_5th_PLANE_SIZE_MASK_	(0x7)
#define _NAND_FLASH_5th_PLANE_SIZE_SHIFT_	(4)

#define _NAND_FLASH_PLANE_CELL_				(1 << 20)		/* MB为单位 */
#define _NAND_FLASH_BLOCK_CELL_				(1 << 10)		/* KB为单位 */
#define _NAND_FLASH_PAGE_CELL_				(1 << 10)		/* KB为单位 */

/* nand Flash信息 */
typedef struct nand_flash_info
{
	uint8 idInfo[_NAND_FLASH_ID_BYTES_];
	uint32 planeNum;
	uint32 planeSize;
	uint32 blkNum;
	uint32 blkSize;
	uint32 pagePerBlk;
	uint32 pageSize;
	uint32 totalSize;
}nand_flash_info_t;

#endif	/* _NAND_FLASH_H_ */

