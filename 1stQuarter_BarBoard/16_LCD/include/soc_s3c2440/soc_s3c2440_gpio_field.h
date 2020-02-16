#ifndef _SCO_S3C2440_GPIO_FIELD_H_
#define _SCO_S3C2440_GPIO_FIELD_H_

/*
S3C2440A has 130 multi-functional input/output port pins and there are eight ports as shown below:
— Port A(GPA): 25-output port
— Port B(GPB): 11-input/out port
— Port C(GPC): 16-input/output port
— Port D(GPD): 16-input/output port
— Port E(GPE): 16-input/output port
— Port F(GPF): 8-input/output port
— Port G(GPG): 16-input/output port
— Port H(GPH): 9-input/output port
— Port J(GPJ): 13-input/output port
*/

/* GPIO common field defines */
#define GPIOCON_PIN0_CONF_BITSf			(0x3 << 0)
#define GPIOCON_PIN1_CONF_BITSf 		(0x3 << 2)
#define GPIOCON_PIN2_CONF_BITSf 		(0x3 << 4)
#define GPIOCON_PIN3_CONF_BITSf 		(0x3 << 6)
#define GPIOCON_PIN4_CONF_BITSf 		(0x3 << 8)
#define GPIOCON_PIN5_CONF_BITSf 		(0x3 << 10)
#define GPIOCON_PIN6_CONF_BITSf 		(0x3 << 12)
#define GPIOCON_PIN7_CONF_BITSf 		(0x3 << 14)
#define GPIOCON_PIN8_CONF_BITSf			(0x3 << 16)
#define GPIOCON_PIN9_CONF_BITSf 		(0x3 << 18)
#define GPIOCON_PIN10_CONF_BITSf 		(0x3 << 20)
#define GPIOCON_PIN11_CONF_BITSf 		(0x3 << 22)
#define GPIOCON_PIN12_CONF_BITSf 		(0x3 << 24)
#define GPIOCON_PIN13_CONF_BITSf 		(0x3 << 26)
#define GPIOCON_PIN14_CONF_BITSf 		(0x3 << 28)
#define GPIOCON_PIN15_CONF_BITSf 		(0x3 << 30)			/* two bits field, max 16 pins for GPG, GPA is signle bit filed */

#define GPIODAT_PIN0_DATA_BITSf			(0x1 << 0)
#define GPIODAT_PIN1_DATA_BITSf			(0x1 << 1)
#define GPIODAT_PIN2_DATA_BITSf			(0x1 << 2)
#define GPIODAT_PIN3_DATA_BITSf			(0x1 << 3)
#define GPIODAT_PIN4_DATA_BITSf			(0x1 << 4)
#define GPIODAT_PIN5_DATA_BITSf			(0x1 << 5)
#define GPIODAT_PIN6_DATA_BITSf			(0x1 << 6)
#define GPIODAT_PIN7_DATA_BITSf			(0x1 << 7) 			/* max bit 8(PIN7) for GPF */
#define GPIODAT_PIN8_DATA_BITSf			(0x1 << 8)          /* max bit 9(PIN8) for GPH */
#define GPIODAT_PIN9_DATA_BITSf			(0x1 << 9)          
#define GPIODAT_PIN10_DATA_BITSf		(0x1 << 10)         /* max bit 11(PIN10) for GPB */
#define GPIODAT_PIN11_DATA_BITSf		(0x1 << 11)         
#define GPIODAT_PIN12_DATA_BITSf		(0x1 << 12)         /* max bit 13(PIN12) for GPJ */
#define GPIODAT_PIN13_DATA_BITSf		(0x1 << 13)         
#define GPIODAT_PIN14_DATA_BITSf		(0x1 << 14)         /* max bit 16(PIN15) for GPC/GPD/GPE/GPG */
#define GPIODAT_PIN15_DATA_BITSf		(0x1 << 15) 
#define GPIODAT_PIN16_DATA_BITSf		(0x1 << 16)
#define GPIODAT_PIN17_DATA_BITSf		(0x1 << 17)
#define GPIODAT_PIN18_DATA_BITSf		(0x1 << 18)
#define GPIODAT_PIN19_DATA_BITSf		(0x1 << 19)
#define GPIODAT_PIN20_DATA_BITSf		(0x1 << 20) 
#define GPIODAT_PIN21_DATA_BITSf		(0x1 << 21)
#define GPIODAT_PIN22_DATA_BITSf		(0x1 << 22)
#define GPIODAT_PIN23_DATA_BITSf		(0x1 << 23)
#define GPIODAT_PIN24_DATA_BITSf		(0x1 << 24)			/* max bit 25(PIN24) for GPA */

