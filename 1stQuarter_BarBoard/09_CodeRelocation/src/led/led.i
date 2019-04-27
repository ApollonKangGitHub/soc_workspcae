# 1 "src/led/led.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/led/led.c"
# 10 "src/led/led.c"
# 1 "./include/led/led.h" 1
# 27 "./include/led/led.h"
void led_delay(volatile int time);
extern void led_init();
extern void led_running();
# 11 "src/led/led.c" 2
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
# 12 "src/led/led.c" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_public.h" 1



# 1 "./include/soc_s3c2440/soc_s3c2440_reg.h" 1
# 5 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_reg_operator.h" 1
# 6 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_gpio_field.h" 1
# 7 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_uart_field.h" 1
# 8 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_other_field.h" 1
# 9 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 13 "src/led/led.c" 2

void led_delay(volatile int time)
{
 while(time--);
}

void led_init()
{

 (*((volatile unsigned int *)((0x56000050)))) &= ~((0x3 << 0) | (0x3 << 4) | (0x3 << 8) | (0x3 << 10) | (0x3 << 12));
 (*((volatile unsigned int *)((0x56000060)))) &= ~((0x3 << 6));


 (*((volatile unsigned int *)((0x56000050)))) |= (((0x1) << (8)) | ((0x1) << (10)) | ((0x1) << (12)));

}

void led_running(uint32 led_cycle_times)
{




 volatile uint32 gpfTemp=0;
 volatile uint32 gpgTemp=0;

 int delay_time = (250000);
 uint32 i = 0;
 uint32 len = 0;
 const unsigned char led_info[] = {
  0x00, 0x40, 0x60, 0x70, 0x30, 0x10, 0x00, 0x70,
  0x00, 0x70, 0x00, 0x70, 0x00, 0x10, 0x30, 0x70,
  0x60, 0x40, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70
 };
 len = sizeof(led_info) / sizeof(led_info[0]);


 while(led_cycle_times--)
 {
  for(i = 0; i < len; i++)
  {
   gpfTemp = (*((volatile unsigned int *)((0x56000054))));
   gpgTemp = (*((volatile unsigned int *)((0x56000064))));


   if(((((gpfTemp) >> (0)) & (0x01)) ? (0) : (1))){
    if(delay_time < (500000)){
     delay_time += (20000);
    }
   }
   if(((((gpfTemp) >> (2)) & (0x01)) ? (0) : (1))){
    if(delay_time > (50000)){
     delay_time -= (20000);
    }
   }
   if(((((gpgTemp) >> (3)) & (0x01)) ? (0) : (1))){
    delay_time = (250000);
   }






   (((((led_info[i]) >> (4)) & (0x01))) ? (((*((volatile unsigned int *)((0x56000054))))) |= ((0x01) << (4))) : (((*((volatile unsigned int *)((0x56000054))))) &= ~((0x01) << (4))));
   (((((led_info[i]) >> (5)) & (0x01))) ? (((*((volatile unsigned int *)((0x56000054))))) |= ((0x01) << (5))) : (((*((volatile unsigned int *)((0x56000054))))) &= ~((0x01) << (5))));
   (((((led_info[i]) >> (6)) & (0x01))) ? (((*((volatile unsigned int *)((0x56000054))))) |= ((0x01) << (6))) : (((*((volatile unsigned int *)((0x56000054))))) &= ~((0x01) << (6))));

   led_delay(delay_time);
  }
 }
}
