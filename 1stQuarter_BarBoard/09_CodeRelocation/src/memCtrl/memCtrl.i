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
# 39 "./include/tools/tools.h"
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
# 49 "./include/memCtrl/memCtrl.h"
typedef enum{
 SOC_MEMCTRL_BANK6_SDRAM_0,
 SOC_MEMCTRL_BANK7_SDRAN_1,
 SOC_MEMCTRL_BANK_SDRAM_ALL
}SDRAM_CTRL_t;

extern int nor_flash_start_squence_set(uint32 choose);
extern int nor_flash_init(void);

extern int sdram_init(SDRAM_CTRL_t choose);
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

int nor_flash_init(void)
{
 { if(gSysDebugFlag){ { if(SOC_DBG_ERROR == SOC_DBG_NORMAL) { print_screen("Error""%%[%s-%d]@ ""start init nor flash!\n\r", __FUNCTION__, 16); } else if(SOC_DBG_WARN == SOC_DBG_NORMAL) { print_screen("Warnning""%%[%s-%d]@ ""start init nor flash!\n\r", __FUNCTION__, 16); } else if(SOC_DBG_NORMAL == SOC_DBG_NORMAL) { print_screen("Debug""%%[%s-%d]@ ""start init nor flash!\n\r", __FUNCTION__, 16); } else if(SOC_DBG_LOG == SOC_DBG_NORMAL){ { print_screen("Logging""%%[%s-%d]@ ""start init nor flash!\n\r", __FUNCTION__, 16); } } }; } };



 if(0x01 == ((*((volatile unsigned int *)((0x48000000)))) >> 1) & 0x03){
  print_screen("Nor flash data width is 16bits!\n\r");
 }
 else if(0x02 == ((*((volatile unsigned int *)((0x48000000)))) >> 1) & 0x03){
  print_screen("Nor flash data width is 32bits!\n\r");
 }
 else{
  print_screen("Nor flash data width is unavail!\n\r");
 }

 nor_flash_start_squence_set((0x5));

 { if(gSysDebugFlag){ { if(SOC_DBG_ERROR == SOC_DBG_NORMAL) { print_screen("Error""%%[%s-%d]@ ""start init nor succeed!\n\r", __FUNCTION__, 32); } else if(SOC_DBG_WARN == SOC_DBG_NORMAL) { print_screen("Warnning""%%[%s-%d]@ ""start init nor succeed!\n\r", __FUNCTION__, 32); } else if(SOC_DBG_NORMAL == SOC_DBG_NORMAL) { print_screen("Debug""%%[%s-%d]@ ""start init nor succeed!\n\r", __FUNCTION__, 32); } else if(SOC_DBG_LOG == SOC_DBG_NORMAL){ { print_screen("Logging""%%[%s-%d]@ ""start init nor succeed!\n\r", __FUNCTION__, 32); } } }; } };
}


int nor_flash_start_squence_set(uint32 choose)
{
 { if(SOC_DBG_ERROR == SOC_DBG_NORMAL) { print_screen("Error""%%[%s-%d]@ ""choose is %d\n\r", __FUNCTION__, 38, choose); } else if(SOC_DBG_WARN == SOC_DBG_NORMAL) { print_screen("Warnning""%%[%s-%d]@ ""choose is %d\n\r", __FUNCTION__, 38, choose); } else if(SOC_DBG_NORMAL == SOC_DBG_NORMAL) { print_screen("Debug""%%[%s-%d]@ ""choose is %d\n\r", __FUNCTION__, 38, choose); } else if(SOC_DBG_LOG == SOC_DBG_NORMAL){ { print_screen("Logging""%%[%s-%d]@ ""choose is %d\n\r", __FUNCTION__, 38, choose); } } };
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
   { (*((volatile unsigned int *)((0x48000004)))) = ((0x7) << (8)) & (0x7<<8);};
   break;
 }
 return 0;
}


int sdram_init(SDRAM_CTRL_t choose)
{
 { if(gSysDebugFlag){ { if(SOC_DBG_ERROR == SOC_DBG_NORMAL) { print_screen("Error""%%[%s-%d]@ ""start init sdram!\n\r", __FUNCTION__, 65); } else if(SOC_DBG_WARN == SOC_DBG_NORMAL) { print_screen("Warnning""%%[%s-%d]@ ""start init sdram!\n\r", __FUNCTION__, 65); } else if(SOC_DBG_NORMAL == SOC_DBG_NORMAL) { print_screen("Debug""%%[%s-%d]@ ""start init sdram!\n\r", __FUNCTION__, 65); } else if(SOC_DBG_LOG == SOC_DBG_NORMAL){ { print_screen("Logging""%%[%s-%d]@ ""start init sdram!\n\r", __FUNCTION__, 65); } } }; } };




 (*((volatile unsigned int *)((0x48000024)))) = 0x8404f5;







 (*((volatile unsigned int *)((0x48000028)))) = 0xb1;

 switch(choose){
  case SOC_MEMCTRL_BANK6_SDRAM_0:
   sdram0_init();
   break;
  case SOC_MEMCTRL_BANK7_SDRAN_1:
   sdram1_init();
   break;
  case SOC_MEMCTRL_BANK_SDRAM_ALL:
  deafult:
   sdram0_init();
   sdram1_init();
   break;
 }

 { if(gSysDebugFlag){ { if(SOC_DBG_ERROR == SOC_DBG_NORMAL) { print_screen("Error""%%[%s-%d]@ ""init sdram succeed!\n\r", __FUNCTION__, 94); } else if(SOC_DBG_WARN == SOC_DBG_NORMAL) { print_screen("Warnning""%%[%s-%d]@ ""init sdram succeed!\n\r", __FUNCTION__, 94); } else if(SOC_DBG_NORMAL == SOC_DBG_NORMAL) { print_screen("Debug""%%[%s-%d]@ ""init sdram succeed!\n\r", __FUNCTION__, 94); } else if(SOC_DBG_LOG == SOC_DBG_NORMAL){ { print_screen("Logging""%%[%s-%d]@ ""init sdram succeed!\n\r", __FUNCTION__, 94); } } }; } };
}

int sdram0_init()
{




 (*((volatile unsigned int *)((0x48000000)))) |= 0x02000000;






 (*((volatile unsigned int *)((0x4800001c)))) = 0x18001;





 (*((volatile unsigned int *)((0x4800002c)))) = 0x20;
}

int sdram1_init()
{




 (*((volatile unsigned int *)((0x48000000)))) |= 0x20000000;






 (*((volatile unsigned int *)((0x48000020)))) = 0x18001;





 (*((volatile unsigned int *)((0x48000030)))) = 0x20;
}
