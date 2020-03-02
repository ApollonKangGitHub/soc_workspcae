#ifndef _LCD_CONTROLLER_COMMON_H_
#define _LCD_CONTROLLER_COMMON_H_

#include <lcd_type.h>
#include <lcd_controller_type.h>
#include <tools.h>


extern int lcd_controller_select(lcd_controller_type_t type);
extern void lcd_controller_init(lcd_parameters_t * p_lcd_para);
extern void lcd_controller_enable(BOOL enable);

#endif	/* _LCD_CONTROLLER_COMMON_H_ */