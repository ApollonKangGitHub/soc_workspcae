#ifndef _I2C_CONTROLLER_S3C2440_H_
#define _I2C_CONTROLLER_S3C2440_H_

/*
 * Acknowledge generation 
 * 	[7] IIC-bus acknowledge enable bit. 0: Disable , 1: Enable 
 * 	In Tx mode, the IICSDA is free in the ack time. 
 * 	In Rx mode, the IICSDA is L in the ack time. 
 * 
 * Tx clock source selection 
 * 	[6] Source clock of IIC-bus transmit clock prescaler selection bit. 
 * 	0: IICCLK = fPCLK /16 
 * 	1: IICCLK = fPCLK /512 
 * 
 * Tx/Rx Interrupt 
 * 	[5] IIC-Bus Tx/Rx interrupt enable/disable bit. 
 * 	0: Disable, 1: Enable 
 * 
 * Interrupt pending flag
 * 	[4] IIC-bus Tx/Rx interrupt pending flag. 
 * 	This bit cannot be written to 1. When this bit is read as 1, the IICSCL is 
 * 	tied to L and the IIC is stopped. To resume the operation, clear this bit as 0. 
 * 	0: 	1) No interrupt pending (when read). 
 * 		2) Clear pending condition & Resume the operation (when write). 
 * 	1: 	1) Interrupt is pending (when read) 
 * 		2) N/A (when write) 
 * 
 * Transmit clock value
 * 	[3:0] IIC-Bus transmit clock prescaler. 
 * 	IIC-Bus transmit clock frequency is determined by this 
 * 	4-bit prescaler value, according to the following 
 * 	formula: Tx clock = IICCLK/(IICCON[3:0]+1).
 */
#define IICCON_BUS_RXTX_MODE_REPLY_ACK			(1)
#define IICCON_BUS_ACK_ENABLE_START			  	(7)
#define IICCON_BUS_TRANSMIT_CLK_PRESCALER_START	(0)
#define IICCON_BUS_CLK_SOURCE_FPCLK_512			(1)
#define IICCON_BUS_CLK_SOURCE_FPCLK_16			(0)
#define IICCON_BUS_CLK_SOURCE_FPCLK_START		(6)
#define IICCON_BUS_INTERRUPR_ENABLE				(1)
#define IICCON_BUS_INTERRUPR_DISABLE			(0)
#define IICCON_BUS_INTERRUPR_ENABLE_START		(5)
/*
 * IIC-bus master/slave Tx/Rx mode select bits. 
 * 00: Slave receive mode 
 * 01: Slave transmit mode 
 * 10: Master receive mode 
 * 11: Master transmit mode
 */
#define IICSTAT_MODE_SELECT_START_BIT	(6)
#define IICSTAT_MODE_SELECT_MASK		(0x3)
#define IICSTAT_MODE_SELECT_SLAVE_RX	(0x0)
#define IICSTAT_MODE_SELECT_SLAVE_TX	(0x1)
#define IICSTAT_MODE_SELECT_MASTER_RX	(0x2)
#define IICSTAT_MODE_SELECT_MASTER_TX	(0x3)

#define IICSTAT_BUSY_STATUS_START_BIT	(5)
#define IICSTAT_BUSY_STATUS_NOBUSY		(0)
#define IICSTAT_BUSY_STATUS_BUSY		(1)

#define IICSTAT_WRRD_OP_SINGLE_START_BIT	(5)
#define IICSTAT_WRRD_OP_SINGLE_START		(1)
#define IICSTAT_WRRD_OP_SINGLE_STOP			(0)

#define IICSTAT_SERIAL_CTRL_START_BIT	(4)
#define IICSTAT_SERIAL_CTRL_DISABLE		(0)
#define IICSTAT_SERIAL_CTRL_ENABLE		(1)

/*
 * Last-received bit status flag [0] IIC-bus last-received bit status flag bit. 
 * 0: Last-received bit is 0 (ACK was received). 
 * 1: Last-received bit is 1 (ACK was not received).
 */
#define IICSTAT_LAST_RECEIVE_STATRT_BIT	(0)
#define IICSTAT_LAST_RECEIVE_ACK		(0)
#define IICSTAT_LAST_RECEIVE_NOACK		(1)

#define IICADDR_RD_WR_FLAG_START		(0)
#define IICADDR_RD_WR_FLAG_WR			(0)
#define IICADDR_RD_WR_FLAG_RD			(1)

#define IICADDR_ADDR_START_BIT			(1)
#define IICADDR_ADDR_MASK				(0x7F)

#define IICCON_INTERRUPT_PEND_START_BIT	(4)
#define IICCON_INTERRUPT_PEND_CLR		(0)

#endif	/* _I2C_CONTROLLER_S3C2440_H_ */
