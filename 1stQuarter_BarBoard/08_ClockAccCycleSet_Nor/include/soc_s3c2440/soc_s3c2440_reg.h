#ifndef _SOC_S3C2440_REG_H_
#define _SOC_S3C2440_REG_H_

#define __REG_VALUE(addr) (*((volatile unsigned int *)(addr)))
#define __REG_VALUE_BYTE(addr) (*((volatile unsigned char *)(addr)))

/* The suffix 'r' means that the macro is a register. */

/* ########################################## Memory Controller register ################################################# */
#define ADDR_BWSCONr		(0x48000000)

#define ADDR_BANKCON0r		(0x48000004)						
#define ADDR_BANKCON1r		(0x48000008)						
#define ADDR_BANKCON2r		(0x4800000c)						
#define ADDR_BANKCON3r		(0x48000010)						
#define ADDR_BANKCON4r		(0x48000014)						
#define ADDR_BANKCON5r		(0x48000018)

#define ADDR_BANKCON6r		(0x4800001c)						
#define ADDR_BANKCON7r		(0x48000020)
		
#define BWSCONr				__REG_VALUE(ADDR_BWSCONr) 		/* Bus width & wait status control register */	

#define BANKCON0r			__REG_VALUE(ADDR_BANKCON0r)		/* Bank 0 control register,reset value 0x0700 */
#define BANKCON1r			__REG_VALUE(ADDR_BANKCON1r)     /* Bank 1 control register,reset value 0x0700 */
#define BANKCON2r			__REG_VALUE(ADDR_BANKCON2r)     /* Bank 2 control register,reset value 0x0700 */
#define BANKCON3r			__REG_VALUE(ADDR_BANKCON3r)     /* Bank 3 control register,reset value 0x0700 */
#define BANKCON4r			__REG_VALUE(ADDR_BANKCON4r)     /* Bank 4 control register,reset value 0x0700 */
#define BANKCON5r			__REG_VALUE(ADDR_BANKCON5r)     /* Bank 5 control register,reset value 0x0700 */

#define BANKCON6r			__REG_VALUE(ADDR_BANKCON6r)     /* Bank 6 control register,reset value 0x0700 */
#define BANKCON7r			__REG_VALUE(ADDR_BANKCON7r)     /* Bank 7 control register,reset value 0x0700 */

#define ADDR_GSTATUS0r		(0x560000ac)			
#define GSTATUS0r			__REG_VALUE(ADDR_GSTATUS0r)		/* External pin status Not define,reset not defined */
/* ################################################# UART register ####################################################### */
#define ADDR_ULCON0r		(0x50000000)
#define ADDR_ULCON1r		(0x50004000)
#define ADDR_ULCON2r		(0x50008000)
#define ULCON0r				__REG_VALUE(ADDR_ULCON0r)			/* UART channel 0 line control register */
#define ULCON1r				__REG_VALUE(ADDR_ULCON1r)			/* UART channel 1 line control register */
#define ULCON2r				__REG_VALUE(ADDR_ULCON2r)			/* UART channel 2 line control register */

#define ADDR_UCON0r			(0x50000004)
#define ADDR_UCON1r			(0x50004004)
#define ADDR_UCON2r			(0x50008004)
#define UCON0r				__REG_VALUE(ADDR_UCON0r)			/* UART channel 0 control register */
#define UCON1r				__REG_VALUE(ADDR_UCON1r)			/* UART channel 1 control register */
#define UCON2r				__REG_VALUE(ADDR_UCON2r)			/* UART channel 2 control register */

#define ADDR_UTRSTAT0r		(0x50000010)
#define ADDR_UTRSTAT1r		(0x50004010)
#define ADDR_UTRSTAT2r		(0x50008010)
#define UTRSTAT0r			__REG_VALUE(ADDR_UTRSTAT0r)			/* UART channel 0 Tx/Rx status register,reset value is 6 */
#define UTRSTAT1r			__REG_VALUE(ADDR_UTRSTAT1r)			/* UART channel 1 Tx/Rx status register,reset value is 6 */
#define UTRSTAT2r			__REG_VALUE(ADDR_UTRSTAT2r)			/* UART channel 2 Tx/Rx status register,reset value is 6 */

/* 
 * (L): The endian mode is Little endian. 
 * (B): The endian mode is Big endian.
 */
