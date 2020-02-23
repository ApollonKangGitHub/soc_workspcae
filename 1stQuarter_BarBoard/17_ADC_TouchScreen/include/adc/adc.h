#ifndef _ADC_H_
#define _ADC_H_

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
extern int adc_read_ain0(void);
extern void adc_input_select(adc_mux_type_t type);
	
#endif	/* _ADC_H_ */