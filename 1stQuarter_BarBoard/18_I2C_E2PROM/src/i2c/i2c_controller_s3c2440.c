#include <i2c_controller_s3c2440.h>
#include <i2c_controller.h>
#include <tools.h>
#include <log.h>
#include <interrupt.h>
#include <soc_s3c2440_public.h>

static i2c_msg_t * gCurI2cOperator = NULL;
static BOOL gLastAckSend = TRUE;	/* 默认都是需要发送ACK的 */

/* I2C操作生效延时 */
static i2c_delay_s3c2440(volatile int delay)
{
	while(delay--);
}

/* 读结束寄存器设置 */
static void i2c_interrupt_handle_read_over_s3c2440(void)
{
	/* 写0x90关闭，选择RX模式，关闭RX，clear中断 */
	IICSTATr = ((IICSTAT_SERIAL_CTRL_ENABLE << IICSTAT_SERIAL_CTRL_START_BIT)
			| (IICSTAT_WRRD_OP_SINGLE_STOP << IICSTAT_WRRD_OP_SINGLE_START_BIT)
			| (IICSTAT_MODE_SELECT_MASTER_RX << IICSTAT_MODE_SELECT_START_BIT));
	SOC_S3C2440_REG_BIT_SET(IICCONr, IICCON_INTERRUPT_PEND_START_BIT, IICCON_INTERRUPT_PEND_CLR);
	i2c_delay_s3c2440(1000);
}

/* 写结束寄存器设置 */
static void i2c_interrupt_handle_write_over_s3c2440(void)
{
	/* 写0xD0关闭，选择的TX模式，关闭TX，clear中断 */
	IICSTATr = ((IICSTAT_SERIAL_CTRL_ENABLE << IICSTAT_SERIAL_CTRL_START_BIT)
			| (IICSTAT_WRRD_OP_SINGLE_STOP << IICSTAT_WRRD_OP_SINGLE_START_BIT)
			| (IICSTAT_MODE_SELECT_MASTER_TX << IICSTAT_MODE_SELECT_START_BIT));
	SOC_S3C2440_REG_BIT_SET(IICCONr, IICCON_INTERRUPT_PEND_START_BIT, IICCON_INTERRUPT_PEND_CLR);
	i2c_delay_s3c2440(1000);	
}

/* clear pending状态 */
static void i2c_interrupt_clear_pend_s3c2440(void)
{
	SOC_S3C2440_REG_BIT_SET(IICCONr, IICCON_INTERRUPT_PEND_START_BIT, IICCON_INTERRUPT_PEND_CLR);
}

/* 是否接收到ACK信号 */
static BOOL i2c_interrupt_receive_ack_s3c2440(void) 
{
	return (IICSTAT_LAST_RECEIVE_ACK == SOC_S3C2440_REG_BIT_GET(IICSTATr, IICSTAT_LAST_RECEIVE_STATRT_BIT));
}

/* 由不同I2C设备决定是否发送ACK（AT24CXX在读的时候，不需要master发送最后一个ACK） */
void i2c_controller_last_ack_control_s3c2440(BOOL send)
{
	gLastAckSend = send;
}

static void i2c_receive_last_ack_check_s3c2440(void)
{
	/* 最后一个数据不发送ACK */
	if ((gCurI2cOperator->cnt == gCurI2cOperator->len - 1) && !gLastAckSend)
	{
		IICCONr &= ~(IICCON_BUS_RXTX_MODE_REPLY_ACK << IICCON_BUS_ACK_ENABLE_START);
	}
	else if (gCurI2cOperator->cnt < gCurI2cOperator->len - 1)
	{
		IICCONr |= (IICCON_BUS_RXTX_MODE_REPLY_ACK << IICCON_BUS_ACK_ENABLE_START);
	}
	else
	{	
		/* 发出停止信号 */
		IICSTATr = ((IICSTAT_SERIAL_CTRL_ENABLE << IICSTAT_SERIAL_CTRL_START_BIT)
				| (IICSTAT_WRRD_OP_SINGLE_STOP << IICSTAT_WRRD_OP_SINGLE_START_BIT)
				| (IICSTAT_MODE_SELECT_MASTER_RX << IICSTAT_MODE_SELECT_START_BIT));
		IICCONr |= (IICCON_BUS_RXTX_MODE_REPLY_ACK << IICCON_BUS_ACK_ENABLE_START);
		i2c_delay_s3c2440(1000);
	}
}