#define GPIOCON_PIN_CONF_VAL_0			(0x0)
#define GPIOCON_PIN_CONF_VAL_1      	(0x1)
#define GPIOCON_PIN_CONF_VAL_2  	    (0x2)
#define GPIOCON_PIN_CONF_VAL_3   	   	(0x3)

#define GPIOCON_PIN0_START_BIT			(0)
#define GPIOCON_PIN1_START_BIT			(2)
#define GPIOCON_PIN2_START_BIT			(4)
#define GPIOCON_PIN3_START_BIT			(6)
#define GPIOCON_PIN4_START_BIT			(8)
#define GPIOCON_PIN5_START_BIT			(10)			
#define GPIOCON_PIN6_START_BIT			(12)
#define GPIOCON_PIN7_START_BIT			(14)			/* max 8 pins for GPF */
#define GPIOCON_PIN8_START_BIT			(16)			/* max 9 pins for GPH */
#define GPIOCON_PIN9_START_BIT			(18)
#define GPIOCON_PIN10_START_BIT			(20)			/* max 11 pins for GPB */
#define GPIOCON_PIN11_START_BIT			(22)
#define GPIOCON_PIN12_START_BIT			(24)
#define GPIOCON_PIN13_START_BIT			(26)			/* max 13 pins for GPJ */
#define GPIOCON_PIN14_START_BIT			(28)
#define GPIOCON_PIN15_START_BIT			(30)			/* max 16 pins for GPC/GPD/GPE/GPG */
#define GPIOCON_PIN16_START_BIT			(32)
#define GPIOCON_PIN17_START_BIT			(34)
#define GPIOCON_PIN18_START_BIT			(36)
#define GPIOCON_PIN19_START_BIT			(38)
#define GPIOCON_PIN20_START_BIT			(20)
#define GPIOCON_PIN21_START_BIT			(22)
#define GPIOCON_PIN22_START_BIT			(26)
#define GPIOCON_PIN24_START_BIT			(28)			/* max 25 pins for GPA */

/* GPB  contrl and data register*/
#define GPBCON_GPB0_MASK				(0x3)
#define GPBCON_GPB0_START				(0)
#define GBPCON_GPB_OUTPUT				(0x1)

#define GPBDATA_GPB_INPUT_DISABLE		(0x0)
#define GPBDATA_GPB_INPUT_ENABLE		(0x1)

/* Vedio Data */
#define GPCCON_VEDIO_DATA_ENABLE		(0xAAAAAAAA)
#define GPDCON_VEDIO_DATA_ENABLE		(0xAAAAAAAA)

/* GPIO_F register fields bits */
#define GPFCON_GPF0_CONF_BITSf		GPIOCON_PIN0_CONF_BITSf
#define GPFCON_GPF1_CONF_BITSf 		GPIOCON_PIN1_CONF_BITSf
#define GPFCON_GPF2_CONF_BITSf 		GPIOCON_PIN2_CONF_BITSf
#define GPFCON_GPF3_CONF_BITSf 		GPIOCON_PIN3_CONF_BITSf
#define GPFCON_GPF4_CONF_BITSf 		GPIOCON_PIN4_CONF_BITSf
#define GPFCON_GPF5_CONF_BITSf 		GPIOCON_PIN5_CONF_BITSf
#define GPFCON_GPF6_CONF_BITSf 		GPIOCON_PIN6_CONF_BITSf
#define GPFCON_GPF7_CONF_BITSf 		GPIOCON_PIN7_CONF_BITSf

