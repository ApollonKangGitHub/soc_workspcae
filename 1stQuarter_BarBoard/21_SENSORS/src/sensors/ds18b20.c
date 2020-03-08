#include <tools.h>
#include <soc_s3c2440_public.h>

/* 使用GPG6作用ds18b20的DATA引脚 */

/*
 * 控制GPIO读取DS18B20的数据或向DS18B20写入数据
 * 访问流程：
 *     1、initization
 *     2、send 1 Byte  rom command
 *     3、send 1 Byte function command
 *
 */

/* rom commands */
#define DS18B20_CMD_SEARCH_ROM    (0xF0)
#define DS18B20_CMD_READ_ROM      (0x33)
#define DS18B20_CMD_MATCH_ROM     (0x55)
#define DS18B20_CMD_SKIP_ROM      (0xCC)
#define DS18B20_CMD_ALARM_ROM     (0xEC)

/* functions commands */
#define DS18B20_CMD_CONVERT_TEAMPERATURE 	(0x44)
#define DS18B20_CMD_WRITE_SCRATCHPAD     	(0x4E)
#define DS18B20_CMD_READ_SCRATCHPAD      	(0xBE)
#define DS18B20_CMD_COPY_SCRATCHPAD      	(0x48)
#define DS18B20_CMD_RECALL_EEPROM        	(0xB8)
#define DS18B20_CMD_READ_POWER_SUPPLY    	(0xB4)

/* rom/ram字节长度 */
#define DS18B20_ROM_MAX_LEN		(8)
#define DS18B20_RAM_MAX_LEN  	(9)

/* 精度信息在RAM中的Byte位置 */
#define DS18B20_RAM_PRECISION_BYTE	(4)
#define DS18B20_IS_PRECSION_9_BIT(ram)		(((ram[DS18B20_RAM_PRECISION_BYTE] >> 5) & 0x3) == 0x0)
#define DS18B20_IS_PRECSION_10_BIT(ram)		(((ram[DS18B20_RAM_PRECISION_BYTE] >> 5) & 0x3) == 0x1)
#define DS18B20_IS_PRECSION_11_BIT(ram)		(((ram[DS18B20_RAM_PRECISION_BYTE] >> 5) & 0x3) == 0x2)
#define DS18B20_IS_PRECSION_12_BIT(ram)		(((ram[DS18B20_RAM_PRECISION_BYTE] >> 5) & 0x3) == 0x3)

/* 温度值在RAM中的Byte位置 */
#define DS18B20_RAM_TEMPERATURE_LSB	(0)
#define DS18B20_RAM_TEMPERATURE_MSB	(1)
/* MSB的高5bit为符号位，为1表示负数 */
#define DS18B20_SIGN_BIT_START		(3)

/* 读取数据 */
#define DS18B20_GPIO_DATA_GET()	((GPGDATr & GPGDAT_GPG6_DATA_BITSf) ? (1) : (0))

/* 输出引脚，CPU输出信号到DS18B20 */
static void ds18b20_gpio_data_cfg_pin_output(void)
{
	GPGCONr &= ~GPGCON_GPG6_CONF_BITSf;
	GPGCONr |= (GPGCON_OUTPUT << GPGCON_GPG6_CONF_START);
}

/* 输入引脚,DS18B20输入信号到CPU */
static void ds18b20_gpio_data_cfg_pin_input(void)
{
	GPGCONr &= ~GPGCON_GPG6_CONF_BITSf;
	GPGCONr |= (GPGCON_INPUT << GPGCON_GPG6_CONF_START);
}

/* 拉低或拉高 */
static void ds18b20_gpio_data_set(BOOL high)
{
	if (high)
		GPGDATr |= GPGDAT_GPG6_DATA_BITSf;
	else
		GPGDATr &= ~GPGDAT_GPG6_DATA_BITSf;
}

/* CPU控制 1-wire BUS 发出数据 */
static ds18b20_set_val_udelay(BOOL high, int us)
{
	ds18b20_gpio_data_cfg_pin_output();
	ds18b20_gpio_data_set(high);
	tool_udelay(us);
}

/* 释放总线一定时长（总线释放后恢复高电平） */
static ds18b20_bus_release_us(int us)
{
	ds18b20_gpio_data_cfg_pin_input();
	tool_udelay(us);
}

