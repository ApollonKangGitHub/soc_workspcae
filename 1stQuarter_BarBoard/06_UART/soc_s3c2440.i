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
# 1 "./include/common.h" 1
# 4 "soc_s3c2440.c" 2

char * itoa(int value, char * str)
{
 int high = value;
 int low = 0;
 int i = 0, j = 0, temp = 0;
 while(high){
  low = high % 10;
  str[i++] = low + '0';
  high = high / 10;
 }
 --i;
 while(i > j){
  temp = str[i];
  str[i--] = str[j];
  str[j++] = temp;
 }
 return str;
}
int soc_s3c2440_main(void)
{




 char ch;
 char strLen[12] = {0};
 char * pStr = ((void *)(0));
 int len = 0;

 uart_init();
 len = uart_puts("Hello S3C2440!\r\n");
 pStr = itoa(len, (char *)(strLen));
 uart_puts("string len = ");
 uart_puts(pStr);
 uart_puts("\r\n");
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



 return 0;
}
