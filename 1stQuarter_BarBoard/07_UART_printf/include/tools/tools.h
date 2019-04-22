#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef NULL
#define NULL ((void *)(0))
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

typedef char * va_list;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define _INTSIZEOF(n)		((sizeof(n) + sizeof(int) - 1)  & ~(sizeof(int) - 1))
/* 要考虑内存对齐，所以不能简单的用sizeof(v) */
#define va_start(ap,v)		(ap = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap,t)		(*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap)			(ap = (va_list)(0))

#define CHANGE_TO_STR_BUFF_LEN	(12)

extern char * uitoa(uint32 value, char * str);
extern char * uitoxa(uint32 value, char * str);
extern char * ftoa(float value, char * str);
extern uint8 * set_buffer(uint8 * const str, uint8 set, uint32 len);

extern int print(const char * fmt, ...);

#endif