/* DS18B20读写初始化 */
static int ds18b20_internal_initization(void)
{
	uint32 val = 0;

	/* 输出低电平,至少维持480us */
	ds18b20_set_val_udelay(0, 485);

	/* 释放总线15~60us后读取数据（为了保证读取OK，释放选择大于60us） */
	ds18b20_bus_release_us(75);

	/* 读取数据 */
	val = DS18B20_GPIO_DATA_GET(); 

	/* 读取完成后等待从设备释放总线（60~240us），按最大时长算 */
	tool_udelay(250);

	/* 读取到0则初始化成功，存在从设备回应（从设备将总线拉低了） */
	return ((0 == val) ? OK : ERROR);
}

/* 发送bit */
static void ds18b20_write_bit(BOOL high)
{
	if (high)
	{
		/* 发送1:拉低总线1~15us，最少1us，最多15us */
		ds18b20_set_val_udelay(0, 2);
		
		/* 释放总线,一直维持高电平（从拉低到结束发送至少60us） */
		ds18b20_bus_release_us(58);
		
		/* 维持总线释放至少1us才继续进行下次操作 */
		tool_udelay(2);
	}
	else
	{
		/* 发送0:一直维持低电平60~120us,然后释放总线 */
		ds18b20_set_val_udelay(0, 60);

		/* 释放总线至少1us才继续进行下次操作 */
		ds18b20_bus_release_us(2);
	}
}

/* 接收bit */
static int ds18b20_read_bit(VOID)
{
	int val = 0;
	
	/* 拉低总线1~15us，最少1us，最多15us，唤醒从设备 */
	ds18b20_set_val_udelay(0, 2);
	
	/* 释放总线（采样需要在拉低总线的15us内完成） */
	ds18b20_bus_release_us(8);

	/* 采样 */
	val = DS18B20_GPIO_DATA_GET();

	/* 从电平拉低到salve传输完毕至少需要60us,采样后继续等待 */
	tool_udelay(50);
	
	/* 维持总线释放至少1us才继续进行下次操作 */
	tool_udelay(2);

	return val;
}

/* 发送一个Byte（优先传输低位） */
static void ds18b20_write_byte(uint8 byte)
{
	int bit = 0;
	for (bit = 0; bit < 8; bit++)
	{
		ds18b20_write_bit((byte >> bit) & 0x1);
	}
}

/* 读取一个Byte（优先传输低位） */
static uint8 ds18b20_read_byte(void)
{
	int bit = 0;
	uint8 byte = 0x0;
	
	for (bit = 0; bit < 8; bit++)
	{
		if (1 == ds18b20_read_bit()) 
		{
			byte |= (0x1 << bit);
		}
	}
	
	return byte;
}

/* 发送Rom命令，即选择某个从设备 */
static void ds18b20_send_rom_cmd(uint8 cmd)
{
	ds18b20_write_byte(cmd);
}

/* 发送功能命令：读、写、查找等 */
static void ds18b20_send_fun_cmd(uint8 cmd)
{
	ds18b20_write_byte(cmd);
}

/* DS18B20读取ROM操作 */
int ds18b20_read_rom(uint8 * buf, uint32 * len)
{
	uint32 cnt = 0;

	if ((NULL == buf) || (NULL == len))
	{
		print_screen("\r\n Invalid parameter!");
		return ERROR;
	}
	
	/* 1、初始化 */
	if (OK != ds18b20_internal_initization())
	{
		print_screen("\r\n read rom initization failed!");
		return ERROR;
	}

	/* 2、发送ROM命令 */
	ds18b20_send_rom_cmd(DS18B20_CMD_READ_ROM);
	
	/* 3、读取数据 */
	for (cnt = 0; (cnt < *len) && (cnt < DS18B20_ROM_MAX_LEN); cnt++)
	{
		buf[cnt] = ds18b20_read_byte();
	}

	/* 4、数据校验 */

	*len = cnt;
	return OK;
}

/* 等待转化成功 */
static int ds18b20_wait_convert_success(int timeout_us)
{
	while (timeout_us--)
	{
		/* 总线变为高电平即转换成功 */
		if (1 == ds18b20_read_bit())
			return OK;
		tool_udelay(1);
	}
	return ERROR;
}

