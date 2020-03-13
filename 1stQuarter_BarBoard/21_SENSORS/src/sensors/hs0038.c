#include <tools.h>
#include <timer.h>
#include <interrupt.h>
#include <necDataQueue.h>
#include <soc_s3c2440_public.h>

#define HS0038_NDEC_SAVE_TRY_MAX 		(10)

/* NEC协议脉冲宽度定义 */
#define HS0038_NEC_PLUSE_WIDTH_BASE		(563)								/* 562.5us，NEC脉冲宽度基本单位 */
#define HS0038_NEC_PLUSE_WIDTH_DELTA	(HS0038_NEC_PLUSE_WIDTH_BASE / 2)	/* 脉冲宽度可接受变化范围 */
#define HS0038_NEC_PREAMBLE_CODE_LOW	(HS0038_NEC_PLUSE_WIDTH_BASE * 16)	/* NEC前导码低电平宽度 */
#define HS0038_NEC_PREAMBLE_CODE_HIGH	(HS0038_NEC_PLUSE_WIDTH_BASE * 8)	/* NEC前导码高电平宽度 */
#define HS0038_NEC_REPEAT_CODE_HIGH		(HS0038_NEC_PLUSE_WIDTH_BASE * 4)	/* NEC重复码高电平宽度 */
#define HS0038_NEC_DATA1_CODE_HIGH		(HS0038_NEC_PLUSE_WIDTH_BASE * 3)	/* NEC数据'1'高电平宽度 */
#define HS0038_NEC_DATA0_CODE_HIGH		(HS0038_NEC_PLUSE_WIDTH_BASE * 1)	/* NEC数据'0'高电平宽度 */
#define HS0038_NEC_DATA_CODE_LOW		(HS0038_NEC_PLUSE_WIDTH_BASE * 1)	/* NEC数据低电平宽度 */
#define HS0038_NEC_END_PLUSE_LOW		(HS0038_NEC_PLUSE_WIDTH_BASE * 1)	/* NEC结束脉冲低电平宽度 */

static uint64 gLastTime = 0;

/* 使用EINT中断、GPF1引脚 */

/* 读取数据 */
#define HS0038_GPIO_DATA_GET()	((GPFDATr & GPFDAT_GPF1_DATA_BITSf) ? (1) : (0))

/* 检查脉冲是否在可接受的精度范围内 */
static BOOL hs0038_data_check(uint32 idealWidth, int actualWidth)
{
	/* 实际宽度超出理想宽度的可接受范围，则返回FALSE */
	if ((actualWidth > (idealWidth + HS0038_NEC_PLUSE_WIDTH_DELTA))
		|| (actualWidth < (idealWidth - HS0038_NEC_PLUSE_WIDTH_DELTA)))
	{
		return FALSE;
	}

	return TRUE;
}

/* 获取NEC数据 */
necReadRet_t hs0038_read_nec_data(uint8 * necAddr, uint8 * necData)
{
	int index = 0;
	uint32 val = 0;
	uint8 addr = 0x0;
	uint8 data = 0x0;
	uint8 addr_check = 0x0;
	uint8 data_check = 0x0;
	necDataQueue_t eventData;
	
	while (TRUE) 
	{
		/* 0、取NEC中断数据,不是低脉冲则认为是垃圾数据 */
		if ((OK == necDataQUeue_get_data_timeout(&eventData, 1000000)) && (0 == eventData.pol))
		{
			/* 1、判断是不是前导码的低脉冲 */
			if (hs0038_data_check(HS0038_NEC_PREAMBLE_CODE_LOW, eventData.duration))
			{
				/* 2、是前导码的低脉冲，再读取下一个高脉冲 */
				if (OK == necDataQUeue_get_data_timeout(&eventData, 10000))
				{
					/* 3、高脉冲有效性判断 */
					if ((1 == eventData.pol) \
						&& hs0038_data_check(HS0038_NEC_PREAMBLE_CODE_HIGH, eventData.duration))
					{
						/* 4、前导码高脉冲4.5ms，重复解析32bit数据 */
						for (index = 0; index < 0; index++)
						{
							/* 5、读取0.56ms的数据低脉冲 */
							if (OK == necDataQUeue_get_data_timeout(&eventData, 10000))
							{
								/* 6、判断是不是有效的数据低脉冲 */
								if ((0 == eventData.pol) \
									&& hs0038_data_check(HS0038_NEC_DATA_CODE_LOW, eventData.duration))
								{
									/* 7、读取数据高脉冲 */
									if (OK == necDataQUeue_get_data_timeout(&eventData, 10000))
									{
										/* 8、判断是不是有效的数据高脉冲（数据0和数据1） */
										if ((1 == eventData.pol) \
											&& hs0038_data_check(HS0038_NEC_DATA1_CODE_HIGH, eventData.duration))
										{	
											/* 数据1的高脉冲 */
											val |= (1 << index);
										}
										else if ((1 == eventData.pol) \
											&& hs0038_data_check(HS0038_NEC_DATA0_CODE_HIGH, eventData.duration))
										{
											/* 数据0的高脉冲 */
											val &= ~(1 << index);
										}
										else
										{
											print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
											return nec_read_ret_unknow;
										}
									}
									else
									{
										print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
										return nec_read_ret_timeout;
									}
								}
								else
								{
									print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
									return nec_read_ret_unknow;
								}
							}
							else
							{
								print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
								return nec_read_ret_timeout;
							}
						}

						/* 得到32bit数据，判断数据是否正确 */
						addr = val & 0xFF;
						data = (val >> 16) & 0xFF;
						addr_check = (val >> 8) & 0xFF;
						data_check = (val >> 24) & 0xFF;
						addr_check = ~addr_check;
						data_check = ~data_check;
						
						/* 32bit（4Byte）分别为：addr、addr取反、data、data取反 */
						if ((addr != addr_check) || (data != data_check))
						{
							print_screen("\r\n addr:%x, addr_check:%x, data:%x, data_check:%x",
								addr, addr_check, data, data_check);
							print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
							return nec_read_ret_crc;
						}

						/* 校验通过 */
						*necAddr = addr;
						*necData = data;
						print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
						return nec_read_ret_ok;
					}
					else if ((1 == eventData.pol) \
						&& hs0038_data_check(HS0038_NEC_REPEAT_CODE_HIGH, eventData.duration))
					{
						/* 重复码高脉冲2.25ms */
						print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
						return nec_read_ret_repl;
					}
					else
					{
						print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
						return nec_read_ret_unknow;
					}
				}
				else
				{
					print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
					return nec_read_ret_timeout;
				}
			}
			else
			{
				print_screen("\r\n[%s-%d]: duration:%d, pol:%d", __FUNCTION__, __LINE__, eventData.duration, eventData.pol);
				return nec_read_ret_unknow;
			}
		}
		else
		{	
			return nec_read_ret_timeout;
		}
	}
}

