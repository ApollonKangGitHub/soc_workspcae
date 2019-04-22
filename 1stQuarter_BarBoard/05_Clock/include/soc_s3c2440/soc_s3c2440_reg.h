#ifndef _SOC_S3C2440_REG_H_
#define _SOC_S3C2440_REG_H_

#define __REG_VALUE(addr) (*((volatile unsigned int *)(addr)))

/* The suffix 'r' means that the macro is a register. */
#define WTCONr	__REG_VALUE(0x53000000)		/* watch dog register */

#define GPFCONr	__REG_VALUE(0x56000050)		/* GPF config register */
#define GPFDATr	__REG_VALUE(0x56000054)		/* GPF data register */
#define GPFUPr	__REG_VALUE(0x56000058)		/* GPF pins Pull-up constrol register,0 is ebable */

#define GPGCONr	__REG_VALUE(0x56000060)		/* GPG config register */
#define GPGDATr __REG_VALUE(0x56000064)		/* GPG data register */
#define GPGUPr	__REG_VALUE(0x56000068)		/* GPG pins Pull-up constrol register,0 is enable */
#endif /* _SOC_S3C2440_REG_H_ */
