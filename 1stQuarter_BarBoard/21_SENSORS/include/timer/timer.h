#ifndef _TIMER_H_
#define _TIMER_H_

#include <tools.h>

#define TIMER0_FREQUENCY	(100)

typedef uint32 time_t;
typedef uint32 time64_t;
struct tm {
	int tm_sec; 	/* 秒 – 取值区间为[0,59] */
	int tm_min; 	/* 分 - 取值区间为[0,59] */
	int tm_hour;	/* 时 - 取值区间为[0,23] */
	int tm_mday;	/* 一个月中的日期 - 取值区间为[1,31] */
	int tm_mon; 	/* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
	int tm_year;	/* 年份，其值等于实际年份减去1970 */
};

/* (时间戳从1970.1.1-0:0:0开始算) */
typedef struct
{
	struct tm __tm__;		/* 标准时间 */
	time64_t __usecStamp__;	/* 时间戳微秒 */
	time64_t __msecStamp__;	/* 时间戳毫秒 */
	time_t __secStamp__;	/* 时间戳秒 */
}sys_time_t;

/* 定时器回掉函数 */
typedef void * (*timer_function)(void * pArgv);
extern void timer_0_init(void);
extern void timer_0_debug_print(void);
extern int timer_register(char * name, timer_function fun);
extern int timer_unregister(char * name);
extern void timer_udelay(int us);
extern void timer_mdelay(int ms);
extern void timer_get_sys_time(sys_time_t * sysTime);
extern void timer_localtime(time_t time, struct tm * tmStruct);

#endif	/* _TIMER_H_ */
