#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#include <tools.h>

extern void spi_flash_read_id(uint8 * pMid, uint8 * pDid);
extern void oled_clear(void);

#endif	/* __SPI_FLASH_H__ */
