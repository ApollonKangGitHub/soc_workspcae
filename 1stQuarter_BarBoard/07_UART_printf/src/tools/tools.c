#include <tools.h>
#include <uart.h>

/* int整数value转十进制字符串str，并返回字符串首地址，如1235，转成"1235" */
char * uitoa(uint32 value, char * str)
{
	int high = value;
	int low = 0;
	int i = 0, j = 0, temp = 0;
	/* 余数对应的ASCII码依次存放在数组中 */
	while(high){
		low = high % 10;
		high = high / 10;
		str[i++] = low + '0';
	}
	--i;
	/* 反转，高低位调换字符串顺序 */
	while(i > j){
		temp = str[i];
		str[i--] = str[j];
		str[j++] = temp;
	}
	return str;
}

/* int整数value转十六进制字符串str，并返回字符串首地址，如1235，转成"0X000004D3" */
char * uitoxa(uint32 value, char * str)
{
	char * s = str;
	char base[] = "0123456789ABCDEF";
	int high = value, low = 0;
	/* 32bit十六进制，0xFFFFFFFF，占10Byte， 倒序填充index从9开始 */
	int i = 9; 

	while(high){
		low = high % 16;
		high = high / 16;
		s[i--] = base[low];
	}
	while(i > 1){
		s[i--] = '0';
	}
	s[1] = 'X';
	s[0] = '0';
	
	return str;
}
char * ftoa(float value, char * str)
{
	return NULL;
}

uint8 * set_buffer(uint8 * const str, uint8 set, uint32 len)
{
	int i = 0;
	if(!str){
		return NULL;
	}
	for(i = 0; i < len; i++){
		str[i] = set;
	}
	return str;
}

/* 简单的print函数，用于调试即可 */
int print(const char * fmt, ...)
{
	const char * pFmt = fmt;
	va_list pList = NULL;
	char str[CHANGE_TO_STR_BUFF_LEN] = {0};	/* 保证buffer长度，且清空   */
	
	if(NULL == pFmt){
		uart_puts("error string for print!!\n");
		return -1;
	}
	va_start(pList, fmt);

	set_buffer(str, 0, sizeof(str));
	while('\0' != *pFmt){
		if('%' != *pFmt){
			uart_putchar(*pFmt++);
			continue;
		}
		pFmt++;		/* 跳过%，判断输出格式 */
		switch(*pFmt){
			case 'c':
				uart_putchar(va_arg(pList, int));
				break;
			case 'u':
			case 'd':
				set_buffer(str, 0, sizeof(str));
				uart_puts(uitoa(va_arg(pList, int), str));
				break;
			case 'x':
			case 'X':
				set_buffer(str, 0, sizeof(str));
				uart_puts(uitoxa(va_arg(pList, int), str));
				break;
			case 's':
				uart_puts(va_arg(pList, char*));
				break;
			default:
				/* 不考虑%%%%的情况 */
				uart_putchar('%');		/* 不是格式化输出，则先输出百分号 */
				uart_putchar(*pFmt);	/* 再输出正常字符 */
				break;
		}
		pFmt++;
	}
	va_end(pList);

}

