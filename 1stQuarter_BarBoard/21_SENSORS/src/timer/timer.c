#include <log.h>
#include <tools.h>
#include <timer.h>
#include <soc_s3c2440_public.h>
#include <interrupt.h>

/* 定时器回调函数最大个数 */
#define TIMER_CALLBACK_ARR_MAX	(64)
#define TIMER_CALLBACK_NAME_MAX	(32)

typedef struct {
	char name[TIMER_CALLBACK_NAME_MAX];
	timer_function callback;
} timer_register_t;

const uint8 gMonDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/* 系统时间 */
static sys_time_t gSysTimer = {	
	.__tm__ = {
		.tm_sec = 0,
		.tm_min = 0,
		.tm_hour = 0,
		.tm_mday = 0,
		.tm_mon = 0,
		.tm_year = 0,
	},
	.__usecStamp__ = 0,	
	.__msecStamp__ = 0,	
	.__secStamp__ = 0,
};
static time_t gLastTimeStamp = 0;
static timer_register_t gTimer_CallbackArr[TIMER_CALLBACK_ARR_MAX] = {{{'\0'}, NULL}};

/* 注册定时器回调函数 */
int timer_register(char * name, timer_function fun)
{
	int index = 0;

	if ((NULL == name) || (tool_strlen(name) > TIMER_CALLBACK_NAME_MAX))
	{
		print_screen("\r\n naem:%p, len:%d, invalid parameter", name, tool_strlen(name));
		return ERROR;
	}
	
	while (index < TIMER_CALLBACK_ARR_MAX)
	{
		if (NULL == gTimer_CallbackArr[index].callback)
		{
			gTimer_CallbackArr[index].callback = fun;
			(void)tool_strnpy(gTimer_CallbackArr[index].name, name, TIMER_CALLBACK_NAME_MAX);
			(void)print_screen("\r\n register %s timer callback success!!", gTimer_CallbackArr[index].name);
			return OK;
		}
	}

	return ERROR;
}

/* 注销定时器回掉函数 */
int timer_unregister(char * name)
{
	int index = 0;
	
	if ((NULL == name) || (tool_strlen(name) > TIMER_CALLBACK_NAME_MAX))
	{	
		print_screen("\r\n naem:%p, len:%d, invalid parameter", name, tool_strlen(name));
		return ERROR;
	}
	
	while (index < TIMER_CALLBACK_ARR_MAX)
	{
		if (0 == tool_strncmp(name, gTimer_CallbackArr[index].name, TIMER_CALLBACK_NAME_MAX))
		{
			gTimer_CallbackArr[index].callback = NULL;
			(void)set_buffer(gTimer_CallbackArr[index].name, 0, TIMER_CALLBACK_NAME_MAX);
			(void)print_screen("\r\n unregister %s timer callback success!!", name);
			return ;
		}
	}

	return ERROR;
}

void timer_0_debug_print(void)
{
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "------------------------------");
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCFG0r :%x", TCFG0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCFG1r :%x", TCFG1r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCONr  :%x", TCONr);
	
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCNTB0r:%x", TCNTB0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCMPB0r:%x", TCMPB0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, " -- TCNTO0r:%x", TCNTO0r);
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "------------------------------");
}

/* 延时微秒 */
void timer_udelay(int us)
{
	int count = us * 5;	/* us对应timer2计数值 */
	volatile int pre = TCNTO0r;
	volatile int cur = 0;
	int delta = 0;		/* 两次计数值差值，可以考虑给延时函数单独分配timer，保证不受中断处理函数影响 */

	while (count > 0) {
		
		cur = TCNTO0r;
		if (cur <= pre)
		{
			delta = pre - cur;
		}
		else 
		{
			delta = pre + (50000 - cur);
		}
		
		count -= delta;
		pre = cur;
	}
}

/* 延时毫秒 */
void timer_mdelay(int ms)
{
	int us = 1000 * ms;
	timer_udelay(us);
}

