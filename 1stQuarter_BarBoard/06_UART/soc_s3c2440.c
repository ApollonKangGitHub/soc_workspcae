#include <led.h>
#include <uart.h>
#include <common.h>

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
#if 0
	led_init();
	led_running();
#elif 1
	char ch;
	char strLen[12] = {0};
	char * pStr = NULL;
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
#else
	
#endif
	return 0;
}