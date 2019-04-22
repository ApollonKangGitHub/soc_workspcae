# 1 "soc_s3c2440.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "soc_s3c2440.c"
# 1 "./include/led/led.h" 1
# 27 "./include/led/led.h"
void led_delay(volatile int time);
extern void led_init();
extern void led_running();
# 2 "soc_s3c2440.c" 2
# 1 "./include/uart/uart.h" 1



extern void uart_init();
extern int uart_getchar(void);
extern void uart_putchar(int ch);
extern int uart_puts(const char * s);
# 3 "soc_s3c2440.c" 2
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
# 4 "soc_s3c2440.c" 2
# 1 "./include/memCtrl/memCtrl.h" 1



# 1 "./include/soc_s3c2440/soc_s3c2440_memCtrl_field.h" 1
# 5 "./include/memCtrl/memCtrl.h" 2
# 30 "./include/memCtrl/memCtrl.h"
int nor_flash_start_squence_set(uint32 choose);
# 5 "soc_s3c2440.c" 2
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
# 6 "soc_s3c2440.c" 2
# 1 "./include/soc_s3c2440/soc_s3c2440.h" 1
# 7 "soc_s3c2440.c" 2

typedef enum{
 TEST_LED,
 TEST_UART,
 TEST_FUNCTION,

 TEST_MAX
}soc_test_t;

void led_test()
{

 led_init();
 led_running();
}

void uart_test()
{

 char ch;
 char strLen[12] = {0};
 char * pStr = ((void *)(0));
 int len = 0;

 uart_init();
 len = uart_puts("Hello S3C2440!\r\n");
 pStr = uitoa(len, (char *)(strLen));
 uart_puts("string len = ");
 uart_puts(pStr);
 uart_puts("\r\n");

 print_screen("\r\ntest1:%d, test2:%d\r\n", 2147483647, 54552);
 print_screen("\r\nTest at [%s - %X] %c Please Enter:\r\n", __FUNCTION__, 39, 43);
 print_screen("\r\nTest at [%s - %X] %X Please Enter:\r\n", __FUNCTION__, 40, 65536);

 while(1){

  ch = uart_getchar();
  if('\r' == ch){
   uart_putchar('\n');
  }
  else if('\n' == ch){
   uart_putchar('\r');
  }
  uart_putchar(ch);
 }
}

void test_function()
{

 char str[(12)] = {0};
 uint32 led_cycle_times = 10;
 uint8 choose = -1;

 uart_init();
 led_init();

 while(1){

  print_screen("\r\n##################### Choose Menu #####################\n\r");
      print_screen("# 0:set bank0 config tacc(nor flash clock) is 1  HCLK #\r\n");
   print_screen("# 1:set bank0 config tacc(nor flash clock) is 2  HCLK #\r\n");
   print_screen("# 2:set bank0 config tacc(nor flash clock) is 3  HCLK #\r\n");
   print_screen("# 3:set bank0 config tacc(nor flash clock) is 4  HCLK #\r\n");
   print_screen("# 4:set bank0 config tacc(nor flash clock) is 6  HCLK #\r\n");
   print_screen("# 5:set bank0 config tacc(nor flash clock) is 8  HCLK #\r\n");
   print_screen("# 6:set bank0 config tacc(nor flash clock) is 10 HCLK #\r\n");
   print_screen("# 7:set bank0 config tacc(nor flash clock) is 14 HCLK #\r\n");
   print_screen("######################## End ##########################\n\r");
  print_screen("Please enter your choose:");

  choose = atoui(get_word(str, sizeof(str)));
  if(choose >= 0 && choose <= 7){
   nor_flash_start_squence_set(choose);


   set_buffer(str, 0, (12));
   print_screen("Please set led cycle times:");

   led_cycle_times = atoui(get_word(str, sizeof(str)));
   { if(SOC_DBG_ERROR == SOC_DBG_LOG) { print_screen("Error""[%s-%d]:""times:%d\r\n", __FUNCTION__, 88, led_cycle_times); } else if(SOC_DBG_WARN == SOC_DBG_LOG) { print_screen("Warnning""[%s-%d]:""times:%d\r\n", __FUNCTION__, 88, led_cycle_times); } else if(SOC_DBG_NORMAL == SOC_DBG_LOG) { print_screen("Debug""[%s-%d]:""times:%d\r\n", __FUNCTION__, 88, led_cycle_times); } else if(SOC_DBG_LOG == SOC_DBG_LOG){ { print_screen("Logging""[%s-%d]:""times:%d\r\n", __FUNCTION__, 88, led_cycle_times); } } };

   print_screen("\n\rPlease observe the status of the LED lamp.\r\n");
   led_running(led_cycle_times);
  }
  else{
   print_screen("\n\rError!Please choose again(between 0~7):\r\n");
   continue;
  }
 }
}

int soc_s3c2440_main(void)
{
 uint8 test_choose = TEST_FUNCTION;
 switch(test_choose){
  case TEST_LED:
   led_test();
   break;
  case TEST_UART:
   uart_test();
   break;
  case TEST_FUNCTION:
   test_function();
   break;
  default:
   print_screen("No have to test operation!\n\r");
   break;
 }
 return 0;
}
