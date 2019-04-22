# 1 "src/log/syslog.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/log/syslog.c"
# 1 "./include/tools/tools.h" 1
# 20 "./include/tools/tools.h"
typedef char * va_list;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
# 33 "./include/tools/tools.h"
typedef enum{
 GET_STR_WORD = 0,
 GET_STR_LINE = 1,
 GET_STR_MAX
}GET_STRING_FLAG;


extern char * uitoa(uint32 value, char * str);
extern uint32 atoui(const char * str);
extern char * uitoxa(uint32 value, char * str);
extern char * ftoa(float value, char * str);
extern uint8 * set_buffer(uint8 * const str, uint8 set, uint32 len);

extern int print_screen(const char * fmt, ...);
extern char * get_word(char * str, uint8 len);
extern char * get_line(char * str, uint8 len);
extern char * get_string(char * str, uint8 len, GET_STRING_FLAG flag);
extern int scan_screen(const char * fmt, ...);
# 2 "src/log/syslog.c" 2
# 1 "./include/log/log.h" 1





typedef enum soc_debug_level{
 SOC_DBG_NORMAL,
 SOC_DBG_WARN,
 SOC_DBG_ERROR,
 SOC_DBG_LOG,
 SOC_DBG_MAX
}SOC_DBG_LEV;
# 37 "./include/log/log.h"
extern int gSysDebugFlag;
# 3 "src/log/syslog.c" 2


int gSysDebugFlag = 0;
