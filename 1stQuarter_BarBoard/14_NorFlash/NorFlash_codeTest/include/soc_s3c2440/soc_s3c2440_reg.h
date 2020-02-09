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

#define ADDR_REFRESHr		(0x48000024)
#define ADDR_BANKSIZEr		(0x48000028)
#define ADDR_MRSRB6r		(0x4800002c)
#define ADDR_MRSRB7r		(0x48000030)

#define BWSCONr				__REG_VALUE(ADDR_BWSCONr) 		/* Bus width & wait status control register */	

#define BANKCON0r			__REG_VALUE(ADDR_BANKCON0r)		/* Bank 0 control register,reset value 0x0700 */
#define BANKCON1r			__REG_VALUE(ADDR_BANKCON1r)     /* Bank 1 control register,reset value 0x0700 */
#define BANKCON2r			__REG_VALUE(ADDR_BANKCON2r)     /* Bank 2 control register,reset value 0x0700 */
#define BANKCON3r			__REG_VALUE(ADDR_BANKCON3r)     /* Bank 3 control register,reset value 0x0700 */
#define BANKCON4r			__REG_VALUE(ADDR_BANKCON4r)     /* Bank 4 control register,reset value 0x0700 */
#define BANKCON5r			__REG_VALUE(ADDR_BANKCON5r)     /* Bank 5 control register,reset value 0x0700 */

#define BANKCON6r			__REG_VALUE(ADDR_BANKCON6r)     /* Bank 6 control register,reset value 0x0700 */
#define BANKCON7r			__REG_VALUE(ADDR_BANKCON7r)     /* Bank 7 control register,reset value 0x0700 */

#define REFRESHr			__REG_VALUE(ADDR_REFRESHr)		/* SDRAM refresh control register */
#define BANKSIZEr			__REG_VALUE(ADDR_BANKSIZEr)		/* Flexible bank size register */
#define MRSRB6r				__REG_VALUE(ADDR_MRSRB6r)		/* Mode register set register bank6 */
#define MRSRB7r				__REG_VALUE(ADDR_MRSRB7r)		/* Mode register set register bank7 */

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

/* ################################################# INterrupt register ####################################################### */
#define ADDR_EXTINT0r			(0x56000088)
#define ADDR_EXTINT1r			(0x5600008C)
#define ADDR_EXTINT2r			(0x56000090)

#define ADDR_EINTMASKr			(0x560000A4)
#define ADDR_EINTPENDr			(0x560000A8)

#define ADDR_SRCPNDr			(0x4A000000)
#define ADDR_INTMASKr			(0x4A000008)
#define ADDR_PRIORITYr			(04A000000C)
#define ADDR_INTPNDr			(0x4A000010)
#define ADDR_INTOFFSETr			(0x4A000014)


#define EXTINT0r				__REG_VALUE(ADDR_EXTINT0r)			/* External interrupt control register 0 */
#define EXTINT1r				__REG_VALUE(ADDR_EXTINT1r)			/* External interrupt control register 1 */
#define EXTINT2r				__REG_VALUE(ADDR_EXTINT2r)			/* External interrupt control register 2 */

#define EINTMASKr				__REG_VALUE(ADDR_EINTMASKr)			/* External Interrupt Mask Register */
#define EINTPENDr				__REG_VALUE(ADDR_EINTPENDr)			/* External interrupt pending register */

#define SRCPNDr					__REG_VALUE(ADDR_SRCPNDr)			/* Indicate the interrupt request status.Reset Value:0x00000000 */
#define INTMASKr				__REG_VALUE(ADDR_INTMASKr)			/* Determine which interrupt source is masked.Reset Value:0xFFFFFFFF */
#define PRIORITYr				__REG_VALUE(ADDR_PRIORITYr)			/* IRQ priority control register */
#define INTPNDr					__REG_VALUE(ADDR_INTPNDr)			/* Indicate the interrupt request status */
#define INTOFFSETr				__REG_VALUE(ADDR_INTOFFSETr)		/* Indicate the IRQ interrupt request source */

/* ################################################################################################################################## */


/* ################################################# Timer register ####################################################### */

#define ADDR_TCFG0r				(0x51000000)
#define ADDR_TCFG1r				(0x51000004)
#define ADDR_TCONr				(0x51000008)

#define ADDR_TCNTB0r			(0x5100000C)
#define ADDR_TCMPB0r			(0x51000010)
#define ADDR_TCNTO0r			(0x51000014)

#define ADDR_TCNTB1r			(0x51000018)
#define ADDR_TCMPB1r			(0x5100001C)
#define ADDR_TCNTO1r			(0x51000020)

#define ADDR_TCNTB2r			(0x51000024)
#define ADDR_TCMPB2r			(0x51000028)
#define ADDR_TCNTO2r			(0x5100002C)

#define ADDR_TCNTB3r			(0x51000030)
#define ADDR_TCMPB3r			(0x51000034)
#define ADDR_TCNTO3r			(0x51000038)

#define ADDR_TCNTB4r			(0x5100003C)
#define ADDR_TCNTO4r			(0x51000040)


#define TCFG0r					__REG_VALUE(ADDR_TCFG0r)			/* Configures the two 8-bit prescalers */
#define TCFG1r					__REG_VALUE(ADDR_TCFG1r)			/* 5-MUX & DMA mode selection register */
#define TCONr					__REG_VALUE(ADDR_TCONr)				/* Timer control register */

#define TCNTB0r					__REG_VALUE(ADDR_TCNTB0r)			/* Timer 0 count buffer register */
#define TCMPB0r					__REG_VALUE(ADDR_TCMPB0r)			/* Timer 0 compare buffer register */
#define TCNTO0r					__REG_VALUE(ADDR_TCNTO0r)			/* Timer 0 count observation register */

#define TCNTB1r					__REG_VALUE(ADDR_TCNTB1r)			/* Timer 1 count buffer register */
#define TCMPB1r					__REG_VALUE(ADDR_TCMPB1r)			/* Timer 1 compare buffer register */
#define TCNTO1r					__REG_VALUE(ADDR_TCNTO1r)			/* Timer 1 count observation register */

#define TCNTB2r					__REG_VALUE(ADDR_TCNTB2r)			/* Timer 2 count buffer register */
#define TCMPB2r					__REG_VALUE(ADDR_TCMPB2r)			/* Timer 2 compare buffer register */
#define TCNTO2r					__REG_VALUE(ADDR_TCNTO2r)			/* Timer 2 count observation register */

#define TCNTB3r					__REG_VALUE(ADDR_TCNTB3r)			/* Timer 3 count buffer register */
#define TCMPB3r					__REG_VALUE(ADDR_TCMPB3r)			/* Timer 3 compare buffer register */
#define TCNTO3r					__REG_VALUE(ADDR_TCNTO3r)			/* Timer 3 count observation register */

#define TCNTB4r					__REG_VALUE(ADDR_TCNTB4r)			/* Timer 4 count buffer register */
#define TCNTO4r					__REG_VALUE(ADDR_TCNTO4r)			/* Timer 4 count observation register */



/* ################################################################################################################################## */


#endif /* _SOC_S3C2440_REG_H_ */
