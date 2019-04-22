#include <led.h>
#include <uart.h>
#include <tools.h>

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
	pStr = uitoa(len, (char *)(strLen));
	uart_puts("string len = ");
	uart_puts(pStr);
	uart_puts("\r\n");
#if 1
	print("\r\ntest1:%d, test2:%d\r\n", 2147483647, 54552);
	print("\r\nTest at [%s - %X] %c Please Enter:\r\n", __FUNCTION__, __LINE__, 43);
	print("\r\nTest at [%s - %X] %X Please Enter:\r\n", __FUNCTION__, __LINE__, 65536);
#endif
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