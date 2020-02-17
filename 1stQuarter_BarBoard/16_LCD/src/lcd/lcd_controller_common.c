#include <lcd_type.h>
#include <lcd_controller_type.h>
#include <log.h>

extern lcd_controller_drv_t lcd_controller_s3c2440;
static lcd_controller_type_t gControllerType = lcd_controller_max;

/* 驱动公共调用结构 */
lcd_controller_drv_t * gLcdControllerDrv[lcd_controller_max] = {
	&lcd_controller_s3c2440
};

/* 选择LCD控制器 */
int lcd_controller_select(lcd_controller_type_t type)
{
	if ((type >= lcd_controller_max)
		|| (NULL == gLcdControllerDrv[type]))
	{
		return ERROR;
	}
	
	gControllerType = type;
}

static lcd_controller_type_t lcd_controller_drv_type(void)
{
	lcd_controller_type_t type = lcd_controller_max;

	type = lcd_controller_soc_s3c2440;

	if (gControllerType != lcd_controller_max)
	{
		type = gControllerType;
	}

	if ((type >= lcd_controller_max)
		|| (NULL == gLcdControllerDrv[type]))
	{
		type = lcd_controller_max;
	}

	return type;
}

/* 调用lcd控制器的初始化函数 */
void lcd_controller_init(lcd_parameters_t * p_lcd_para)
{
	lcd_controller_type_t __type__ = lcd_controller_max;

	__type__ = lcd_controller_drv_type();

	if ((__type__ < lcd_controller_max)
		&& (gLcdControllerDrv[__type__]->lcd_init != NULL))
	{
		gLcdControllerDrv[__type__]->lcd_init(p_lcd_para);
	}
	else 
	{
		SOC_DEBUG_PRINT(SOC_DBG_ERROR, "\r\n %s:%s-%d is valid", __FILE__, __FUNCTION__, __LINE__);
	}
}

/* LCD使能（控制器使能、供电、背光灯使能等等一系列使能） */
void lcd_controller_enable(BOOL enable)
{
	lcd_controller_type_t __type__ = lcd_controller_max;

	__type__ = lcd_controller_drv_type();
	
	if ((__type__ < lcd_controller_max)
		&& (gLcdControllerDrv[__type__]->lcd_enable != NULL))
	{
		gLcdControllerDrv[__type__]->lcd_enable(enable);
	}		
	else
	{
		SOC_DEBUG_PRINT(SOC_DBG_ERROR, "\r\n %s:%s-%d is valid", __FILE__, __FUNCTION__, __LINE__);
	}
}