/* 配置为中断引脚 */
static void hs0038_gpio_data_cfg_pin_eint(void)
{
	GPFCONr &= ~GPFCON_GPF1_CONF_BITSf;
	GPFCONr |= (GPFCON_EINT << GPFCON_GPF1_CONF_START);
}

/* 配置为输入引脚 */
static void hs0038_gpio_data_cfg_pin_input(void)
{
	GPFCONr &= ~GPFCON_GPF1_CONF_BITSf;
	GPFCONr |= (GPFCON_INPUT << GPFCON_GPF1_CONF_START);
}

/* 红外检测传感器GPIO初始化 */
static hs0038_gpio_init(void)
{
	hs0038_gpio_data_cfg_pin_eint();

	/* EINT1设置为双边沿触发 */
	SOC_S3C2440_REG_BITS_SET(EXTINT0r, \
		EXTINT0_CON_EINT1_START_BIT, EXTINT_EINT_MASK, EXTINT_CON_BOTH_EDGE_TRIGGER);	
}

/* NEC数据中断处理函数 */
void * hs0038_interrupt_handle(void * pArgv)
{
	int ret = OK;
	int tryCnt = 0;
	uint64 curTime = 0;
	sys_time_t sysTime;
	necDataQueue_t eventData;
		
	(void)timer_get_sys_time(&sysTime);
	curTime = sysTime.__usecStamp__;

	/* 
	 * 脉冲宽度为两次中断时间记录之差
	 * 跳变产生中断，若当前为低电平，
	 * 则中断前为高电平，因此中断为当前电平取反
	 */
	eventData.duration = curTime - gLastTime;

try_again:
	eventData.pol = !(HS0038_GPIO_DATA_GET());
	ret = necDataQUeue_put_data(&eventData);
	if ((ret == ERROR) && (tryCnt < HS0038_NDEC_SAVE_TRY_MAX))
	{
		print_screen("\r\n queue is full, try again, tryCnt:%d!", tryCnt);
		goto try_again;
	}
	
	gLastTime = curTime;
}

/* NEC数据信息打印 */
void hs0038_raw_data_dump(void)
{	
	int ret = OK;
	necDataQueue_t eventData;

	set_buffer((uint8 *)&eventData, 0, sizeof(necDataQueue_t));
	ret = necDataQUeue_get_data(&eventData);

	/* 打印脉宽和极性 */
	if (ret != ERROR)
	{
		print_screen("\r\n pulse width: %d, polarity:%d", 
			eventData.duration, eventData.pol);
	}
}

/* 初始化，配置GPIO，注册中断 */
void hs0038_init(void)
{
	/*
	 * 记录脉冲发生系统时间，与上次中断系统时间比较
	 * 得到一次数据中断的脉冲宽度，写入NEC数据循环队列
     */
    hs0038_gpio_init();

	/* 注册中断处理函数 */
	interrupt_register(interrupt_type_EXT_INT1, hs0038_interrupt_handle);
}

/* detach硬件操作 */
void hs0038_detach(void)
{
	hs0038_gpio_data_cfg_pin_input();
	interrupt_unregister(interrupt_type_EXT_INT1);
}

