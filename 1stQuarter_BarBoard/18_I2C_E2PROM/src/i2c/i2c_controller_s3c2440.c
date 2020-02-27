#include <i2c_controller_s3c2440.h>
#include <i2c_controller.h>
#include <tools.h>
#include <interrupt.h>
#include <soc_s3c2440_public.h>

static i2c_msg_t * gCurI2cOperator = NULL;

/* I2C中断处理函数 */
static void * i2c_interrupt_handle_s3c2440(void * pArgv)
{
	uint32 iicstat = IICSTATr;

	/* 
	 * 每TX/RX完一个数据产生一个中断
	 * 第一次中断是表示“已经传输完Slave Address”
	 */

	if (!gCurI2cOperator)
	{		
		return NULL;
	}

	/* 判断当前传输模式（TX/RX）*/	
	if (I2C_CONTROLLER_READ == gCurI2cOperator->flags)
	{

	}
	else if (I2C_CONTROLLER_WRITE == gCurI2cOperator->flags)
	{
		/* 
		 * 第一个中断是发送设备地址后产生的，判断是否存在ACK，
		 * 有ACK则设备存在发送准备第一个数据，无ACK则表示无设备，
		 * 第二个开始个中断则是数据发送完成中断，准备发送下一个数据
		 */
		 if (0 == gCurI2cOperator->cnt)
		 {
			if (IICSTAT_LAST_RECEIVE_NOACK == SOC_S3C2440_REG_BIT_GET(iicstat, IICSTAT_LAST_RECEIVE_STATRT_BIT))
			{
				/* 停止传输，清除中断pend状态 */
				SOC_S3C2440_REG_BIT_SET(IICSTATr, IICSTAT_WR_OP_SINGLE_START_BIT, IICSTAT_WR_OP_SINGLE_STOP); 
				SOC_S3C2440_REG_BIT_SET(IICCONr, IICCON_INTERRUPT_PEND_START_BIT, IICCON_INTERRUPT_PEND_CLR);
			}
			else 
			{
				return NULL;
			}
		 }
	}
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
	IICCONr = (0 << 6) | (1 << 5) | (30 << 0);

	/* 注册中断处理函数 */
	return interrupt_register(interrupt_type_INT_IIC, i2c_interrupt_handle_s3c2440);
}

/* 发送数据 */
static int i2c_controller_master_transmit_s3c2440(i2c_msg_t * msgs)
{
	msgs->cnt = 0 ;
	gCurI2cOperator = msgs;
	
	/* 
	 * 1、设置I2C主从模式 Master RX
	 * 2、发送start标志
	 * 3、写7bit的I2C地址
	 * 4、发送1bit的写标志
	 * 5、启动传输,数据将被发送出去，导致产生中断，中断中发送数据
	 */
	 SOC_S3C2440_REG_BITS_SET(IICSTATr, IICSTAT_MODE_SELECT_START_BIT, IICSTAT_MODE_SELECT_MASK, IICSTAT_MODE_SELECT_MASTER_TX);
	 SOC_S3C2440_REG_BITS_SET(IICADDr, IICADDR_ADDR_START_BIT, IICADDR_ADDR_MASK, msgs->addr);
	 SOC_S3C2440_REG_BIT_SET(IICADDr, IICADDR_RD_WR_FLAG_START, IICADDR_RD_WR_FLAG_WR);
	 SOC_S3C2440_REG_BIT_SET(IICSTATr, IICSTAT_WR_OP_SINGLE_START_BIT, IICSTAT_WR_OP_SINGLE_START); 
	 SOC_S3C2440_REG_BIT_SET(IICSTATr, IICSTAT_SERIAL_CTRL_START_BIT, IICSTAT_SERIAL_CTRL_ENABLE);

	 while (msgs->cnt != msgs->len);
}

/* 读取数据 */
static int i2c_controller_master_receive_s3c2440(i2c_msg_t * msgs)
{
	msgs->cnt = 0 ;
	gCurI2cOperator = msgs;
	
	/* 
	 * 1、设置I2C主从模式 Master RX
	 * 2、发送start标志
	 * 3、写7bit的I2C地址
	 * 4、发送1bit的读标志
	 * 5、启动传输，从地址将被发送出去，导致产生中断，中断中读取数据
	 */
	 SOC_S3C2440_REG_BITS_SET(IICSTATr, IICSTAT_MODE_SELECT_START_BIT, IICSTAT_MODE_SELECT_MASK, IICSTAT_MODE_SELECT_MASTER_RX);
	 SOC_S3C2440_REG_BIT_SET(IICSTATr, IICSTAT_WR_OP_SINGLE_START_BIT, IICSTAT_WR_OP_SINGLE_START); 
	 SOC_S3C2440_REG_BITS_SET(IICADDr, IICADDR_ADDR_START_BIT, IICADDR_ADDR_MASK, msgs->addr);
	 SOC_S3C2440_REG_BIT_SET(IICADDr, IICADDR_RD_WR_FLAG_START, IICADDR_RD_WR_FLAG_RD);
	 SOC_S3C2440_REG_BIT_SET(IICSTATr, IICSTAT_SERIAL_CTRL_START_BIT, IICSTAT_SERIAL_CTRL_ENABLE);

	 while (msgs->cnt != msgs->len);
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
			ret = i2c_controller_master_transmit_s3c2440(&msgs[i]);
		}
		else if (I2C_CONTROLLER_WRITE == msgs[i].flags)
		{
			ret = i2c_controller_master_receive_s3c2440(&msgs[i]);
		}

		if (ret != OK)
		{
			return ret;
		}
	}

	return OK;
}

/* 驱动结构 */
i2c_controller_t gI2cControllerDrvS3c2440 = {
	.i2c_controller_init = i2c_controller_init_s3c2440,
	.i2c_master_xfer = i2c_controller_master_xfer_s3c2440,
	.i2c_controller_name = "s3c2440 i2c controller"
};

