#include <lcd_type.h>
#include <lcd_controller_type.h>
#include <lcd_common.h>

extern lcd_parameters_t lcd_3_5_para;
extern lcd_parameters_t lcd_4_3_para;

static lcd_type_t gLcdType = lcd_type_max;

static lcd_parameters_t *gLcdPara[lcd_type_max] = {
	&lcd_3_5_para,
	&lcd_4_3_para
}; 

/* lcd初始化 */
int lcd_common_init
(
	lcd_type_t lcd_type, 				/* lcd类型 */
	lcd_controller_type_t lcd_ctrl_type	/* lcd控制器类型 */
)
{
	lcd_parameters_t * lcdPara = NULL;
	
	if ((lcd_type >= lcd_type_max)
		|| (lcd_ctrl_type >= lcd_controller_max))
	{
		return ERROR;
	}	
	/* 选择某款LCD参数 */
	lcdPara = gLcdPara[lcd_type];
	gLcdType = lcd_type;
#if 0
	/* 选择某款LCD控制器 */
	(void)lcd_controller_select(lcd_ctrl_type);

	/* 使用LCD参数初始化LCD控制器 */
	(void)lcd_controller_init(lcdPara);
#else
	extern void lcd_controller_init_s3c2440(lcd_parameters_t * p_lcd_para);
	lcd_controller_init_s3c2440(lcdPara);
#endif
	
}

void lcd_common_enable(BOOL enable)
{
	/* 存在blx指令异常问题，暂时先不用函数指针 */
#if 0
	lcd_controller_enable(enable);
#else
	extern void lcd_controller_enable_s3c2440(BOOL enable);
	lcd_controller_enable_s3c2440(enable);
#endif
}

static bpp_type_t lcd_common_bpp_convert(uint32 bpp)
{
	switch(bpp)
	{
		case 8:
			return bpp_type_8bits;
		case 16:
			return bpp_type_16bits;
		case 24:
			return bpp_type_24bits; 
		case 32:
			return bpp_type_32bits; 
		default:
			return 0;
	}
	return bpp_type_max;
}

void lcd_common_para_get(lcd_common_para_t * pPara)
{
	pPara->_fb_base = gLcdPara[gLcdType]->fb_base;
	pPara->_x_res = gLcdPara[gLcdType]->pixel._x_res;
	pPara->_y_res =  gLcdPara[gLcdType]->pixel._y_res;
	pPara->_bpp =  lcd_common_bpp_convert(gLcdPara[gLcdType]->pixel._bpp);
}

