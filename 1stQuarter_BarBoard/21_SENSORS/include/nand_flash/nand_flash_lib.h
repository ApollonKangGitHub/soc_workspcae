#ifndef _NAND_FLASH_LIB_H_
#define _NAND_FLASH_LIB_H_

#include <tools.h>
#include <nand_flash.h>

extern void nand_flash_init(void);
extern void nand_flash_get_mem_info(nand_flash_info_t * info);
extern void nand_flash_data_read(uint32 addr, uint8 * buf, uint32 len);
extern void nand_flash_oob_read(uint32 addr, uint8 * buf, uint32 len);
extern BOOL test_nand_flash_bad_blk_check(uint32 blkIdx, uint32 pageNum);
extern int nand_flash_earse(uint32 addr, int len);
extern void nand_flash_write(uint32 addr, uint8 *buf, uint32 len);

#endif	/* _NAND_FLASH_LIB_H_ */