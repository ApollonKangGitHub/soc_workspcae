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

static timer_register_t gTimerCallbackArr[TIMER_CALLBACK_ARR_MAX] = {{{'\0'}, NULL}};

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
		if (NULL == gTimerCallbackArr[index].callback)
		{
			gTimerCallbackArr[index].callback = fun;
			(void)tool_strnpy(gTimerCallbackArr[index].name, name, TIMER_CALLBACK_NAME_MAX);
			(void)print_screen("\r\n register %s timer callback success!!", gTimerCallbackArr[index].name);
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
		if (0 == tool_strncmp(name, gTimerCallbackArr[index].name, TIMER_CALLBACK_NAME_MAX))
		{
			gTimerCallbackArr[index].callback = NULL;
			(void)set_buffer(gTimerCallbackArr[index].name, 0, TIMER_CALLBACK_NAME_MAX);
			(void)print_screen("\r\n unregister %s timer callback success!!", name);
			return ;
		}
	}

	return ERROR;
}

/* 所有注册函数处理 */
void * timer_irq_traversal_handle(void * pArgv)
{
	int index = 0;
	for (index = 0; index < TIMER_CALLBACK_NAME_MAX; index++)
	{
		if (gTimerCallbackArr[index].callback)
		{
			(void)gTimerCallbackArr[index].callback(pArgv);
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
	interrupt_register(interrupt_type_INT_TIMER0, timer_irq_traversal_handle);

	/* 设置TIMER0的时钟 
	 * Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value} 
	 * {prescaler value} = 0~255 
	 * {divider value} = 2, 4, 8, 16
	 * 假设:50000000 / (99+1) / 16 = 31250
	 * 50000000 / (49 + 1) / 16 = 62500
	 */
	 
	/* prescaler 0 = 49 */
	SOC_S3C2440_REG_BITS_SET(TCFG0r, \
		TCFG0_PRESCALER0_START_BIT, TCFG0_PRESCALER0_BITSf, 49);
	
	/* MUX0 = 1/16 */
	SOC_S3C2440_REG_BITS_SET(TCFG1r, \
		TCFG1_MUX0_START_BIT, TCFG1_MUX0_BITSf, TCFG1_MUX_PWM_1_16);	
	
	/* 设置TIMER0的初值,10ms中断一次 */
	TCNTB0r = 625;
	TCMPB0r = 0;
	
	/* 加载初值, 启动TIMER0 */
	SOC_S3C2440_REG_BITS_SET(TCONr, \
		 TCON_TIMER0_START_BIT, TCON_TIMER0_BITSf, TCON_TIMER_MANUAL_UPDATE);

	/* 设置为自动加载并启动（清除TCON_TIMER_MANUAL_UPDATE） */
	SOC_S3C2440_REG_BITS_SET(TCONr, TCON_TIMER0_START_BIT, \
		 TCON_TIMER0_BITSf, TCON_TIMER_AUTO_RELOAD_ON | TCON_TIMER_START_ON);
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

