#ifndef _SOC_S3C2440_FIELD_H_
#define _SOC_S3C2440_FIELD_H_

/* The suffix 'f' means that the macro is the bits of a field in a register. */

/* GPFCON register fields bits */
#define GPFCON_GPF0_CONF_BITSf	(0x3 << 0)
#define GPFCON_GPF1_CONF_BITSf 	(0x3 << 2)
#define GPFCON_GPF2_CONF_BITSf 	(0x3 << 4)
#define GPFCON_GPF3_CONF_BITSf 	(0x3 << 6)
#define GPFCON_GPF4_CONF_BITSf 	(0x3 << 8)
#define GPFCON_GPF5_CONF_BITSf 	(0x3 << 10)
#define GPFCON_GPF6_CONF_BITSf 	(0x3 << 12)
#define GPFCON_GPF7_CONF_BITSf 	(0x3 << 14)

#define GPFCON_INPUT_BITS	(0x0)
#define GPFCON_OUTPUT_BITS	(0x1)
#define GPFCON_EINT_BITS	(0x2)			/* EINT0~EINT7 at GPFCON GPF0~GPF7 */
#define GPFCON_REVERSE_BITS	(0x3)

/* GPFDAT register fields bits */
#define GPFDAT_GPF0_DATA_BITSf	(0x1 << 0)
#define GPFDAT_GPF1_DATA_BITSf	(0x1 << 1)
#define GPFDAT_GPF2_DATA_BITSf	(0x1 << 2)
#define GPFDAT_GPF3_DATA_BITSf	(0x1 << 3)
#define GPFDAT_GPF4_DATA_BITSf	(0x1 << 4)
#define GPFDAT_GPF5_DATA_BITSf	(0x1 << 5)
#define GPFDAT_GPF6_DATA_BITSf	(0x1 << 6)
#define GPFDAT_GPF7_DATA_BITSf	(0x1 << 7)

/* GPGCON register fields bits, GPG[15:13] must be selected as Input in NAND boot mode. */
#define GPGCON_GPG0_CONF_BITSf		(0x3 << 0)
#define GPGCON_GPG1_CONF_BITSf 		(0x3 << 2)
#define GPGCON_GPG2_CONF_BITSf 		(0x3 << 4)
#define GPGCON_GPG3_CONF_BITSf 		(0x3 << 6)
#define GPGCON_GPG4_CONF_BITSf 		(0x3 << 8)
#define GPGCON_GPG5_CONF_BITSf 		(0x3 << 10)
#define GPGCON_GPG6_CONF_BITSf 		(0x3 << 12)
#define GPGCON_GPG7_CONF_BITSf 		(0x3 << 14)
#define GPGCON_GPG8_CONF_BITSf		(0x3 << 16)
#define GPGCON_GPG9_CONF_BITSf 		(0x3 << 18)
#define GPGCON_GPG10_CONF_BITSf 	(0x3 << 20)
#define GPGCON_GPG11_CONF_BITSf 	(0x3 << 22)
#define GPGCON_GPG12_CONF_BITSf 	(0x3 << 24)
#define GPGCON_GPG13_CONF_BITSf 	(0x3 << 26)
#define GPGCON_GPG14_CONF_BITSf 	(0x3 << 28)
#define GPGCON_GPG15_CONF_BITSf 	(0x3 << 30)

#define GPGCON_INPUT_BITS	(0x0)
#define GPGCON_OUTPUT_BITS	(0x1)
#define GPGCON_EINT_BITS	(0x2)		/* EINT8~EINT23 at GPGCON GPG0~GPG15 */
#define GPGCON_REVERSE_BITS	(0x3)

/* GPGDAT register fields bits */
#define GPGDAT_GPG0_DATA_BITSf	(0x1 << 0)
#define GPGDAT_GPG1_DATA_BITSf	(0x1 << 1)
#define GPGDAT_GPG2_DATA_BITSf	(0x1 << 2)
#define GPGDAT_GPG3_DATA_BITSf	(0x1 << 3)
#define GPGDAT_GPG4_DATA_BITSf	(0x1 << 4)
#define GPGDAT_GPG5_DATA_BITSf	(0x1 << 5)
#define GPGDAT_GPG6_DATA_BITSf	(0x1 << 6)
#define GPGDAT_GPG7_DATA_BITSf	(0x1 << 7) 
#define GPGDAT_GPG8_DATA_BITSf	(0x1 << 8)
#define GPGDAT_GPG9_DATA_BITSf	(0x1 << 9)
#define GPGDAT_GPG10_DATA_BITSf	(0x1 << 10)
#define GPGDAT_GPG11_DATA_BITSf	(0x1 << 11)
#define GPGDAT_GPG12_DATA_BITSf	(0x1 << 12)
#define GPGDAT_GPG13_DATA_BITSf	(0x1 << 13)
#define GPGDAT_GPG14_DATA_BITSf	(0x1 << 14)
#define GPGDAT_GPG15_DATA_BITSf	(0x1 << 15) 


#endif /* _SOC_S3C2440_FIELD_H_ */
