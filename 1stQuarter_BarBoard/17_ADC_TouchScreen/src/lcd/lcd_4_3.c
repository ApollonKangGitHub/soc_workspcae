#include <lcd_4_3.h>
#include <lcd_type.h>

lcd_parameters_t lcd_4_3_para = {
	.name = "lcd_4_3",
	.type = lcd_type_4_3,
	.pins_pol = {
		._vclk = POLARITY_TYPE_NORMAL,
		._rgb = POLARITY_TYPE_NORMAL, 
		._hsync = POLARITY_TYPE_INVERT,
		._vsync = POLARITY_TYPE_INVERT,
		._pwrEn = POLARITY_TYPE_NORMAL,
		._dataEn = POLARITY_TYPE_NORMAL,
	},		/* 引脚极性 */
	.time_seq = {
		/* 垂直方向各个脉冲宽度 */
		._tvp = 10,		/* Vertical Pulse width(垂直脉冲宽度) */
		._tvb = 2,		/* Vertical Back porch(上/后边黑框) */
		._tvf = 2,		/* Vertical Front porch(下/前边黑框) */
		
		/* 水平方向各个脉冲宽度/周期(三个值加起来大于44) */
		._thp = 41,		/* Horizontal Pulse width(水平脉冲宽度) */
		._thb = 2,		/* Horizontal Back porch(左/后边黑框) */
		._thf = 2,		/* Horizontal Front porch(右/前边黑框) */
		
		.vclk = 9,		/* Vsync Clock cycle, MHZ */
	},		/* 时序 */	
	.pixel = {
		._x_res = 480,				/* x轴，水平 */
		._y_res = 272,				/* y轴，垂直 */
		._bpp = 24,					/* Bit Per pixel */
	},		/* 像素分辨率 */
	.fb_base = LCD_4_3_FB,			/* frame Buffer基地址 */
};

