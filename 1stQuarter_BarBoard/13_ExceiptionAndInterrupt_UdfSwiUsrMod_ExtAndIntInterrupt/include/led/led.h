#ifndef _LED_H_
#define _LED_H_

#define FRESQUENCY_HIGH

#ifndef FRESQUENCY_HIGH
#define CLOCK_FCLK_12MHZ
#else
#define CLOCK_FCLK_400MHZ
#endif

#ifdef CLOCK_FCLK_12MHZ
#define DEFAULT_DELAY_TIME		(25000)
#define DEFAULT_INC_DEALY_TIME	(2000)
#define DEFAULT_ADD_DEALY_TIME	(2000)
#define DELAY_TIME_MIN			(5000)
#define DELAY_TIME_MAX			(50000)
#endif
#ifdef CLOCK_FCLK_400MHZ
#define DEFAULT_DELAY_TIME		(250000)
#define DEFAULT_INC_DEALY_TIME	(20000)
#define DEFAULT_ADD_DEALY_TIME	(20000)
#define DELAY_TIME_MIN			(50000)
#define DELAY_TIME_MAX			(500000)
#endif

void led_delay(volatile int time);
extern void led_init();
extern void led_running();

#endif