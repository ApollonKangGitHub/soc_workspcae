#include <i2c_controller.h>
#include <tools.h>

int i2c_read(uint32 addr, int len, uint8 * buf)
{
	i2c_msg_t msg;

	msg.addr = addr;
	msg.flags = I2C_CONTROLLER_READ;
	msg.buf = buf;
	msg.len = len;
	msg.cnt = -1;
	
	return i2c_controller_master_transfer(&msg, 1);
}

int i2c_write(uint32 addr, int len, uint8 * buf)
{
	i2c_msg_t msg;

	msg.addr = addr;
	msg.flags = I2C_CONTROLLER_WRITE;
	msg.buf = buf;
	msg.len = len;
	msg.cnt = -1;
	
	return i2c_controller_master_transfer(&msg, 1);
}

void i2c_last_ack_control(BOOL send)
{
	i2c_controller_last_ack_send(send);
}

int i2c_init(void)
{
	i2c_controller_select(i2c_controller_type_s3c2440);
	i2c_controller_init();
}