/* I2C中断处理函数 */
static void * i2c_interrupt_handle_s3c2440(void * pArgv)
{
	uint32 iiccon;
	/* 
	 * 每TX/RX完一个数据产生一个中断
	 * 第一次中断是表示“已经传输完Slave Address”
	 */

	if (!gCurI2cOperator)
	{		
		return NULL;
	}

	gCurI2cOperator->cnt++;

	/* 判断当前传输模式（TX/RX）*/	
	if (I2C_CONTROLLER_READ == gCurI2cOperator->flags)
	{
		/* 
		 * 第一个中断表示从设备准备传数据，先判断ACK是否存在，
		 * 不存在则结束，存在则清除中断，恢复传输，等待第一个数据
		 */
		if (0 == gCurI2cOperator->cnt)
		{
			/* 判断ACK未收到则出错返回 */
			if (!i2c_interrupt_receive_ack_s3c2440())
			{
				/* 无ACK则设备不存在，标记error */
				gCurI2cOperator->error = TRUE;
				
				print_screen("\r\n rx ack error");
				
				/* 停止传输，清除中断pend状态等待结束 */
				(void)i2c_interrupt_handle_read_over_s3c2440();

				return NULL;
			}

			/* 清除pending，启动数据传输 */
			i2c_interrupt_clear_pend_s3c2440();
			return NULL;
		} 		

		/* 第二个中断开始表示从设备数据发送过来 */
		if (gCurI2cOperator->cnt <= gCurI2cOperator->len)
		{
			/* 读取数据 */
			gCurI2cOperator->buf[gCurI2cOperator->cnt - 1] = IICDSr;
		}

		/* 最后一个ACK发送检查 */
		i2c_receive_last_ack_check_s3c2440();
		/* 清除pending中断 */
		i2c_interrupt_clear_pend_s3c2440();
		/* 恢复默认值 */
		i2c_controller_last_ack_control_s3c2440(TRUE);
		return NULL;		
	}
	else if (I2C_CONTROLLER_WRITE == gCurI2cOperator->flags)
	{
		/* 
		 * 第一个中断是发送设备地址后产生的，判断是否存在ACK，
		 * 有ACK则设备存在发送准备第一个数据，无ACK则表示无设备直接返回，
		 * 第二个个中断开始则是数据发送完成中断，准备发送下一个数据
		 */
		if (!i2c_interrupt_receive_ack_s3c2440())
		{
			/* 无ACK则设备不存在，或者发送失败，标记error */
			gCurI2cOperator->error = TRUE;
			print_screen("\r\n tx ack error");

			/* 结束IIC传输 */
			i2c_interrupt_handle_write_over_s3c2440();
			return NULL;
	 	}

		/* 数据传输 */
		if (gCurI2cOperator->cnt < gCurI2cOperator->len)
		{
			 /* 写数据，写统计自加，清除Pending位 */
			IICDSr = gCurI2cOperator->buf[gCurI2cOperator->cnt];
			i2c_interrupt_clear_pend_s3c2440();
		}
		else
		{
			/* 写数据长度足够则		 结束IIC传输 */
			i2c_interrupt_handle_write_over_s3c2440();
		}
	}

	return NULL;
}

/* 初始化 */
static int i2c_controller_init_s3c2440(void)
{
	/* 1、时钟初始化 */
	/* [7] : IIC-bus acknowledge enable bit, 1-enable in rx mode
	 * [6] : 时钟源, 0: IICCLK = fPCLK /16; 1: IICCLK = fPCLK /512
	 * [5] : 1-enable interrupt
	 * [4] : 读出为1时表示中断发生了, 写入0来清除并恢复I2C操作
	 * [3:0] : Tx clock = IICCLK/(IICCON[3:0]+1).
	 * Tx Clock = 100khz = 50Mhz/16/(IICCON[3:0]+1)
	 */
	IICCONr = (IICCON_BUS_RXTX_MODE_REPLY_ACK << IICCON_BUS_ACK_ENABLE_START)
			| (IICCON_BUS_CLK_SOURCE_FPCLK_16 << IICCON_BUS_CLK_SOURCE_FPCLK_START) 
			| (IICCON_BUS_INTERRUPR_ENABLE << IICCON_BUS_INTERRUPR_ENABLE_START) 
			| (30 << IICCON_BUS_TRANSMIT_CLK_PRESCALER_START);

	/* 2、配置GPIO引脚用于I2C功能 */
	GPECONr &= ~(GPIOCON_PIN14_CONF_BITSf | GPIOCON_PIN15_CONF_BITSf);
	GPECONr |= ((GPECON_GPE14_IICSCL << GPECON_GPE14_START) 
			| (GPECON_GPE15_IICSDA << GPECON_GPE15_START));

	/* 注册中断处理函数 */
	return interrupt_register(interrupt_type_INT_IIC, i2c_interrupt_handle_s3c2440);
}

