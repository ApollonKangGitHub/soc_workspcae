#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER0_FREQUENCY	(100)

/* 定时器回掉函数 */
typedef void * (*timer_function)(void * pArgv);
extern void timer_0_init(void);
extern void timer_0_debug_print(void);
extern int timer_register(char * name, timer_function fun);
extern int timer_unregister(char * name);

#endif	/* _TIMER_H_ */