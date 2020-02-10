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

void tool_calc_mem_size
(
	uint32 size, 
	uint32 *gbbyte, 
	uint32 *mbbyte, 
	uint32 *kbyte, 
	uint32 *byte
)
{
	uint32 temp = size;
	
	*gbbyte = temp / _TOOL_GB_BASE_;
	temp = temp % _TOOL_GB_BASE_;
	*mbbyte = temp / _TOOL_MB_BASE_;
	temp = temp % _TOOL_MB_BASE_;
	*kbyte = temp / _TOOL_KB_BASE_;
	*byte = temp % _TOOL_KB_BASE_;
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
#if (SOC_S3C2440_THUMB_INSTRUCTION_TEST == TRUE)
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

int tool_strncmp(const char * s1, const char * s2, int len)
{
	int i = 0;
	
	while ((i < len) && (s1[i] != '\0') && (s2[i] != '\0')) {
		if (s1[i] == s2[i]) {
			i++;
			continue;
		}
		else if (s1[i] > s2[i]) {
			return TOOL_STR_CMP_GREATER(i);
		}
		else{
			return TOOL_STR_CMP_LESS(i);
		}
	}

	if (i < len) {
		if (s1[i] == '\0') {
			return TOOL_STR_CMP_LESS(i);
		}
		else {
			return TOOL_STR_CMP_GREATER(i);
		}
	}

	return TOOL_STR_CMP_EQUAL;
}

/* 十进制的字符串表示形式，转换为十进制数值，只支持无符号uint32 */
BOOL tool_atoui(const char * str, uint32 * result)
{
	static char uintMax[TOOL_MAX_INT_STR_LEN] = {"4294967295"};
	int index = 0;
	const char *s = str;
	uint32 value = 0;

	/* 传入的string值大于最大正数的string则不支持，返回FALSE */
	if (tool_strncmp(s, uintMax, TOOL_MAX_INT_STR_LEN) > 0) {
		return FALSE;
	}
	
	while(s[index]) {
		if((s[index] < '0') 
			|| (s[index] > '9') 
			|| (index >= TOOL_MAX_INT_STR_LEN)) {
			return FALSE;
		}

		/* 即value = value x 10 + low */
		value = (value << 3) + (value << 1);	
		value += (s[index] - '0');
		index++;
	}

	*result = value;
	return TRUE;
}

/* 十六进制的字符串表示形式，转换为十进制数值，只支持无符号uint32 */
BOOL tool_atoux(const char * str, uint32 * result)
{
	int len = 0;
	static char asciiTable[256] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 1:0~15 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 2:16~31 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 3:32~47 */
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1,	/* 4:48~63 */
		-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 5:64~79 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 6:80~95 */
		-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 7:96~111 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 8:112~127 */

		/* 128~255 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 9 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 10 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 11 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 12 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 13 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 14 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	/* 15 */
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1		/* 16 */
	};

	const char *s = str;
	uint32 value = 0;

	/* 检查并跳过"0x"开头 */
	if (s[0] != '0' || ((s[1] != 'x') && (s[1] != 'X')))
	{
		return FALSE;
	}
	
	s += 2;
	while(*s) {
		if((asciiTable[*s] < 0) || (asciiTable[*s] > 15)){
			return FALSE;
		}
		
		/* 即value = value x 16 + low */
		value = value << 4;
		value += asciiTable[*s];
		s++;
	}
	
	/* 最高只支持32位数，"0xFFFFFFFF" */
	if (((s - str) > 10) || ((s - str) < 3))
	{
		return FALSE;
	}
	
	*result = value;
	return TRUE;

}

/* byte整数value转十六进制字符串str，并返回字符串首地址 */
char * tool_ubtoxa(uint8 value, char * str)
{
	char * s = str;
	char base[] = "0123456789ABCDEF";
	uint16 temp = 0x0;

	temp |= value;

#if (SOC_S3C2440_THUMB_INSTRUCTION_TEST == TRUE)
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
	//print_screen("\r\n ==============len=%d", len);
	if(!str){
		return NULL;
	}
	for(i = 0; i < len; i++){
		str[i] = set;
	}
	return str;
}

void print_byteHex(uint8 value)
{
	static char hex[17] = {"0123456789ABCDEF"};
	
	print_screen("%c%c", hex[(value >> 4) & 0xF], hex[value & 0xF]);
}

void print_hexStr_multiple(uint8 * buf, int len, uint32 startAddr)
{
	int start = 0;
	int over = 0;
	int indexAddr = 0;
	int index = 0;
	char strBuf[_TOOL_PRINT_HEX_STR_LEN_];
	uint8 * pData = buf;

	set_buffer(strBuf, 0, sizeof(strBuf));
	print_screen("\r\n-------------------------------------------------------------");

	start = startAddr / 16 * 16;
	over = startAddr + len;
	
	if (over % 16) {
		over = ((over / 16 + 1) * 16);
	}
	else {
		over = (over / 16 * 16);
	}

	for (indexAddr = start; indexAddr < over; indexAddr++)
	{
		index = indexAddr - startAddr;
		
		/* 先打印十六进制值，再打印字符 */
		if (0 == (indexAddr % 16) || (indexAddr == start))
		{
			print_screen("\r\n [buffer:%x] ", indexAddr);			
		}

		/* 打印十六进制 */
		if ((indexAddr < startAddr) 
			|| (indexAddr >= startAddr + len))
		{
			print_screen("%c%c%c", 0x20, 0x20, 0x20);	
		}
		else
		{
			print_byteHex(pData[index]);
			print_screen("%c", 0x20);	
		}

		/* 缓存字符 */
		if (pData[index] > 0x20 && pData[index] < 0x7F)
		{
			strBuf[indexAddr % 16] = pData[index];
		}
		/* 不属于打印范围但是要格式化补全的一行 */
		else if ((indexAddr < startAddr) 
			|| (indexAddr >= startAddr + len))
		{
			strBuf[indexAddr % 16] = ' ';
		}
		/* 非可显示字符 */
		else 
		{
			strBuf[indexAddr % 16] = '.';
		}

		/* 打印缓存的字符 */
		if ((0 == (indexAddr+1) % 16) && (indexAddr != start))
		{
			print_screen(" | %s |", strBuf);
			set_buffer(strBuf, 0, sizeof(strBuf));
		}
	}

	print_screen("\r\n-------------------------------------------------------------");
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
	char str[_TOOL_GET_STRING_LEN_];	/* 保证buffer长度，且清空   */
	
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

int tool_strlen(char * str)
{	
	char *p = str;
	int len = 0;
	
	if (NULL == p) {
		return -1;
	}

	while(*p != '\0') {
		len++;
		p++;
	}
	
	return len;
}

/*
 * 获取串口输入，以空格或回车结束
 * 这里有个坑，如果传入的值是大于255的，超过255的部分传进来就被截断了
 * 先注释出来，随后排查原因
 */
char * get_word(char * str, uint32 len)
{
	return get_string(str, len, GET_STR_WORD);
}

/* 获取串口输入，以回车结束 */
char * get_line(char * str, uint32 len)
{
	return get_string(str, len, GET_STR_LINE);
}

/* 获取串口输入，实现细节 */
char * get_string(char * str, uint32 len, GET_STRING_FLAG flag)
{
	char *s = str;
	char ch = 0;
	uint8 loop_max = len;
	uint8 i = 0;

	if(NULL == str){
		return NULL;
	}

	if (len <= 0)
	{
		print_screen("\r\n------------------get_string error len :%d", loop_max);
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