#define GPFCON_INPUT				GPIOCON_PIN_CONF_VAL_0
#define GPFCON_OUTPUT				GPIOCON_PIN_CONF_VAL_1
#define GPFCON_EINT					GPIOCON_PIN_CONF_VAL_2			/* EINT0~EINT7 at GPFCON GPF0~GPF7 */
#define GPFCON_GPF0_EINT0			GPFCON_EINT
#define GPFCON_GPF1_EINT1			GPFCON_EINT
#define GPFCON_GPF2_EINT2			GPFCON_EINT
#define GPFCON_GPF3_EINT3			GPFCON_EINT
#define GPFCON_GPF4_EINT4			GPFCON_EINT
#define GPFCON_GPF5_EINT5			GPFCON_EINT
#define GPFCON_GPF6_EINT6			GPFCON_EINT
#define GPFCON_GPF7_EINT7			GPFCON_EINT
#define GPFCON_RESRVED				GPIOCON_PIN_CONF_VAL_3


#define GPFDAT_GPF0_DATA_BITSf		GPIODAT_PIN0_DATA_BITSf
#define GPFDAT_GPF1_DATA_BITSf		GPIODAT_PIN1_DATA_BITSf
#define GPFDAT_GPF2_DATA_BITSf		GPIODAT_PIN2_DATA_BITSf
#define GPFDAT_GPF3_DATA_BITSf		GPIODAT_PIN3_DATA_BITSf
#define GPFDAT_GPF4_DATA_BITSf		GPIODAT_PIN4_DATA_BITSf
#define GPFDAT_GPF5_DATA_BITSf		GPIODAT_PIN5_DATA_BITSf
#define GPFDAT_GPF6_DATA_BITSf		GPIODAT_PIN6_DATA_BITSf
#define GPFDAT_GPF7_DATA_BITSf		GPIODAT_PIN7_DATA_BITSf

/* GPIO_G register fields bits */
#define GPGCON_GPG0_CONF_BITSf		GPIOCON_PIN0_CONF_BITSf	
#define GPGCON_GPG1_CONF_BITSf 		GPIOCON_PIN1_CONF_BITSf 
#define GPGCON_GPG2_CONF_BITSf 		GPIOCON_PIN2_CONF_BITSf 
#define GPGCON_GPG3_CONF_BITSf 		GPIOCON_PIN3_CONF_BITSf 
#define GPGCON_GPG4_CONF_BITSf 		GPIOCON_PIN4_CONF_BITSf 
#define GPGCON_GPG5_CONF_BITSf 		GPIOCON_PIN5_CONF_BITSf 
#define GPGCON_GPG6_CONF_BITSf 		GPIOCON_PIN6_CONF_BITSf 
#define GPGCON_GPG7_CONF_BITSf 		GPIOCON_PIN7_CONF_BITSf 
#define GPGCON_GPG8_CONF_BITSf		GPIOCON_PIN8_CONF_BITSf	
#define GPGCON_GPG9_CONF_BITSf 		GPIOCON_PIN9_CONF_BITSf 
#define GPGCON_GPG10_CONF_BITSf 	GPIOCON_PIN10_CONF_BITSf
#define GPGCON_GPG11_CONF_BITSf 	GPIOCON_PIN11_CONF_BITSf
#define GPGCON_GPG12_CONF_BITSf 	GPIOCON_PIN12_CONF_BITSf
#define GPGCON_GPG13_CONF_BITSf 	GPIOCON_PIN13_CONF_BITSf
#define GPGCON_GPG14_CONF_BITSf 	GPIOCON_PIN14_CONF_BITSf
#define GPGCON_GPG15_CONF_BITSf 	GPIOCON_PIN15_CONF_BITSf

