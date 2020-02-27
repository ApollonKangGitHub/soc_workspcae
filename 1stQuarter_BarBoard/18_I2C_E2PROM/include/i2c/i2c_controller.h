#ifndef _I2C_CONTROLLER_H_
#define _I2C_CONTROLLER_H_

#include <tools.h>

#define I2C_CONTROLLER_READ		(1U << 0)
#define I2C_CONTROLLER_WRITE	(1U << 0)

/* I2C控制器类型 */
typedef enum {
	i2c_controller_type_s3c2440,
	i2c_controller_type_max	
}i2c_controller_type_t;

/* I2C操作结构 */
typedef struct {
	uint32 addr;
	uint32 flags;
	uint32 len;	/* 徐亚传输长度 */
	uint32 cnt;	/* 已经传输长度 */
	uint8 * buf;
}i2c_msg_t;

/* I2C控制器驱动结构体 */
typedef struct {
	int (*i2c_controller_init)(void);
	int (*i2c_master_xfer)(i2c_msg_t * msgs, int num);
	char * i2c_controller_name;
}i2c_controller_t;

extern int i2c_controller_select(i2c_controller_type_t type);
extern int i2c_controller_master_transfer(i2c_msg_t * msgs, int num);
extern int i2c_controller_init(void);

#endif	/* _I2C_CONTROLLER_H_ */