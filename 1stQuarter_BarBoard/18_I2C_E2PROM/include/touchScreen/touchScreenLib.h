#ifndef _TOUCH_SCREEN_LIB_H_
#define _TOUCH_SCREEN_LIB_H_

#include <tools.h>

extern void touchScreen_init(void);
extern void touchScreen_calibration(void);
extern int touchScreen_get_point
(
	uint32 * lcd_x, 
	uint32 * lcd_y,
	BOOL * pressure
);


#endif
