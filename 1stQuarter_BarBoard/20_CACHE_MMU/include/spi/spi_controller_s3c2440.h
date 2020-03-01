#ifndef _SPI_CONTRLLER_S3C2440_H_
#define _SPI_CONTRLLER_S3C2440_H_

#include <tools.h>

extern void spi_send_byte(uint8 data);
extern uint8 spi_receive_byte(void);
extern void spi_init(void);

#endif	/* _SPI_CONTRLLER_S3C2440_H_ */