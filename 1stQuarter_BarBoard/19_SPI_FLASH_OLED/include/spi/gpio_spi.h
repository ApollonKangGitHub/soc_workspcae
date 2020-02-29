#ifndef _GPIO_SPI_H_
#define _GPIO_SPI_H_

extern void spi_init(void);
extern void spi_send_byte(uint8 data);
extern uint8 spi_receive_byte(void);

#endif	/* _GPIO_SPI_H_ */