/* 根据秒数时间戳计算年月日时分秒 */
void timer_localtime(time_t time, struct tm * tmStruct)
{
	uint32 Pass4year = 0;
	int hours_per_year;
	time_t timeStamp = time;

	if (timeStamp < 0)
	{
		timeStamp = 0;
	}

	/* 取秒时间 */
	tmStruct->tm_sec = (int)(timeStamp % 60);
	timeStamp /= 60;

	/* 取分钟时间 */
	tmStruct->tm_min = (int)(timeStamp % 60);
	timeStamp /= 60;

	/* 取过去多少个四年，每四年有 1461 * 24 小时 */
	Pass4year = ((unsigned int)timeStamp / (1461L * 24L));

	/* 计算年份 */
	tmStruct->tm_year = (Pass4year << 2) + 1970;

	/* 四年中剩下的小时数 */
	timeStamp %= 1461L * 24L;

	/* 校正闰年影响的年份，计算一年中剩下的小时数 */
	while(TRUE)
	{
		/* 一年的小时数 */
		hours_per_year = 365 * 24;

		/* 判断闰年 */
		if ((tmStruct->tm_year & 3) == 0)
		{
			/* 是闰年，一年则多24小时，即一天 */
			hours_per_year += 24;
		}

		if (timeStamp < hours_per_year)
		{
			break;
		}
		tmStruct->tm_year++;
		timeStamp -= hours_per_year;
	}

	/* 小时数 */
	tmStruct->tm_hour = (int)(timeStamp % 24);

	/* 一年中剩下的天数 */
	timeStamp /= 24;

	/* 假定为闰年 */
	timeStamp++;

	/* 校正闰年的误差，计算月份，日期 */
	if((tmStruct->tm_year & 3) == 0)
	{
		if (timeStamp > 60)
		{
			timeStamp--;
		}
		else
		{
			if (timeStamp == 60)
			{
				tmStruct->tm_mon = 1;
				tmStruct->tm_mday = 29;
				return ;
			}
		}
	}

	/* 计算月日 */
	for (tmStruct->tm_mon = 1; \
		gMonDays[tmStruct->tm_mon - 1] < timeStamp; \
		tmStruct->tm_mon++)
	{
		timeStamp -= gMonDays[tmStruct->tm_mon];
	}

	tmStruct->tm_mday = (int)(timeStamp);

	return;
}


/* 记录系统时间 */
static void timer_record_sys_time(void)
{
	time64_t oldSec = gSysTimer.__secStamp__;

	/* 中断一次10ms */
	gSysTimer.__usecStamp__ += (10 * 1000);		
	gSysTimer.__msecStamp__ += 10;
	gSysTimer.__secStamp__ = (time_t)(gSysTimer.__msecStamp__ / 1000);

	/* 每过一秒计算update一次 */
	if (gSysTimer.__secStamp__ > gLastTimeStamp)
	{
		gLastTimeStamp = gSysTimer.__secStamp__;
		timer_localtime(gSysTimer.__secStamp__, &gSysTimer.__tm__);
	}
}

/* 获取系统时间 */
void timer_get_sys_time(sys_time_t * sysTime)
{
	time64_t us = 0;
	
	/* 获取时间时离上次中断的us */
	us = (50000 - TCNTO0r) / 5;	
	memcpy(sysTime, &gSysTimer, sizeof(sys_time_t));

	/* 在上次统计的us上加上获取时间的点的新增us */
	sysTime->__usecStamp__ += us;
}

/* 所有注册函数处理 */
void * timer_irq_traversal_handle_timer0(void * pArgv)
{
	int index = 0;
	
	/* 更新系统时间 */
	timer_record_sys_time();

	/* 处理回调注册 */
	for (index = 0; index < TIMER_CALLBACK_NAME_MAX; index++)
	{
		if (gTimer_CallbackArr[index].callback)
		{
			(void)gTimer_CallbackArr[index].callback(pArgv);
		}
	}
}

/* 
 * 注意：在设置自动加载时，TCONr寄存器对应的TIMERn，其
 * TCON_TIMER_MANUAL_UPDATE 位 必须先置为1，表示手动加载初始值TCNTB0r到定时器内部，
 * 然后随后将其置为0，表示之后为自动加载，否则直接设置为0或者直接设置为1都有问题，
 * 因为timer内部的计数器值初始化一直为0，没有手动加载，也不会触发中断并自动加载新值
 */
void timer_0_init(void)
{
	/* 设置定时器中断入口函数 */
	interrupt_register(interrupt_type_INT_TIMER0, timer_irq_traversal_handle_timer0);

	/* 设置TIMER0的时钟 
	 * Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value} 
	 * {prescaler value} = 0~255 
	 * {divider value} = 2, 4, 8, 16
	 * 假设:50000000 / (99+1) / 16 = 31250
	 * 50000000 / (49 + 1) / 16 = 62500
	 * 50000000 / (4+1) / 2 = 5000000
	 */
	 
	/* prescaler 0 = 4 */
	SOC_S3C2440_REG_BITS_SET(TCFG0r, TCFG0_PRESCALER0_START_BIT, TCFG0_PRESCALER0_MASK, 4);
	
	/* MUX0 = 1/2 */
	SOC_S3C2440_REG_BITS_SET(TCFG1r, TCFG1_MUX0_START_BIT, TCFG1_MUX0_MASK, TCFG1_MUX_PWM_1_2);	
	
	/* 设置TIMER0的初值,10ms中断一次,但是每0.2us cnt自减一次 */
	TCNTB0r = 50000;
	TCMPB0r = 0;
	
	/* 加载初值, 启动TIMER0 */
	SOC_S3C2440_REG_BITS_SET(TCONr, TCON_TIMER0_START_BIT, TCON_TIMER0_MASK, TCON_TIMER_MANUAL_UPDATE);

	/* 设置为自动加载并启动（清除TCON_TIMER_MANUAL_UPDATE） */
	SOC_S3C2440_REG_BITS_SET(TCONr, TCON_TIMER0_START_BIT,  TCON_TIMER0_MASK, TCON_TIMER_AUTO_RELOAD_ON | TCON_TIMER_START_ON);
}

