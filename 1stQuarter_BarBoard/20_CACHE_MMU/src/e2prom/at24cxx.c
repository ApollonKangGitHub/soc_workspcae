#include <at24cxx.h>
#include <i2c.h>
#include <tools.h>

/* E2PROM Read */
int at24cxx_read_random(uint8 offset, uint8 * data, int len)
{
	int ret = 0;
	
	if (len + offset > 256)
	{
		return ERROR;
	}

	/* 选择偏移地址 */
	ret = i2c_write(AT24CXX_DEVICE_ADDR, AT24CXX_WORD_ADDR_LEN, &offset);
	if (ret != OK) 
	{
		print_screen("\r\n i2c write %x failed.", AT24CXX_DEVICE_ADDR);
		return ERROR;
	}

	/* ST24CXX读操作最后一个数据不能回复ACK */
	i2c_last_ack_control(FALSE);

	/* 读取操作 */
	ret = i2c_read(AT24CXX_DEVICE_ADDR, len, data);
	if (ret != OK) 
	{	
		print_screen("\r\n i2c read %x failed.", AT24CXX_DEVICE_ADDR);
		return ERROR;
	}

	return OK;
}

/* E2PROM Write */
int at24cxx_write_bytes(uint8 offset, uint8 * data, int len)
{
	int ret = 0;
	uint8 dataByte[2]= {0};
	int index = 0;

	if (len + offset > 256)
	{
		return ERROR;
	}

	/* 多字节依次发送 */
	for (index = 0; index < len; index++)
	{
		dataByte[0] = offset++;
		dataByte[1] = data[index];

		/* 发送数据 */
		ret = i2c_write(AT24CXX_DEVICE_ADDR, 2, dataByte);
		if (ret != OK) 
		{
			print_screen("\r\n i2c write %x failed.", AT24CXX_DEVICE_ADDR);
			return ERROR;
		}
	}

	return OK;
}

