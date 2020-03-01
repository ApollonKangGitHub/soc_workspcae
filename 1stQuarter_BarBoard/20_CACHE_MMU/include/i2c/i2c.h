#ifndef _I2C_COMMON_H_
#define _I2C_COMMON_H_

#include <tools.h>
#include <i2c_controller.h>

extern int i2c_init(void);
extern int i2c_read(uint32 addr, int len, uint8 * buf);
extern int i2c_write(uint32 addr, int len, uint8 * buf);
extern void i2c_last_ack_control(BOOL send);

#endif	/* _I2C_COMMON_H_ */