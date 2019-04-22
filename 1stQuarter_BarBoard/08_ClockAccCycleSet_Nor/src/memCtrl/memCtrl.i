# 1 "src/memCtrl/memCtrl.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/memCtrl/memCtrl.c"







# 1 "./include/soc_s3c2440/soc_s3c2440_memCtrl_field.h" 1
# 9 "src/memCtrl/memCtrl.c" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_reg.h" 1
# 10 "src/memCtrl/memCtrl.c" 2
# 1 "./include/memCtrl/memCtrl.h" 1




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
# 6 "./include/memCtrl/memCtrl.h" 2
# 30 "./include/memCtrl/memCtrl.h"
int nor_flash_start_squence_set(uint32 choose);
# 11 "src/memCtrl/memCtrl.c" 2

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
# 13 "src/memCtrl/memCtrl.c" 2


int nor_flash_start_squence_set(uint32 choose)
{
 { if(SOC_DBG_ERROR == SOC_DBG_NORMAL) { print_screen("Error""[%s-%d]:""choose is %d\r\n", __FUNCTION__, 17, choose); } else if(SOC_DBG_WARN == SOC_DBG_NORMAL) { print_screen("Warnning""[%s-%d]:""choose is %d\r\n", __FUNCTION__, 17, choose); } else if(SOC_DBG_NORMAL == SOC_DBG_NORMAL) { print_screen("Debug""[%s-%d]:""choose is %d\r\n", __FUNCTION__, 17, choose); } else if(SOC_DBG_LOG == SOC_DBG_NORMAL){ { print_screen("Logging""[%s-%d]:""choose is %d\r\n", __FUNCTION__, 17, choose); } } };
 switch(choose)
 {
  case (0x0):
  case (0x1):
  case (0x2):
  case (0x3):
  case (0x4):
  case (0x5):
  case (0x6):
  case (0x7):

   { (*((volatile unsigned int *)((0x48000004)))) = (choose << (8)) & (0x7<<8);};
   break;
  default:
   return -1;
 }
 return 0;
}
