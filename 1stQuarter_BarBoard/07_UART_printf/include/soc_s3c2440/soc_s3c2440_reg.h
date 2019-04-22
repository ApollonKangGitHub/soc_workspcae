#ifndef _SOC_S3C2440_REG_H_
#define _SOC_S3C2440_REG_H_

#define __REG_VALUE(addr) (*((volatile unsigned int *)(addr)))
#define __REG_VALUE_BYTE(addr) (*((volatile unsigned char *)(addr)))

/* The suffix 'r' means that the macro is a register. */

/* ################################################# UART register ####################################################### */
#define ULCON0r				__REG_VALUE(0x50000000)			/* UART channel 0 line control register */
#define ULCON1r				__REG_VALUE(0x50004000)			/* UART channel 1 line control register */
#define ULCON2r				__REG_VALUE(0x50008000)			/* UART channel 2 line control register */

#define UCON0r				__REG_VALUE(0x50000004)			/* UART channel 0 control register */
#define UCON1r				__REG_VALUE(0x50004004)			/* UART channel 1 control register */
#define UCON2r				__REG_VALUE(0x50008004)			/* UART channel 2 control register */

#define UTRSTAT0r			__REG_VALUE(0x50000010)			/* UART channel 0 Tx/Rx status register,reset value is 6 */
#define UTRSTAT1r			__REG_VALUE(0x50004010)			/* UART channel 1 Tx/Rx status register,reset value is 6 */
#define UTRSTAT2r			__REG_VALUE(0x50008010)			/* UART channel 2 Tx/Rx status register,reset value is 6 */

/* 
 * (L): The endian mode is Little endian. 
 * (B): The endian mode is Big endian.
 */
#define UTXH0Lr				__REG_VALUE_BYTE(0x50000020)			/* UART channel 0 transmit buffer register for Little endian */
#define UTXH0Br				__REG_VALUE_BYTE(0x50000023)			/* UART channel 0 transmit buffer register for Big endian */
#define UTXH1Lr				__REG_VALUE_BYTE(0x50004020)			/* UART channel 1 transmit buffer register for Little endian */
#define UTXH1Br				__REG_VALUE_BYTE(0x50004023)			/* UART channel 1 transmit buffer register for Big endian */
#define UTXH2Lr				__REG_VALUE_BYTE(0x50008020)			/* UART channel 2 transmit buffer register for Little endian */
#define UTXH2Br				__REG_VALUE_BYTE(0x50008023)			/* UART channel 2 transmit buffer register for Big endian */
#define URXH0Lr				__REG_VALUE_BYTE(0x50000024)			/* UART channel 0 receive buffer register for Little endian */
#define URXH0Br				__REG_VALUE_BYTE(0x50000027)			/* UART channel 0 receive buffer register for Big endian */
#define URXH1Lr				__REG_VALUE_BYTE(0x50004024)			/* UART channel 1 receive buffer register for Little endian */
#define URXH1Br				__REG_VALUE_BYTE(0x50004027)			/* UART channel 1 receive buffer register for Big endian */
#define URXH2Lr				__REG_VALUE_BYTE(0x50008024)			/* UART channel 2 receive buffer register for Little endian */
#define URXH2Br				__REG_VALUE_BYTE(0x50008027)			/* UART channel 2 receive buffer register for Big endian */

#define UBRDIV0r			__REG_VALUE(0x50000028)			/* Baud rate divisior register 0 */
#define UBRDIV1r			__REG_VALUE(0x50004028)			/* Baud rate divisior register 1 */
#define UBRDIV2r			__REG_VALUE(0x50008028)			/* Baud rate divisior register 2 */

/* ################################################# WatchDog register ####################################################### */
#define WTCONr				__REG_VALUE(0x53000000)			/* watchDog timer control register */

/* ################################################# I/O port register ####################################################### */
#define GPFCONr				__REG_VALUE(0x56000050)			/* GPF control register */
#define GPFDATr				__REG_VALUE(0x56000054)			/* GPF data register */
#define GPFUPr				__REG_VALUE(0x56000058)			/* GPF pins Pull-up constrol register,0 is ebable */
#define GPFRESRVEDr			__REG_VALUE(0x5600005C)			/* Reserved  */


#define GPGCONr				__REG_VALUE(0x56000060)			/* GPG control register */
#define GPGDATr 			__REG_VALUE(0x56000064)			/* GPG data register */
#define GPGUPr				__REG_VALUE(0x56000068)			/* GPG pins Pull-up constrol register,0 is enable */
#define GPGRESRVEDr			__REG_VALUE(0x5600006C)			/* Reserved  */


#define GPHCONr				__REG_VALUE(0x56000070)			/* GPH control register */
#define GPHDATr 			__REG_VALUE(0x56000074)			/* GPH data register */
#define GPHUPr				__REG_VALUE(0x56000078)			/* GPH pins Pull-up constrol register,0 is enable */
#define GPHRESRVEDr			__REG_VALUE(0x5600007C)			/* Reserved  */
/* ################################################################################################################################## */

#endif /* _SOC_S3C2440_REG_H_ */
