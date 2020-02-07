#include <tools.h>
#include <uart.h>
#include <log.h>

void tool_dealy(uint16 sec)
{
	int delay = 400000;
	while(sec--){
		delay = 400000;
		while(delay--){
			;/* nop */
		}
	}
}

/* 对给定非负数值取整和取余，THUMB指令集专用函数，效率极其低下 */
void tool_getRoundingRemainder
(
	int target, 		/* 操作对象整数 */
	int base,			/* 取余取整基数 */
	int * round,		/* 取整结果 */
	int * remain		/* 取余结果 */
)
{
	int high = target;
	int low = 0;
	int roundTemp = 0;
	
	while (high >= base)
	{
		high -= base;
		roundTemp++;
	}
	
	*round = roundTemp;
	*remain = high;
}


/* 
 * 大端存储高位存在低字节，低位存在高字节，类似于数据按字符串存储，
 * 比如char str[] = {"123456"}; '1'为高位，但是'1'存在低字节str[0]
 */
BOOL tool_isBigEndOrder(void)
{
	uint16 value = 0x1122;
	uint8 * temp = NULL;

	temp = (uint8 *)(&value);

	if (0x11 == temp[0])
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* int整数value转十进制字符串str，并返回字符串首地址，如1235，转成"1235" */
char * tool_itoa(uint32 value, char * str)
{
	int high = value;
	int low = 0;
	char minStr[12] = {"-2147483648"};
	int i = 0, j = 0, temp = 0;

	/* 0以及-2147483648不适用于随后的算法 */
	if(0x0 == value){
		str[0] = '0';
		return str;
	}
	else if (0x80000000 == value)
	{
		while(i < sizeof(minStr))
		{
			str[i] = minStr[i];
			i++;
		}
		return str;
	}

	/* 负数,存在符号时，符号不反转 */
	if (value >> 31)
	{
		high = ~value + 1;
		str[i++] = '-';
		j++;
	}
	
	/* 余数对应的ASCII码依次存放在数组中 */
	while(high){
#if (SOC_S3C2440_THUMB_INTERRUPT_TEST == TRUE)
		(void)tool_getRoundingRemainder(high, 10, &high, &low);
#else
		low = high % 10;
		high = high / 10;
#endif
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
uint32 tool_atoui(const char * str)
{
	const char *s = str;
	uint32 value = 0;
	while(*s){
		if(*s < '0' || *s > '9'){
			return -1;
		}
		value = (value << 3) + (value << 1);	/* 即value = value x 10 */
		value += (*s - '0');
		s++;
	}
	return value;
}

/* byte整数value转十六进制字符串str，并返回字符串首地址 */
char * tool_ubtoxa(uint8 value, char * str)
{
	char * s = str;
	char base[] = "0123456789ABCDEF";
	uint16 temp = 0x0;

	temp |= value;

#if (SOC_S3C2440_THUMB_INTERRUPT_TEST == TRUE)
	s[0] = base[(temp >> 4) & 0x0FFFFFFF];
	s[1] = base[temp & 0x0000000F];
#else
	s[0] = base[temp / 16];
	s[1] = base[temp % 16];
#endif
		
	return str;
}

/* 
 * int整数value转十六进制字符串str，并返回字符串首地址，如1235，转成"0X000004D3"
 * 由于原始的基于int整数的循环取余取整法，对于负数需要特殊处理，因此修改为该新的方式
 */
char * tool_uitoxa(uint32 value, char * str)
{
	char * s = str;
	uint8 * pTemp = (uint8 *)(&value);

	s[0] = '0';
	s[1] = 'X';

	if (tool_isBigEndOrder())
	{
		/* 大端存储 */
		tool_ubtoxa(pTemp[0], &s[2]);
		tool_ubtoxa(pTemp[1], &s[4]);
		tool_ubtoxa(pTemp[2], &s[6]);
		tool_ubtoxa(pTemp[3], &s[8]);
	}
	else
	{
		/* 小端存储 */
		tool_ubtoxa(pTemp[0], &s[8]);
		tool_ubtoxa(pTemp[1], &s[6]);
		tool_ubtoxa(pTemp[2], &s[4]);
		tool_ubtoxa(pTemp[3], &s[2]);
	}
	
	return str;
}

char * tool_ftoa(float value, char * str)
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

/* 
 * 简单的print函数，用于调试即可
 * uart初始化、sdram初始化完成，且如果有重定位问题，在重定位完成后，
 * print_screen()才能使用,因为该函数中有绝对地址的访问
 */

int print_screen(const char * fmt, ...)
{
	const char * pFmt = fmt;
	va_list pList = NULL;
	int temp = 0;
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
				set_buffer(str, 0, sizeof(str));
				temp = va_arg(pList, int);
				/* 最大支持到2147483647的输出 */
				if (temp >> 31) {
					temp &= 0X7FFFFFFF;
				}
				uart_puts(tool_itoa(temp, str));
				break;
			case 'd':
				set_buffer(str, 0, sizeof(str));
				uart_puts(tool_itoa(va_arg(pList, int), str));
				break;
			case 'x':
			case 'X':
				set_buffer(str, 0, sizeof(str));
				uart_puts(tool_uitoxa(va_arg(pList, int), str));
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

