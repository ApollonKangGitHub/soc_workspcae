#include <lcd_controller_common.h>
#define lcd_controller_max 	(NULL)

extern lcd_controller_drv_t lcd_controller_s3c2440;

/* 驱动公共调用结构 */
lcd_controller_drv_t * lcd_controller[lcd_drv_max + 1] = {
	&lcd_controller_s3c2440,
	lcd_controller_max
};

lcd_drv_type_t lcd_controller_drv_type(void)
{
	return lcd_drv_soc_s3c2440;
}


void lcd_controller_init(lcd_parameters_t * p_lcd_para)
{
	lcd_drv_type_t __type__ = lcd_drv_max;

	__type__ = lcd_controller_drv_type();
	
	/* 调用lcd控制器的初始化函数 */
	lcd_controller[__type__]->lcd_init(p_lcd_para);
}

void lcd_controller_enable(BOOL enable)
{
	lcd_drv_type_t __type__ = lcd_drv_max;

	__type__ = lcd_controller_drv_type();
	
	/* 调用lcd控制器的使能函数 */
	lcd_controller[__type__]->lcd_enable(enable);
}

