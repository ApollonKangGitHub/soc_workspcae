#include <tools.h>
#include <adc.h>
#include <soc_s3c2440_public.h>

void adc_init(void)
{
	/*
	 * ECFLG [15]: End of conversion flag(Read only) 
	 *		0 = A/D conversion in process 
	 *		1 = End of A/D conversion
	 * PRSCEN [14]: A/D converter prescaler enable (ADC预分频器使能) 
	 * 		0 = Disable 
	 * 		1 = Enable
	 * PRSCVL [13:6]: A/D converter prescaler value (预分频器值)
 	 * 		Data value: 0 ~ 255 
	 *		NOTE: ADC Freqeuncy should be set less than PCLK by 5times.
	 *		(Ex. PCLK=10MHZ, ADC Freq.< 2MHz)
	 * SEL_MUX [5:3]: Analog input channel select (多路选择器输入选择)
	 * 		000 = AIN 0 
	 * 		001 = AIN 1 
	 * 		010 = AIN 2 
	 * 		011 = AIN 3 
	 * 		100 = YM 
	 * 		101 = YP 
	 * 		110 = XM 
	 * 		111 = XP
	 * STDBM [2]: Standby mode select (待机模式选择)
	 * 		0 = Normal operation mode 
	 * 		1 = Standby mode
	 * ENABLE_START [0]: A/D conversion starts by enable.  (ADC启动)
	 * 		If READ_START is enabled, this value is not valid. 
	 * 		0 = No operation 
	 * 		1 = A/D conversion starts and this bit is cleared after the start-up.
	 */

	/* 先使能预分频器、设置预分频器 */
	ADCCONr = (0x1 << 14) | (49 << 6);	

	/* 设置读取周期，默认值 */
	ADCDLYr = 0x00ff;
}

void adc_input_select(adc_mux_type_t type)
{
	/* clear当前MUX输入 */	
	ADCCONr &= ~(0x7 << 3);

	switch (type) {
		case adc_mux_type_ain0:
			ADCCONr |= (0x0 << 3);
			break;
		case adc_mux_type_ain1:
			ADCCONr |= (0x1 << 3);
			break;
		case adc_mux_type_ain2:
			ADCCONr |= (0x2 << 3);
			break;
		case adc_mux_type_ain3:
			ADCCONr |= (0x3 << 3);
			break;
		case adc_mux_type_ym:
			ADCCONr |= (0x4 << 3);
			break;
		case adc_mux_type_yp:
			ADCCONr |= (0x5 << 3);
			break;
		case adc_mux_type_xm:
			ADCCONr |= (0x6 << 3);
			break;
		case adc_mux_type_xp:
			ADCCONr |= (0x7 << 3);
			break;
		default:
			ADCCONr &= ~(0x7 << 3);
			break;
	}
}

int adc_start(void)
{
	/* 启动ADC */
	ADCCONr |= (1 << 0);
}

void adc_delay(uint32 millisecond)
{
	/*
	 * 延时时间 = ADCDLY * 晶振周期 
     * millisecond = ADCDLY * (1s / CRYSTAL_OSCILLATOR_FREQUENCY)
     * ADCDLY = (CRYSTAL_OSCILLATOR_FREQUENCY * millisecond) / 1s 
     *        = (CRYSTAL_OSCILLATOR_FREQUENCY * millisecond) / 1000ms
     */
	ADCDLYr = (CRYSTAL_OSCILLATOR_FREQUENCY * millisecond / 1000);
}

uint32 adc_read_x(void)
{
	return (ADCDAT0r & ADC_VAL_MASK);
}

uint32 adc_read_y(void)
{
	return (ADCDAT1r & ADC_VAL_MASK);
}

int adc_read_ain0(void)
{
	adc_start();

	/* 判断转换状态 */
	while (!(ADCCONr & (1 << 15)));

	/*
	 * 返回ADC值
	 * 		XPDATA (Normal ADC) 
	 * 		[9:0] X-Position conversion data value
	 * 		(include normal ADC conversion data value) 
	 * 		Data value: 0x0 ~ 0x3FF
	 */
	return (ADCDAT0r & ADC_VAL_MASK);
}


