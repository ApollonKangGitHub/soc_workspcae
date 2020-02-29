#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#include <tools.h>

extern void spi_flash_earse(uint32 addr);
extern void spi_flash_write(uint32 addr, uint8 * buf, int len);
extern void spi_flash_read(uint32 addr, uint8 * buf, int len);
extern void spi_flash_read_id(uint8 * pMid, uint8 * pDid);
extern void spi_flash_init(void);

#endif	/* __SPI_FLASH_H__ */
