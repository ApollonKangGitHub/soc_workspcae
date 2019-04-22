# 1 "src/uart/uart.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/uart/uart.c"
# 9 "src/uart/uart.c"
# 1 "./include/uart/uart.h" 1



extern void uart_init();
extern int uart_getchar(void);
extern void uart_putchar(int ch);
extern int uart_puts(const char * s);
# 10 "src/uart/uart.c" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_public.h" 1



# 1 "./include/soc_s3c2440/soc_s3c2440_reg.h" 1
# 5 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_reg_operator.h" 1



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
# 5 "./include/soc_s3c2440/soc_s3c2440_reg_operator.h" 2
# 6 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_gpio_field.h" 1
# 7 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_uart_field.h" 1
# 8 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_other_field.h" 1
# 9 "./include/soc_s3c2440/soc_s3c2440_public.h" 2
# 11 "src/uart/uart.c" 2
void uart_init()
{

 (*((volatile unsigned int *)((0x56000070)))) &= ~((0x3 << 4) | (0x3 << 6));
 (*((volatile unsigned int *)((0x56000070)))) |= (((0x2) << (4)) | ((0x2) << (6)));



 (*((volatile unsigned int *)((0x56000078)))) &= ~((0x1 << 2) | (0x1 << 3));


 (*((volatile unsigned int *)((0x50000004)))) &= ~((0x3 << 10) | (0x3 << 2) | (0x3 << 0));
 (*((volatile unsigned int *)((0x50000004)))) |= (((0x1) << (2)) | ((0x1) << (0)));
# 33 "src/uart/uart.c"
 (*((volatile unsigned int *)((0x50000028)))) &= 0x0;
 (*((volatile unsigned int *)((0x50000028)))) |= 26;






 (*((volatile unsigned int *)((0x50000000)))) &= ~((0x3 << 6) | (0x3 << 2) | (0x3 << 0) | (0x3 << 3));
 (*((volatile unsigned int *)((0x50000000)))) |= (((0x0) << (2)) | ((0x3) << (0)));


}


void uart_putchar(int ch)
{

 while(!(((((*((volatile unsigned int *)((0x50000010))))) >> ((0x2))) & (0x01)) ? (1) : (0)));


 (*((volatile unsigned char *)((0x50000020)))) = (unsigned char)(ch);
}


int uart_getchar(void)
{

 while(!(((((*((volatile unsigned int *)((0x50000010))))) >> ((0x0))) & (0x01)) ? (1) : (0)));

 return (*((volatile unsigned char *)((0x50000024))));
}

int uart_puts(const char *s)
{
 const char * ps = s;
 int i = 0;
 while (ps[i])
 {
  uart_putchar(ps[i]);
  i++;
 }
 return i;
}
