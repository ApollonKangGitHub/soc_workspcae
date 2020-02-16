#ifndef _LCD_CONTROLLER_COMMON_H_
#define _LCD_CONTROLLER_COMMON_H_

#include <lcd_common.h>
#include <tools.h>

typedef enum {
	lcd_drv_soc_s3c2440,
	lcd_drv_max	
}lcd_drv_type_t;

/* lcd控制器驱动结构体 */
typedef struct {
	char * lcd_controller_drv_name;						/* 驱动名 */
	void (*lcd_init)(lcd_parameters_t * p_lcd_para);	/* lcd初始化 */
	void (*lcd_enable)(BOOL enable);					/* lcd使能/禁用 */
}lcd_controller_drv_t;

#endif	/* _LCD_CONTROLLER_COMMON_H_ */