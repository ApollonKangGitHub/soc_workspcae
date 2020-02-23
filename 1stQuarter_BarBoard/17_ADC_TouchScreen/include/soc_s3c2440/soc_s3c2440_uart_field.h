#ifndef _SOC_S3C2440_UART_FIELD_H_
#define _SOC_S3C2440_UART_FIELD_H_

/* ########################### ULCONn UART line control register fields ############################ */
#define ULCON_INFRARED_MODE_BITSf					(0x3 << 6) /* default 0 */
#define ULCON_PARITY_MODE_BITSf						(0x3 << 3)	/* default 000 */
#define ULCON_NUM_OF_STOP_BITSf						(0x3 << 2)	/* default 0 */
#define ULCON_WORD_LEN_BITSf						(0x3 << 0)	/* default 00 */

#define ULCON_INFRARED_MODE_START					(6)
#define ULCON_PARITY_MODE_START						(3)
#define ULCON_NUM_OF_STOP_START						(2)
#define ULCON_WORD_LEN_START						(0)

#define ULCON_INFRARED_MODE_NORMAL					(0x0)
#define ULCON_INFRARED_MODE_TXRX					(0x1)

/*
 * 0xx = No parity 
 * 100 = Odd parity 
 * 101 = Even parity 
 * 110 = Parity forced/checked as 1 
 * 111 = Parity forced/checked as 0
*/
#define ULCON_PARITY_MODE_NO						(0x0)
#define ULCON_PARITY_MODE_ODD						(0x4)				
#define ULCON_PARITY_MODE_EVEN						(0x5)
#define ULCON_PARITY_MODE_FORCE1					(0x6)
#define ULCON_PARITY_MODE_FORCE0					(0x7)

#define ULCON_NUM_OF_STOP_1							(0x0) /* 1 stop bit */
#define ULCON_NUM_OF_STOP_2							(0x1) /* 2 stop bits */

#define ULCON_WORD_LEN_5							(0x0) /* 5 data bits */
#define ULCON_WORD_LEN_6							(0x1) /* 6 data bits */
#define ULCON_WORD_LEN_7							(0x2) /* 7 data bits */
#define ULCON_WORD_LEN_8							(0x3) /* 8 data bits */

/* ########################### UCONn UART control register fields ############################ */
#define UCON_CLOCK_SECTION_BITSf					(0x3 << 10)	/* choose PCLK,UEXTCLK,or FCLK/n */
#define UCON_TRANS_MODE_BITSf						(0x3 << 2)
#define UCON_RECV_MODE_BITSf						(0x3 << 0)

#define UCON_CLOCK_SECTION_START					(10)
#define UCON_TRANS_MODE_START						(2)
#define UCON_RECV_MODE_START						(0)

#define UCON_CLOCK_SECTION_VLAUE_0_PCLK				(0x0)
#define UCON_CLOCK_SECTION_VALUE_1_PCLK				(0x1)
#define UCON_CLOCK_SECTION_VALUE_2_UEXTCLK			(0x2)
#define UCON_CLOCK_SECTION_VALUE_3_FCLK_DIV_N		(0x3)

/* 
 * Transmit Mode or Receive Mode bits functions
 * 00 = Disable 
 * 01 = Interrupt request or polling mode 
 * 10 = DMA0 request (Only for UART0), 
 * 	    DMA3 request (Only for UART2) 
 * 11 = DMA1 request (Only for UART1)
*/
#define UCON_TRANS_MODE_DISABLE						(0x0)
#define UCON_TRANS_MODE_INTREQ_OR_POLL				(0x1)
#define UCON_TRANS_MODE_UART0_OR_2					(0x2)
#define UCON_TRANS_MODE_UART1						(0x3)

#define UCON_RECV_MODE_DISABLE						(0x0)
#define UCON_RECV_MODE_INTREQ_OR_POLL				(0x1)
#define UCON_RECV_MODE_UART0_OR_2					(0x2)
#define UCON_RECV_MODE_UART1						(0x3)

/* ########################### UTRSTAT UART TX/RX state register fields ############################ */
#define UTRSTAT_TRANS_EMTPY_BITSf					(0x2)		/* initial state is 1 */
#define UTRSTAT_TRANS_BUFF_EMPTY_BITSf				(0x1)		/* initial state is 1 */
#define UTRSTAT_RECV_BUFF_EMPTY_BITSf				(0x0)		/* initial state is 0 */

#define UTRSTAT_TRANS_EMTPY_START					(0x2)
#define UTRSTAT_TRANS_BUFF_EMPTY_START				(0x1)
#define UTRSTAT_RECV_BUFF_READY_START				(0x0)

/* 
 * Set to 1 automatically when the transmit buffer register has no valid data
 * to transmit and the transmit shift register is empty. 
 */
#define UTRSTAT_TRANS_EMTPY_NO						(0x0)	/* Not empty */
#define UTRSTAT_TRANS_EMTPY_YES						(0x1)	/* Transmitter empty */
/* In Non-FIFO mode */
#define UTRSTAT_TRANS_BUFF_EMPTY_NO					(0x0)	/* The buffer register is not empty */
#define UTRSTAT_TRANS_BUFF_EMPTY_YES				(0x1)	/* Empty */
#define UTRSTAT_RECV_BUFF_READY_NO					(0x0)	/* Empty */
#define UTRSTAT_RECV_BUFF_READY_YES					(0x1)	/* The buffer register has a received data */

#endif