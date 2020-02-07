#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef NULL
#define NULL ((void *)(0))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL
#define BOOL int
#endif

typedef char * va_list;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define __GET_BIT(sValue, bitPos)		(((sValue) >> (bitPos)) & (0x01))
#define __SET_BIT(sValue, bitPos, setBit)	((setBit) ? ((sValue) |= ((0x01) << (bitPos))) : ((sValue) &= ~((0x01) << (bitPos))))

#define _TEST_BIT_IS_ZERO(sValue, bitPos)	(__GET_BIT(sValue, bitPos) ? (0) : (1))
#define _TEST_BIT_IS_ONE(sValue, bitPos)	(__GET_BIT(sValue, bitPos) ? (1) : (0))

#define _INTSIZEOF(n)		((sizeof(n) + sizeof(int) - 1)  & ~(sizeof(int) - 1))
/* 要考虑内存对齐，所以不能简单的用sizeof(v) */
#define va_start(ap,v)		(ap = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap,t)		(*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap)			(ap = (va_list)(0))

#define CHANGE_TO_STR_BUFF_LEN	(12)

typedef enum{
	GET_STR_WORD = 0,
	GET_STR_LINE = 1,
	GET_STR_MAX
}GET_STRING_FLAG;

extern void tool_dealy(uint16 sec);
extern BOOL tool_isBigEndOrder(void);
extern char * tool_itoa(uint32 value, char * str);
extern uint32 tool_atoui(const char * str);
extern char * tool_uitoxa(uint32 value, char * str);
extern char * tool_ftoa(float value, char * str);
extern uint8 * set_buffer(uint8 * const str, uint8 set, uint32 len);

extern int print_screen(const char * fmt, ...);
extern char * get_word(char * str, uint8 len);
extern char * get_line(char * str, uint8 len);
extern char * get_string(char * str, uint8 len, GET_STRING_FLAG flag);
extern int scan_screen(const char * fmt, ...);

#endif