/* 温度转换读取 */
static int ds18b20_convert_temperature(void)
{
	/* 1、初始化 */
	if (OK != ds18b20_internal_initization())
	{
		print_screen("\r\n read rom initization failed!");
		return ERROR;
	}

	/* 2、发送ROM命令，只有一个DS18B20从设备，因此可以跳过ROM选择 */
	ds18b20_send_rom_cmd(DS18B20_CMD_SKIP_ROM);

	/* 3、发送功能命令（温度转化计算） */
	ds18b20_send_rom_cmd(DS18B20_CMD_CONVERT_TEAMPERATURE);

	/* 4、等待/判断转换成功（精度最高12bit时，转换最多需要750ms） */
	if (OK != ds18b20_wait_convert_success(800000))
	{
		print_screen("\r\nds18b20 wait convert temperature failed!");
		return ERROR;
	}
	
	return OK;
}

/* 读取9Bytes的RAM   */
int ds18b20_read_ram(uint8 * ram, int * len)
{
	int cnt;
	
	/* 1、初始化 */
	if (OK != ds18b20_internal_initization())
	{
		print_screen("\r\nds18b20 wait convert temperature failed!");
		return ERROR;
	}
	
	/* 2、发送ROM命令，只有一个DS18B20从设备，因此可以跳过ROM选择 */
	ds18b20_send_rom_cmd(DS18B20_CMD_SKIP_ROM);

	/* 3、发送功能命令（读取RAM） */
	ds18b20_send_fun_cmd(DS18B20_CMD_READ_SCRATCHPAD);

	for (cnt = 0; cnt < DS18B20_RAM_MAX_LEN; cnt++)
	{
		ram[cnt] = ds18b20_read_byte();
	}
	
	*len = cnt;
	return OK;
}

/* 读取并转化温度 */
int ds18b20_read_temperature(double * temperature)
{
	double sum = 0;
	int index;
	int ramLen = 9;
	unsigned char ram[9];
	double tempRegBitWeight[] = {
		0.0625,	/* 权重:2^(-4) */ 
		0.125,	/* 权重:2^(-3) */
		0.25, 	/* 权重:2^(-2) */
		0.5, 	/* 权重:2^(-1) */
		1, 		/* 权重:2^0 */
		2,		/* 权重:2^1 */
		4,		/* 权重:2^2 */
		8, 		/* 权重:2^3 */
		16, 	/* 权重:2^4 */
		32,		/* 权重:2^5 */
		64		/* 权重:2^6 */
	};

	/* 1、温度转换（转换结果存在ram中） */
	if (OK != ds18b20_convert_temperature())
	{
		return ERROR;
	}

	/* 2、读取RAM */
	if (OK != ds18b20_read_ram(ram, &ramLen))
	{
		return ERROR;
	}

	/* 3、计算温度 */

	/* 3.1、先判断精度（精度bit输包括符号位） */
	if (DS18B20_IS_PRECSION_9_BIT(ram)) 			
		index = 3;	/* 精度: 9bit */
	else if (DS18B20_IS_PRECSION_10_BIT(ram))
		index = 2;	/* 精度: 10bit */
	else if (DS18B20_IS_PRECSION_11_BIT(ram))
		index = 1;	/* 精度: 11bit */
	else if (DS18B20_IS_PRECSION_12_BIT(ram))
		index = 0;	/* 精度是 12 bit */

	/* 3.2、根据精度计算温度值 */

	/* 处理LSB */
	while(index < 8)
	{
		if (ram[DS18B20_RAM_TEMPERATURE_LSB] & (0x1 << index))
		{
			sum += tempRegBitWeight[index];
		}
		index++;
	}

	/* 处理MSB，MSB只有低三位值有效 */
	for (index = 0; index < DS18B20_SIGN_BIT_START; index++)
	{
		if (ram[DS18B20_RAM_TEMPERATURE_MSB] & (0x1 << index))
			sum += tempRegBitWeight[8 + index];
	}

	/* 3.3、正负数判断，MSB的高5bit为1则为负数 */
	if (ram[DS18B20_RAM_TEMPERATURE_MSB] & (0x1 << DS18B20_SIGN_BIT_START))
	{
		sum = 0 - sum;
	}

	*temperature = sum;
	return 0;
}

void ds18b20_init(void)
{
	/* 初始化状态确保为高电平 */
	ds18b20_bus_release_us(1000000);
}

