#ifndef _SOC_S3C2440_ADC_TS_FIELD_H_
#define _SOC_S3C2440_ADC_TS_FIELD_H_


#define ADCUPDN_TSC_UP_BIT		(1)
#define ADCUPDN_TSC_DOWN_BIT	(0)

/*
 * ADCTSCr:
 * UD_SEN [8] Detect Stylus Up or Down status. 
 *		0 = Detect Stylus Down Interrupt Signal. 
 *		1 = Detect Stylus Up Interrupt Signal. 
 * 
 * YM_SEN [7] YM Switch Enable	(y轴正极)
 *		0 = YM Output Driver Disable. 
 *		1 = YM Output Driver Enable. 
 * 
 * YP_SEN [6] YP Switch Enable	(y轴负极)
 *		0 = YP Output Driver Enable. 
 *		1 = YP Output Driver Disable. 
 * 
 * XM_SEN [5] XM Switch Enable 
 *		0 = XM Output Driver Disable. 
 *		1 = XM Output Driver Enable. 
 * 
 * XP_SEN [4] XP Switch Enable 
 *		0 = XP Output Driver Enable. 
 *		1 = XP Output Driver Disable. 
 * 
 * PULL_UP [3] Pull-up Switch Enable 
 *		0 = XP Pull-up Enable. 
 *		1 = XP Pull-up Disable. 
 * 
 * AUTO_PST [2] Automatically sequencing conversion of X-Position and Y-Position 
 *		0 = Normal ADC conversion. 
 *		1 = Auto Sequential measurement of X-position, Y-position. 
 * 
 * XY_PST [1:0] Manually measurement of X-Position or Y-Position. 
 *		00 = No operation mode 
 *		01 = X-position measurement 
 *		10 = Y-position measurement 
 *		11 = Waiting for Interrupt Mode
 */
#define ADCTSC_UD_SEN_START		(8)
#define ADCTSC_YM_SEN_START		(7)
#define ADCTSC_YP_SEN_START		(6)
#define ADCTSC_XM_SEN_START		(5)
#define ADCTSC_XP_SEN_START		(4)
#define ADCTSC_PULL_UP_START	(3)
#define ADCTSC_AUTO_PST_START	(2)
#define ADCTSC_XY_PST_START		(0)

#define ADCTSC_WAIT_PEN_DOWN	(0 << ADCTSC_UD_SEN_START)
#define ADCTSC_WAIT_PEN_UP		(1 << ADCTSC_UD_SEN_START)

#define ADCTSC_YM_ENABLE		(1 << ADCTSC_YM_SEN_START)
#define ADCTSC_YM_DISABLE		(0 << ADCTSC_YM_SEN_START)
#define ADCTSC_YP_ENABLE		(0 << ADCTSC_YP_SEN_START)
#define ADCTSC_YP_DISABLE		(1 << ADCTSC_YP_SEN_START)

#define ADCTSC_XM_ENABLE		(1 << ADCTSC_XM_SEN_START)
#define ADCTSC_XM_DISABLE		(0 << ADCTSC_XM_SEN_START)
#define ADCTSC_XP_ENABLE		(0 << ADCTSC_XP_SEN_START)
#define ADCTSC_XP_DISABLE		(1 << ADCTSC_XP_SEN_START)

#define ADCTSC_PULL_UP_ENABLE	(0 << ADCTSC_PULL_UP_START)
#define ADCTSC_PULL_UP_DISABLE	(1 << ADCTSC_PULL_UP_START)

#define ADCTSC_AUTO_PST_NOTMAL	(0 << ADCTSC_AUTO_PST_START)
#define ADCTSC_AUTO_PST_AUTO	(1 << ADCTSC_AUTO_PST_START)

/*
 * Manually measurement of X-Position or Y-Position. 
 * 00 = No operation mode 
 * 01 = X-position measurement 
 * 10 = Y-position measurement 
 * 11 = Waiting for Interrupt Mode
 */
#define ADCTSC_XY_PST_WAIT_INT_MODE		(0x3 << ADCTSC_XY_PST_START)
#define ADCTSC_XY_PST_Y_POS_MEA_MODE	(0x2 << ADCTSC_XY_PST_START)
#define ADCTSC_XY_PST_X_POS_MEA_MODE	(0x1 << ADCTSC_XY_PST_START)
#define ADCTSC_XY_PST_NO_OPER_MODE		(0x0 << ADCTSC_XY_PST_START)

#define ADCDATA_UPDOWN_BIT				(15)
#define ADCDATA_UPDOWN_UP				(1)
#define ADCDATA_UPDOWN_DOWN				(0)

#endif	/* _SOC_S3C2440_ADC_TS_FIELD_H_ */
