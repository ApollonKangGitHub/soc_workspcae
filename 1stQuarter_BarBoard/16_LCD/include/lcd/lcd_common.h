#ifndef _LCD_COMMON_H_
#define _LCD_COMMON_H_

#include <tools.h>

/* 极性类型定义 */
#define POLARITY_TYPE_NORMAL	(0)
#define POLARITY_TYPE_INVERT	(1)

/* 引脚极性反转定义 */
typedef struct  {
	uint8 _vclk:1;		/* NORMAL:在下降沿获取数据，反转则是在上升沿获取数据 */
	uint8 _rgb:1;		/* NORMAL:高电平表示1，反转则是低电平表示1 */
	uint8 _hsync:1;		/* NORMAL:为高脉冲 */
	uint8 _vsync:1;		/* NORMAL:为高脉冲 */
	uint8 _pwrEn:1;		/* NORMAL:高电平有效 */
	uint8 _dataEn:1;	/* NORMAL:高电平使能传输数据 */
}lcd_pins_polarity_t;

/* 引脚时序 */
typedef struct  {
	/* 垂直方向各个脉冲宽度 */
	uint32 _tvp;		/* Vertical Pulse width(垂直脉冲宽度) */
	uint32 _tvb;		/* Vertical Back porch(上/后边黑框) */
	uint32 _tvf;		/* Vertical Front porch(下/前边黑框) */

	/* 水平方向各个脉冲宽度 */
	uint32 _thp;		/* Horizontal Pulse width(水平脉冲宽度) */
	uint32 _thb;		/* Horizontal Back porch(左/后边黑框) */
	uint32 _thf;		/* Horizontal Front porch(右/前边黑框) */
}lcd_time_sequence_t;

/* bpp类型 */
typedef enum {
	bpp_type_8bits,
	bpp_type_16bits,
	bpp_type_24bits,
	bpp_type_max
}bpp_type_t;

typedef struct {
	uint32 _x_res;		/* x轴，水平 */
	uint32 _y_res;		/* y轴，垂直 */
	bpp_type_t _bpp;	/* Bit Per pixel */
}lcd_pixel_t;

typedef struct {
	lcd_pins_polarity_t pins_pol;	/* 引脚极性 */
	lcd_time_sequence_t time_seq;	/* 时序 */	
	lcd_pixel_t pixel;				/* 像素分辨率 */
	uint32 fb_base;					/* frame Buffer基地址 */
	uint32 vclk;					/* Vsync Clock cycle */
}lcd_parameters_t;

#endif	/* _LCD_COMMON_H_ */