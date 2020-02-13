#ifndef _NAND_FLASH_H_
#define _NAND_FLASH_H_

#define NAND_K9F2G08U0C_TACLS	(0)
#define NAND_K9F2G08U0C_TWRPH0	(1)
#define NAND_K9F2G08U0C_TWRPH1	(0)

#define NAND_INIT_ECC_DECODER_ENCODER	(1)	/* 初始化ECC编码解码器 */
#define NAND_nFCE_CTRL_DISABLE			(1)	/* 高电平禁止，低电平有效，禁止片选 */
#define NAND_NAND_CONTOLLER_ENABLE		(1)	/* 使能Nand Flash控制器 */

/* 发命令 */
#define __NAND_FLASH_READ_ID_CMD__		(0x90)
/* 读ID的起始地址 */
#define __NAND_FLASH_READ_ID_ADDR__		(0x0)

/* ID各个字节含义 */
#define _NAND_FLASH_ID_MAKER_		(0)	/* 生产厂家ID */
#define _NAND_FLASH_ID_DEVICE_		(1)	/* 设备ID */
#define _NAND_FLASH_ID_3rd_			(2)
#define _NAND_FLASH_ID_4th_			(3)
#define _NAND_FLASH_ID_5th_			(4)

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

extern void nand_flash_info_display(void);
extern void nand_flash_init(void);

#endif	/* _NAND_FLASH_H_ */