#define GPGCON_GPG0_CONF_START		(GPIOCON_PIN0_START_BIT	)
#define GPGCON_GPG1_CONF_START 		(GPIOCON_PIN1_START_BIT )
#define GPGCON_GPG2_CONF_START 		(GPIOCON_PIN2_START_BIT )
#define GPGCON_GPG3_CONF_START 		(GPIOCON_PIN3_START_BIT )
#define GPGCON_GPG4_CONF_START 		(GPIOCON_PIN4_START_BIT )
#define GPGCON_GPG5_CONF_START 		(GPIOCON_PIN5_START_BIT )
#define GPGCON_GPG6_CONF_START 		(GPIOCON_PIN6_START_BIT )
#define GPGCON_GPG7_CONF_START 		(GPIOCON_PIN7_START_BIT )
#define GPGCON_GPG8_CONF_START		(GPIOCON_PIN8_START_BIT	)
#define GPGCON_GPG9_CONF_START 		(GPIOCON_PIN9_START_BIT )
#define GPGCON_GPG10_CONF_START 	(GPIOCON_PIN10_START_BIT)
#define GPGCON_GPG11_CONF_START 	(GPIOCON_PIN11_START_BIT)
#define GPGCON_GPG12_CONF_START 	(GPIOCON_PIN12_START_BIT)
#define GPGCON_GPG13_CONF_START 	(GPIOCON_PIN13_START_BIT)
#define GPGCON_GPG14_CONF_START 	(GPIOCON_PIN14_START_BIT)
#define GPGCON_GPG15_CONF_START 	(GPIOCON_PIN15_START_BIT)

#define GPGCON_INPUT				GPIOCON_PIN_CONF_VAL_0
#define GPGCON_OUTPUT				GPIOCON_PIN_CONF_VAL_1
#define GPGCON_EINT					GPIOCON_PIN_CONF_VAL_2			/* EINT8~EINT23 at GPGCON GPF0~GPF15 */
#define GPGCON_LCD_PWRDN			GPIOCON_PIN_CONF_VAL_3
#define GPGCON_GPG0_EINT8			GPGCON_EINT
#define GPGCON_GPG1_EINT9			GPGCON_EINT
#define GPGCON_GPG2_EINT10			GPGCON_EINT
#define GPGCON_GPG3_EINT11			GPGCON_EINT
#define GPGCON_GPG4_EINT12			GPGCON_EINT
#define GPGCON_GPG5_EINT13			GPGCON_EINT
#define GPGCON_GPG6_EINT14			GPGCON_EINT
#define GPGCON_GPG7_EINT15			GPGCON_EINT
#define GPGCON_GPG8_EINT16			GPGCON_EINT
#define GPGCON_GPG9_EINT17			GPGCON_EINT
#define GPGCON_GPG10_EINT18			GPGCON_EINT
#define GPGCON_GPG11_EINT19			GPGCON_EINT
#define GPGCON_GPG12_EINT20			GPGCON_EINT
#define GPGCON_GPG13_EINT21			GPGCON_EINT
#define GPGCON_GPG14_EINT22			GPGCON_EINT
#define GPGCON_GPG15_EINT23			GPGCON_EINT
#define GPGCON_RESRVED				GPIOCON_PIN_CONF_VAL_3

#define GPGDAT_GPG0_DATA_BITSf		GPIODAT_PIN0_DATA_BITSf	
#define GPGDAT_GPG1_DATA_BITSf		GPIODAT_PIN1_DATA_BITSf	
#define GPGDAT_GPG2_DATA_BITSf		GPIODAT_PIN2_DATA_BITSf	
#define GPGDAT_GPG3_DATA_BITSf		GPIODAT_PIN3_DATA_BITSf	
#define GPGDAT_GPG4_DATA_BITSf		GPIODAT_PIN4_DATA_BITSf	
#define GPGDAT_GPG5_DATA_BITSf		GPIODAT_PIN5_DATA_BITSf	
#define GPGDAT_GPG6_DATA_BITSf		GPIODAT_PIN6_DATA_BITSf	
#define GPGDAT_GPG7_DATA_BITSf		GPIODAT_PIN7_DATA_BITSf	
#define GPGDAT_GPG8_DATA_BITSf		GPIODAT_PIN8_DATA_BITSf	
#define GPGDAT_GPG9_DATA_BITSf		GPIODAT_PIN9_DATA_BITSf	
#define GPGDAT_GPG10_DATA_BITSf		GPIODAT_PIN10_DATA_BITSf
#define GPGDAT_GPG11_DATA_BITSf		GPIODAT_PIN11_DATA_BITSf
#define GPGDAT_GPG12_DATA_BITSf		GPIODAT_PIN12_DATA_BITSf
#define GPGDAT_GPG13_DATA_BITSf		GPIODAT_PIN13_DATA_BITSf
#define GPGDAT_GPG14_DATA_BITSf		GPIODAT_PIN14_DATA_BITSf
#define GPGDAT_GPG15_DATA_BITSf		GPIODAT_PIN15_DATA_BITSf 


