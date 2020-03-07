#include <tools.h>
#include <log.h>
#include <soc_s3c2440_public.h>

#define DHT11_INTERRUPT_SLEEP_CALL	"dht11 read"

/* 使用GPG5作用dht11的DATA引脚 */

/* 控制GPIO读取DHT11的数据 
 * 1. 主机发出至少18MS的低脉冲: start信号
 * 2. start信号变为高, 20-40us之后, dht11会拉低总线维持80us
 *    然后拉高80us: 回应信号
 * 3. 之后就是数据, 逐位发送
 *    bit0 : 50us低脉冲, 26-28us高脉冲
 *    bit1 : 50us低脉冲, 70us高脉冲
 * 4. 数据有40bit: 8bit湿度整数数据+8bit湿度小数数据
 *                 +8bit温度整数数据+8bit温度小数数据
 *                 +8bit校验和
 */


/* GPIO基本操作 */
static void dht11_gpio_data_cfg_pin_output(void)
{
	GPGCONr &= ~GPGCON_GPG5_CONF_BITSf;
	GPGCONr |= (GPGCON_OUTPUT << GPGCON_GPG5_CONF_START);
}

static void dht11_gpio_data_cfg_pin_input(void)
{
	GPGCONr &= ~GPGCON_GPG5_CONF_BITSf;
	GPGCONr |= (GPGCON_INPUT << GPGCON_GPG5_CONF_START);
}

/* 拉低或拉高 */
static void dht11_gpio_data_set(BOOL high)
{
	if (high)
		GPGDATr |= GPGDAT_GPG5_DATA_BITSf;
	else
		GPGDATr &= ~GPGDAT_GPG5_DATA_BITSf;
}

/* 读取数据 */
#define DTH11_GPIO_DATA_GET()	((GPGDATr & GPGDAT_GPG5_DATA_BITSf) ? (1) : (0))

/* 等待高低电平指定超时时间 */
static int dht11_wait_for_val(int val, int timeout_us)
{
	volatile int wait = 0;
	volatile int timeout = timeout_us;
	
	while (--timeout)
	{
		wait = DTH11_GPIO_DATA_GET();
		if (val == wait) {
			return OK;
		}
		tool_udelay(1);
	}
	return ERROR;
}

/* 启动拉低等待至少18ms然后释放引脚作为输入，即发送start信号 */
static void dht11_start(void)
{
	dht11_gpio_data_set(0);
	tool_mdelay(20);
	dht11_gpio_data_cfg_pin_input();
}

/* 延时等待ACK，未接DHT11时读取无数据 */
static int dht11_wait_ack(void)
{
	/* 发送start释放引脚后至少等待40us */
	tool_udelay(60);

	/* 读取引脚电平,低为等到ACK */
	return DTH11_GPIO_DATA_GET();
}

/* 读取一个字节 */
static int dht11_recv_byte(void)
{
	int i;
	int data = 0;

	for (i = 0; i < 8; i++)
	{
		if (ERROR == dht11_wait_for_val(1, 1000))
		{
			print_screen("\r\ndht11 wait for high data err[i=%d]!\n\r", i);
			return ERROR;
		}
	
		tool_udelay(40);
		data <<= 1;
		if (1 == DTH11_GPIO_DATA_GET()) {
			data |= 1;
		}
		
		if (ERROR == dht11_wait_for_val(0, 1000))
		{
			print_screen("\r\ndht11 wait for low data err[i=%d, data:%x]!\n\r", i, data);
			return ERROR;
		}
	}
	
	return data;
}

int dht11_read(uint32 * humidity, uint32 * temperature)
{
	uint8 humHigh, humLow;
	uint8 tempHigh, tempLow;
	uint8 checkSum;
	volatile uint32 intmask = 0;
	
	interrupt_sleep(DHT11_INTERRUPT_SLEEP_CALL);

	dht11_start();

	/* 等待ACK */
	if (0 != dht11_wait_ack())
	{
		print_screen("\r\nDHT11 no have ack, error!");
		goto ret_err_wake_up;
	}

	/* 等待高低电平的回应信号，ACK之后低电平至少80us，高电平至少80us */
	if (OK != dht11_wait_for_val(1, 1000))
	{
		print_screen("dht11 wait for ack high error!\n\r");
		goto ret_err_wake_up;
	}

	if (OK != dht11_wait_for_val(0, 1000))
	{
		print_screen("dht11 wait for ack low error!\n\r");
		goto ret_err_wake_up;
	}	

	/* 等到低电平则到达数据阶段（共5byte数据） */
	humHigh = dht11_recv_byte();
	humLow = dht11_recv_byte();
	tempHigh = dht11_recv_byte();
	tempLow = dht11_recv_byte();
	checkSum = dht11_recv_byte();

	/* 读取完成后拉高1s（读取周期） */
	dht11_gpio_data_cfg_pin_output();
	dht11_gpio_data_set(1);

	/* 校验(校验值只返回整数部分（即高8bit）) */
	if (humHigh + humLow + tempHigh + tempLow == checkSum)
	{
		*humidity = humHigh;
		*temperature = tempHigh;
		tool_mdelay(1500);
	}
	else
	{
		print_screen("\r\n read dht11 check failed!!");
		goto ret_err_wake_up;
	}
	
	interrupt_wake_up(DHT11_INTERRUPT_SLEEP_CALL);
	return OK;
	
ret_err_wake_up:
	interrupt_wake_up(DHT11_INTERRUPT_SLEEP_CALL);
	return ERROR;
}


/* 初始化拉高，延时至少1s等待稳定 */
void dht11_init(void)
{
	interrupt_sleep(DHT11_INTERRUPT_SLEEP_CALL);
	dht11_gpio_data_cfg_pin_output();
	dht11_gpio_data_set(1);
	tool_mdelay(1500);
	interrupt_wake_up(DHT11_INTERRUPT_SLEEP_CALL);
}

