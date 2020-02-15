# 1 "soc_s3c2440_init.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "soc_s3c2440_init.c"
# 1 "./include/tools/tools.h" 1
# 26 "./include/tools/tools.h"
typedef char * va_list;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
# 50 "./include/tools/tools.h"
typedef enum{
 GET_STR_WORD = 0,
 GET_STR_LINE = 1,
 GET_STR_MAX
}GET_STRING_FLAG;





extern void tool_dealy(uint16 sec);
extern int tool_isBigEndOrder(void);
extern int tool_strlen(char * str);
extern char * tool_itoa(uint32 value, char * str);
extern int tool_atoui(const char * str, uint32 *result);
extern int tool_atoux(const char * str, uint32 * result);
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
# 2 "soc_s3c2440_init.c" 2
# 1 "./include/nand_flash/nand_flash.h" 1
# 59 "./include/nand_flash/nand_flash.h"
extern void nand_flash_info_display(void);
extern void nand_flash_init(void);
extern void nand_flash_read(uint32 addr, uint8 * buf, uint32 len);
# 3 "soc_s3c2440_init.c" 2
# 1 "./include/log/log.h" 1





typedef enum soc_debug_level{
 SOC_DBG_NORMAL,
 SOC_DBG_WARN,
 SOC_DBG_ERROR,
 SOC_DBG_LOG,
 SOC_DBG_MAX
}SOC_DBG_LEV;
# 37 "./include/log/log.h"
extern int volatile gSysDebugFlag;
# 4 "soc_s3c2440_init.c" 2


int isBootFromNorFlash(void)
{
 volatile uint32 * pTest = (volatile uint32 * )(0x0);
 uint32 valBak = *pTest;
 uint32 valTest = 0x12345678;
 *pTest = valTest;

 if (valTest == *pTest)
 {

  *pTest = valBak;
  return 0;
 }

 return 1;
}
# 35 "soc_s3c2440_init.c"
void code_relocation(void)
{
 extern uint32 _code_start_;
 extern uint32 _bss_start_;

 volatile uint32 * dst = (volatile uint32 *)(&_code_start_);
 volatile uint32 * src = (volatile uint32 *)(0x0);
 volatile uint32 * end = (volatile uint32 *)(&_bss_start_);
 int len = ((int)(&_bss_start_) - (int)(&_code_start_));


 if (isBootFromNorFlash())
 {

  while(dst < end)
  {
   *dst++ = *src++;
  }
 }
 else
 {

  nand_flash_init();
  nand_flash_read((uint32)src, (uint8 *)dst, len);
 }

}

void bss_clear(void)
{
 extern uint32 _bss_start_;
 extern uint32 _bss_end_;

 volatile uint32 * bss_start = (volatile uint32 *)(&_bss_start_);
 volatile uint32 * bss_end = (volatile uint32 *)(&_bss_end_);


 while (bss_start < bss_end)
 {
  *bss_start++ = 0;
 }
}

void relocate_init(void)
{
 code_relocation();
 bss_clear();
}