/* GPIO_H register fields bits */
#define GPHCON_GPH0_CONF_BITSf		GPIOCON_PIN0_CONF_BITSf	
#define GPHCON_GPH1_CONF_BITSf 		GPIOCON_PIN1_CONF_BITSf 
#define GPHCON_GPH2_CONF_BITSf 		GPIOCON_PIN2_CONF_BITSf
#define GPHCON_GPH3_CONF_BITSf 		GPIOCON_PIN3_CONF_BITSf 
#define GPHCON_GPH4_CONF_BITSf 		GPIOCON_PIN4_CONF_BITSf 
#define GPHCON_GPH5_CONF_BITSf 		GPIOCON_PIN5_CONF_BITSf 
#define GPHCON_GPH6_CONF_BITSf 		GPIOCON_PIN6_CONF_BITSf 
#define GPHCON_GPH7_CONF_BITSf 		GPIOCON_PIN7_CONF_BITSf 
#define GPHCON_GPH8_CONF_BITSf		GPIOCON_PIN8_CONF_BITSf	
#define GPHCON_GPH9_CONF_BITSf 		GPIOCON_PIN9_CONF_BITSf 
#define GPHCON_GPH10_CONF_BITSf 	GPIOCON_PIN10_CONF_BITSf


#define GPHCON_INPUT				GPIOCON_PIN_CONF_VAL_0
#define GPHCON_OUTPUT				GPIOCON_PIN_CONF_VAL_1
#define GPHCON_GPH0_NCTS0			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH1_NRTS0			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH2_TXD0			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH3_RXD0			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH4_TXD1			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH5_RXD1			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH6_TXD2			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH7_RXD2			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH8_UEXTCLK			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH9_CLKOUT0			GPIOCON_PIN_CONF_VAL_2
#define GPHCON_GPH10_CLKOUT1		GPIOCON_PIN_CONF_VAL_2

#define GPH_RESRVED					GPIOCON_PIN_CONF_VAL_3
#define GPHCON_GPH6_NRTS1			GPIOCON_PIN_CONF_VAL_3
#define GPHCON_GPH7_NCTS1			GPIOCON_PIN_CONF_VAL_3

#define GPHDAT_GPH0_DATA_BITSf		GPIODAT_PIN0_DATA_BITSf	
#define GPHDAT_GPH1_DATA_BITSf		GPIODAT_PIN1_DATA_BITSf	
#define GPHDAT_GPH2_DATA_BITSf		GPIODAT_PIN2_DATA_BITSf	
#define GPHDAT_GPH3_DATA_BITSf		GPIODAT_PIN3_DATA_BITSf	
#define GPHDAT_GPH4_DATA_BITSf		GPIODAT_PIN4_DATA_BITSf	
#define GPHDAT_GPH5_DATA_BITSf		GPIODAT_PIN5_DATA_BITSf	
#define GPHDAT_GPH6_DATA_BITSf		GPIODAT_PIN6_DATA_BITSf	
#define GPHDAT_GPH7_DATA_BITSf		GPIODAT_PIN7_DATA_BITSf	
#define GPHDAT_GPH8_DATA_BITSf		GPIODAT_PIN8_DATA_BITSf	
#define GPHDAT_GPH9_DATA_BITSf		GPIODAT_PIN9_DATA_BITSf	
#define GPHDAT_GPH10_DATA_BITSf		GPIODAT_PIN10_DATA_BITSf


#endif
