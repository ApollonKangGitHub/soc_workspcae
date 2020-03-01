#ifndef _LCD_COMMON_H_
#define _LCD_COMMON_H_

#include <tools.h>
#include <lcd_type.h>
#include <lcd_controller_type.h>

/* bpp类型 */
typedef enum {
	bpp_type_8bits,
	bpp_type_16bits,
	bpp_type_24bits,
	bpp_type_32bits,
	bpp_type_max
}bpp_type_t;


/* 上层关心的参数 */
typedef struct
{
	uint32 _x_res;		/* x轴，水平 */
	uint32 _y_res;		/* y轴，垂直 */
	bpp_type_t _bpp;	/* Bit Per pixel */
	uint32 _fb_base;	/* frame Buffer基地址 */
}lcd_common_para_t;

extern int lcd_common_init
(
	lcd_type_t lcd_type, 				/* lcd类型 */
	lcd_controller_type_t lcd_ctrl_type	/* lcd控制器类型 */
);
extern void lcd_common_enable(BOOL enable);
extern void lcd_common_para_get(lcd_common_para_t * pPara);


#endif	/* _LCD_COMMON_H_ */