#define ADDR_UTXH0Lr		(0x50000020)
#define ADDR_UTXH0Br		(0x50000023)
#define ADDR_UTXH1Lr		(0x50004020)
#define ADDR_UTXH1Br		(0x50004023)
#define ADDR_UTXH2Lr		(0x50008020)
#define ADDR_UTXH2Br		(0x50008023)
#define ADDR_URXH0Lr		(0x50000024)
#define ADDR_URXH0Br		(0x50000027)
#define ADDR_URXH1Lr		(0x50004024)
#define ADDR_URXH1Br		(0x50004027)
#define ADDR_URXH2Lr		(0x50008024)
#define ADDR_URXH2Br		(0x50008027)
#define UTXH0Lr				__REG_VALUE_BYTE(ADDR_UTXH0Lr)			/* UART channel 0 transmit buffer register for Little endian */
#define UTXH0Br				__REG_VALUE_BYTE(ADDR_UTXH0Br)			/* UART channel 0 transmit buffer register for Big endian */
#define UTXH1Lr				__REG_VALUE_BYTE(ADDR_UTXH1Lr)			/* UART channel 1 transmit buffer register for Little endian */
#define UTXH1Br				__REG_VALUE_BYTE(ADDR_UTXH1Br)			/* UART channel 1 transmit buffer register for Big endian */
#define UTXH2Lr				__REG_VALUE_BYTE(ADDR_UTXH2Lr)			/* UART channel 2 transmit buffer register for Little endian */
#define UTXH2Br				__REG_VALUE_BYTE(ADDR_UTXH2Br)			/* UART channel 2 transmit buffer register for Big endian */
#define URXH0Lr				__REG_VALUE_BYTE(ADDR_URXH0Lr)			/* UART channel 0 receive buffer register for Little endian */
#define URXH0Br				__REG_VALUE_BYTE(ADDR_URXH0Br)			/* UART channel 0 receive buffer register for Big endian */
#define URXH1Lr				__REG_VALUE_BYTE(ADDR_URXH1Lr)			/* UART channel 1 receive buffer register for Little endian */
#define URXH1Br				__REG_VALUE_BYTE(ADDR_URXH1Br)			/* UART channel 1 receive buffer register for Big endian */
#define URXH2Lr				__REG_VALUE_BYTE(ADDR_URXH2Lr)			/* UART channel 2 receive buffer register for Little endian */
#define URXH2Br				__REG_VALUE_BYTE(ADDR_URXH2Br)			/* UART channel 2 receive buffer register for Big endian */

#define ADDR_UBRDIV0r		(0x50000028)
#define ADDR_UBRDIV1r       (0x50004028)
#define ADDR_UBRDIV2r       (0x50008028)
#define UBRDIV0r			__REG_VALUE(ADDR_UBRDIV0r)				/* Baud rate divisior register 0 */
#define UBRDIV1r			__REG_VALUE(ADDR_UBRDIV1r)				/* Baud rate divisior register 1 */
#define UBRDIV2r			__REG_VALUE(ADDR_UBRDIV2r)				/* Baud rate divisior register 2 */

/* ################################################# WatchDog register ####################################################### */
#define ADDR_WTCONr			(0x53000000)
#define WTCONr				__REG_VALUE(ADDR_WTCONr)			/* watchDog timer control register */

#define ADDR_GPFCONr		(0x56000050)
#define ADDR_GPFDATr		(0x56000054)
#define ADDR_GPFUPr			(0x56000058)
#define ADDR_GPFRESRVEDr	(0x5600005C)
#define GPFCONr				__REG_VALUE(ADDR_GPFCONr)			/* GPF control register */
#define GPFDATr				__REG_VALUE(ADDR_GPFDATr)			/* GPF data register */
#define GPFUPr				__REG_VALUE(ADDR_GPFUPr)			/* GPF pins Pull-up constrol register,0 is ebable */
#define GPFRESRVEDr			__REG_VALUE(ADDR_GPFRESRVEDr)		/* Reserved  */

#define ADDR_GPGCONr		(0x56000060)
#define ADDR_GPGDATr		(0x56000064)
#define ADDR_GPGUPr			(0x56000068)
#define ADDR_GPGRESRVEDr	(0x5600006C)
#define GPGCONr				__REG_VALUE(ADDR_GPGCONr)			/* GPG control register */
#define GPGDATr 			__REG_VALUE(ADDR_GPGDATr)			/* GPG data register */
#define GPGUPr				__REG_VALUE(ADDR_GPGUPr)			/* GPG pins Pull-up constrol register,0 is enable */
#define GPGRESRVEDr			__REG_VALUE(ADDR_GPGRESRVEDr)		/* Reserved  */

#define ADDR_GPHCONr		(0x56000070)
#define ADDR_GPHDATr		(0x56000074)
#define ADDR_GPHUPr			(0x56000078)
#define ADDR_GPHRESRVEDr	(0x5600007C)
#define GPHCONr				__REG_VALUE(ADDR_GPHCONr)			/* GPH control register */
#define GPHDATr 			__REG_VALUE(ADDR_GPHDATr)			/* GPH data register */
#define GPHUPr				__REG_VALUE(ADDR_GPHUPr	)			/* GPH pins Pull-up constrol register,0 is enable */
#define GPHRESRVEDr			__REG_VALUE(ADDR_GPHRESRVEDr)		/* Reserved  */
/* ################################################################################################################################## */

#endif /* _SOC_S3C2440_REG_H_ */