/* 发送数据 */
static int i2c_controller_master_transmit_s3c2440(i2c_msg_t * msgs)
{
	msgs->cnt = -1;
	msgs->error = FALSE;
	gCurI2cOperator = msgs;
	
	/* 1. 配置为 master tx mode, TX mode, 在ACK周期释放SDA */
	IICCONr |= (IICCON_BUS_RXTX_MODE_REPLY_ACK << IICCON_BUS_ACK_ENABLE_START); 
	IICSTATr = (IICSTAT_SERIAL_CTRL_ENABLE << IICSTAT_SERIAL_CTRL_START_BIT);
		
	/* 2. 把从设备地址写入IICDS */
	IICDSr = ((msgs->addr << IICADDR_ADDR_START_BIT)
			| (IICADDR_RD_WR_FLAG_WR << IICADDR_RD_WR_FLAG_START));

	/* 3. IICSTAT = 0xf0 , 数据即被发送出去, 将导致中断产生 */
	IICSTATr = ((IICSTAT_SERIAL_CTRL_ENABLE << IICSTAT_SERIAL_CTRL_START_BIT)
			| (IICSTAT_WRRD_OP_SINGLE_START << IICSTAT_WRRD_OP_SINGLE_START_BIT)
			| (IICSTAT_MODE_SELECT_MASTER_TX << IICSTAT_MODE_SELECT_START_BIT));

	while ((!msgs->error) && (msgs->cnt != msgs->len));
	if (msgs->error)
	{
		return ERROR;
	}

	return OK;
}

/* 读取数据 */
static int i2c_controller_master_receive_s3c2440(i2c_msg_t * msgs)
{
	msgs->cnt = -1;
	msgs->error = FALSE;
	gCurI2cOperator = msgs;

	/* 1. 配置为 master tx mode, TX mode, 在ACK周期回应ACK */
	IICCONr |= (IICCON_BUS_RXTX_MODE_REPLY_ACK << IICCON_BUS_ACK_ENABLE_START); 
	IICSTATr = (IICSTAT_SERIAL_CTRL_ENABLE << IICSTAT_SERIAL_CTRL_START_BIT);
		 
	/* 2. 把从设备地址写入IICDS */
	IICDSr = ((msgs->addr << IICADDR_ADDR_START_BIT)
		 	| (IICADDR_RD_WR_FLAG_RD << IICADDR_RD_WR_FLAG_START));

	/* 3. IICSTAT = 0xb0 , 从设备地址即被发送出去, 将导致中断产生 */
	IICSTATr = ((IICSTAT_SERIAL_CTRL_ENABLE << IICSTAT_SERIAL_CTRL_START_BIT)
			| (IICSTAT_WRRD_OP_SINGLE_START << IICSTAT_WRRD_OP_SINGLE_START_BIT)
			| (IICSTAT_MODE_SELECT_MASTER_RX << IICSTAT_MODE_SELECT_START_BIT));

	while ((!msgs->error) && (msgs->cnt != msgs->len));
	if (msgs->error)
	{	
	 	return ERROR;
	}

	return OK;
}

/* 主设备传输数据 */
static int i2c_controller_master_xfer_s3c2440(i2c_msg_t * msgs, int num)
{
	int i = 0;
	int ret = 0;
	
	for (i = 0; i < num; i++)
	{
		if (I2C_CONTROLLER_READ == msgs[i].flags)
		{
			ret = i2c_controller_master_receive_s3c2440(&msgs[i]);
		}
		else if (I2C_CONTROLLER_WRITE == msgs[i].flags)
		{
			ret = i2c_controller_master_transmit_s3c2440(&msgs[i]);
		}

		if (ret != OK)
		{
			print_screen("\r\n I2C operator %x failed!!", msgs[i].flags);
			return ret;
		}
	}

	return OK;
}

/* 驱动结构 */
i2c_controller_t gI2cControllerDrvS3c2440 = {
	.i2c_controller_init = i2c_controller_init_s3c2440,
	.i2c_master_xfer = i2c_controller_master_xfer_s3c2440,
	.i2c_last_ack_send = i2c_controller_last_ack_control_s3c2440,
	.i2c_controller_name = "s3c2440 i2c controller"
};

