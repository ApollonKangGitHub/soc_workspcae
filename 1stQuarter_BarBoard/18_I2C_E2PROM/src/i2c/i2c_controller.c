#include <i2c_controller.h>
#include <tools.h>

/* I2C控制器驱动数组 */
extern i2c_controller_t gI2cControllerDrvS3c2440;
static i2c_controller_t * gI2cControllerDrv[i2c_controller_type_max] = {
	&gI2cControllerDrvS3c2440,
};

static i2c_controller_type_t gI2cControllerType = i2c_controller_type_max;

/* 驱动获取类型进行具体函数调用 */
static i2c_controller_type_t i2c_controller_drv_type(void)
{
	i2c_controller_type_t __type__ = i2c_controller_type_max;

	__type__ = i2c_controller_type_s3c2440;

	if (gI2cControllerType != i2c_controller_type_max)
	{
		__type__ = gI2cControllerType;
	}

	if ((__type__ >= i2c_controller_type_max)
		|| (NULL == gI2cControllerDrv[__type__]))
	{
		__type__ = i2c_controller_type_max;
	}

	return __type__;
}

/* I2C控制器选择 */
int i2c_controller_select(i2c_controller_type_t type)
{
	if ((type >= i2c_controller_type_max)
		|| (NULL == gI2cControllerDrv[type]))
	{
		return ERROR;
	}
	
	gI2cControllerType = type;
}

/* I2C transfer函数（读/写） */
int i2c_controller_master_transfer(i2c_msg_t * msgs, int num)
{
	i2c_controller_type_t __type__ = i2c_controller_drv_type();

	if ((__type__ < i2c_controller_type_max)
		&& gI2cControllerDrv[__type__]->i2c_master_xfer)
	{
		gI2cControllerDrv[__type__]->i2c_master_xfer(msgs, num);
	}
	else 
	{
		print_screen("\r\n Invalid function %s-%d, type:%d", __FUNCTION__, __LINE__, __type__);
	}
}

/* I2C 控制器初始化 */
int i2c_controller_init(void)
{
	i2c_controller_type_t __type__ = i2c_controller_drv_type();

	if ((__type__ < i2c_controller_type_max)
		&& gI2cControllerDrv[__type__]->i2c_controller_init)
	{
		gI2cControllerDrv[__type__]->i2c_controller_init();
	}
	else 
	{
		print_screen("\r\n Invalid function %s-%d, type:%d", __FUNCTION__, __LINE__, __type__);
	}
}

