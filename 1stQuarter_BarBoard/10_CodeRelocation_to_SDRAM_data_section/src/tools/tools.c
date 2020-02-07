#include <tools.h>
#include <uart.h>
#include <log.h>

void tool_dealy(uint16 sec)
{
	int delay = 400000;
	while(sec--){
		while(delay--){
			;/* nop */
		}
	}
}


/* int整数value转十进制字符串str，并返回字符串首地址，如1235，转成"1235" */
char * uitoa(uint32 value, char * str)
{
	int high = value;
	int low = 0;
	int i = 0, j = 0, temp = 0;

	if(!value){
		str[0] = '0';
		return str;
	}
	
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

/* 十进制的字符串表示形式，转换为十进制数值，只支持无符号uint32 */
uint32 atoui(const char * str)
{
	const char *s = str;
	uint32 value = 0;
	while(*s){
		if(*s < '0' || *s > '9'){
			return -1;
		}
		value = (value << 3) + (value << 1);	/* 即value = value x 10 */
		value += (*s - '0');
		//SOC_DEBUG_PRINT(SOC_DBG_LOG, "value = %d\r\n", value);
		s++;
	}
	return value;
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

/* 清空buffer */
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
int print_screen(const char * fmt, ...)
{
	const char * pFmt = fmt;
	va_list pList = NULL;
	char str[CHANGE_TO_STR_BUFF_LEN] = {0};	/* 保证buffer长度，且清空   */
	
	if(NULL == pFmt){
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
				uart_putchar(*--pFmt);		/* 不是格式化输出，则输出百分号 */
				break;
		}
		pFmt++;
	}
	va_end(pList);
	return 0;
}

/* 获取串口输入，以空格或回车结束 */
char * get_word(char * str, uint8 len)
{
	return get_string(str, len, GET_STR_WORD);

}

/* 获取串口输入，以回车结束 */
char * get_line(char * str, uint8 len)
{
	return get_string(str, len, GET_STR_LINE);
}

/* 获取串口输入，实现细节 */
char * get_string(char * str, uint8 len, GET_STRING_FLAG flag)
{
	char *s = str;
	char ch = 0;
	uint8 loop_max = len;
	uint8 i = 0;

	if(NULL == str){
		return NULL;
	}
	set_buffer(s, 0, len);
	while(loop_max > i+1){
		s[i] = (uint8)uart_getchar();
		/* 退格删除，清除buffer缓存，并重新获取 */
		if('\b' == s[i]){
			if(0 == i){
				continue;			/* 防止backspace删除超出范围 */
			}
			s[i] = 0;				/* 清除回退控制符buffer */
			s[--i] = 0;				/* 清除控制对象buffer */
			uart_putchar('\b');		/* 退格准备清除屏幕退格对象 */
			uart_putchar(32);		/* 清除屏幕退格对象	*/
			uart_putchar('\b');		/* 再退格，光标回退 */
			//SOC_DEBUG_PRINT(SOC_DBG_LOG, "i = %d\r\n", i);
			continue;
		}
		else if(('\r' == s[i]) || ('\n' == s[i]) || ((GET_STR_WORD == flag) && (32 == s[i]))){	
			/* 换行或空格(flag为获取单词，而非整行)结束输入 */
			uart_putchar(s[i]);
			if('\r' == s[i]){
				uart_putchar('\n');
			}else if('\n' == s[i]){
				uart_putchar('\r');
			}
			s[i] = '\0';
			break;
		}
		else{
			/* 正常回显,str[]录入不区分控制字符，所以输入时尽量不要用上下左右键之类的按键 */
			uart_putchar(s[i]);		
			i++;
		}
	}
	return s;
}

int scan_screen(const char * fmt, ...)
{

}

