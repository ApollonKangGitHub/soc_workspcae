#ifndef _TOUCH_SCREEN_H_
#define _TOUCH_SCREEN_H_

#include <tools.h>

extern void touchScreen_wait_pen_down(void);
extern void touchScreen_adc_ts_reg_init(void);
extern void touchScreen_adc_ts_interrupt_init(void);
extern void * touchScreen_timer_handle(void * pArgv);
extern void touchScreen_get_contact_coordinates(uint32 *x, uint32 *y, BOOL * pPressure);

#endif	/* _TOUCH_SCREEN_H_ */