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

#define OK		(0)
#define ERROR	(-1)

#define TOOL_STR_CMP_EQUAL			(0)
#define TOOL_STR_CMP_GREATER(pos) 	(pos)
#define TOOL_STR_CMP_LESS(pos)		(-(pos))

#define TOOL_MAX_INT_STR_LEN		(11)

typedef char * va_list;
typedef unsigned long uint32;
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

#define _TOOL_GET_STRING_LEN_		(64)
#define _TOOL_PRINT_HEX_STR_LEN_	(20)

#define _TOOL_KB_BASE_		(1 << 10)
#define _TOOL_MB_BASE_		(1 << 20)
#define _TOOL_GB_BASE_		(1 << 30)

#define TOOL_TEST_PRINT()	print_screen("\r\n--------%s - %s - %d", __FILE__, __FUNCTION__, __LINE__)

typedef enum{
	GET_STR_WORD = 0,
	GET_STR_LINE = 1,
	GET_STR_MAX
}GET_STRING_FLAG;

#define tool_getChar uart_getchar
#define tool_putChar uart_putchar
#define tool_puts uart_puts

extern void tool_dealy(uint16 sec);
extern BOOL tool_isBigEndOrder(void);
extern int tool_strlen(char * str);
extern char * tool_itoa(uint32 value, char * str);
extern BOOL tool_atoui(const char * str, uint32 *result);
extern BOOL tool_atoux(const char * str, uint32 * result);
extern char * tool_uitoxa(uint32 value, char * str);
extern char * tool_ftoa(float value, char * str);
extern uint8 * set_buffer(uint8 * const str, uint8 set, uint32 len);
extern void print_byteHex(uint8 value);
extern int print_screen(const char * fmt, ...);
extern char * get_word(char * str, uint32 len);
extern char * get_line(char * str, uint32 len);
extern char * get_string(char * str, uint32 len, GET_STRING_FLAG flag);
extern int scan_screen(const char * fmt, ...);
extern void tool_calc_mem_size
(
	uint32 size, 
	uint32 *gbbyte, 
	uint32 *mbbyte, 
	uint32 *kbyte, 
	uint32 *byte
);
extern void print_hexStr_multiple(uint8 * buf, int len, uint32 startAddr);
extern int tool_strncmp(const char * s1, const char * s2, int len);

#endif
