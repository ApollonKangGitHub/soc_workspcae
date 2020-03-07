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

typedef enum {
	led_num_d10,
	led_num_d11,
	led_num_d12,
	led_num_max
}led_num_t;

typedef enum {
	led_light_off,
	led_light_on,
	led_light_max
}led_light_t;

typedef struct {
	led_num_t ledNum;
	led_light_t ledLight;
}led_info; 

void led_delay(volatile int time);
extern void led_init();
extern void led_running();
extern void led_light(led_num_t num, led_light_t light);
extern void led_light_all(int num, led_info * ledArr);

#endif
