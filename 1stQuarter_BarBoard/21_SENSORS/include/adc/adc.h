#ifndef _ADC_H_
#define _ADC_H_

/*
 * ADCDATA0: XPDATA [9:0] X-position conversion data value 
 * ADCDATA1: YPDATA [9:0] Y-position conversion data value 
 * Data value: 0 ~ 3FF
 */
#define ADC_VAL_MASK	(0x3FF)

typedef enum{
	adc_mux_type_ain0,
	adc_mux_type_ain1,
	adc_mux_type_ain2,
	adc_mux_type_ain3,
	adc_mux_type_ym,
	adc_mux_type_yp,
	adc_mux_type_xm,
	adc_mux_type_xp,
	adc_mux_type_max
}adc_mux_type_t;

extern void adc_init(void);
extern int adc_start(void);
extern uint32 adc_read_x(void);
extern uint32 adc_read_y(void);
extern void adc_delay(uint32 millisecond);
extern int adc_read_ain0(void);
extern int adc_read_channel(adc_mux_type_t type);
	
#endif	/* _ADC_H_ */