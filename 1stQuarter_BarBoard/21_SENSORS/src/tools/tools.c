#include <tools.h>
#include <uart.h>
#include <font.h>
#include <oled.h>
#include <log.h>
#include <timer.h>

/*
 * 2020-2-16 15:44:27
 * 解决编译问题：
 * /opt/FriendlyARM/toolschain/4.4.3/lib/gcc/arm-none-linux-gnueabi/4.4.3//libgcc.a(_dvmd_lnx.o): In function `__div0':
 * /opt/FriendlyARM/mini2440/build-toolschain/working/src/gcc-4.4.3/libgcc/../gcc/config/arm/lib1funcs.asm:1093: undefined reference to `raise'
 * 代码参考u-boot-2012.04.01
 */
int raise (int signum)
{
	print_screen("\r\nraise: Signal # %d caught\n", signum);
	return 0;
}

void tool_delay(int sec)
{
	timer_mdelay(1000 * sec);
}
void tool_mdelay(int msec)
{
	timer_mdelay(msec);
}
void tool_udelay(int usec)
{
	timer_udelay(usec);
}

float tool_abs_f(float value)
{
	return ((value >= 0) ? (value) : (-value));
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

void *memcpy(void *dest, const void *src, int n)
{
	int index = 0;
	uint8 * pSrc = (uint8*)src;
	uint8 * pDest = (uint8*)dest;	
	if ((NULL == dest) || (NULL == src))
	{
		return NULL;
	}
	
	while (index < n)
	{
		pDest[index] = pSrc[index];
		index++;
	}

	return dest;
}

int tool_strncmp(const char * s1, const char * s2, int len)
{
	int i = 0;

	if (!s2 || !s1) {
		return ERROR;
	}
	
	while ((i < len) && s1[i] && s2[i]) {
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

/* 字符串拷贝，成功返回拷贝字符数，失败返回ERROR */
int tool_strnpy(char * dst, char * src, int len)
{
	int i = 0;
	if (!dst || !src) {
		return ERROR;
	}

	while ((i < len) && src[i])
	{
		dst[i] = src[i];
		i++;
	}

	return i;
}

/* 十进制的字符串表示形式，转换为十进制数值，只支持无符号uint32 */
BOOL tool_atoui(const char * str, uint32 * result)
{
	static char uintMax[TOOL_MAX_INT_STR_LEN] = {"4294967295"};
	int index = 0;
	const char *s = str;
	uint32 value = 0;
	int len = 0;
	
	len = tool_strlen(s);

	/* 传入的string值大于最大正数的string则不支持，返回FALSE */
	if ((len  > TOOL_MAX_INT_STR_LEN) || 
		((len == TOOL_MAX_INT_STR_LEN) && 
		(tool_strncmp(s, uintMax, TOOL_MAX_INT_STR_LEN) > 0))) {
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

	s[0] = base[temp / 16];
	s[1] = base[temp % 16];

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
	int lineStart = 0;
	int lineOver = 0;
	int printEnd = 0;
	int indexAddr = 0;
	int index = 0;
	char strBuf[_TOOL_PRINT_HEX_STR_LEN_];
	uint8 * pData = buf;

	set_buffer(strBuf, 0, sizeof(strBuf));
	print_screen("\r\n-------------------------------------------------------------");
	
	lineStart = startAddr / 16 * 16;
	printEnd = startAddr + len;
	
	if (printEnd % 16) {
		lineOver = ((printEnd / 16 + 1) * 16);
	}
	else {
		lineOver = (printEnd / 16 * 16);
	}

	for (indexAddr = lineStart; indexAddr < lineOver; indexAddr++)
	{
		index = indexAddr - startAddr;
		
		/* 先打印十六进制值，再打印字符 */
		if (0 == (indexAddr % 16) || (indexAddr == lineStart))
		{
			print_screen("\r\n [buffer:%x] ", indexAddr);			
		}

		/* 打印十六进制 */
		if ((indexAddr < startAddr) 
			|| (indexAddr >= printEnd))
		{
			print_screen("%c%c%c", 0x20, 0x20, 0x20);	
		}
		else
		{
			print_byteHex(pData[index]);
			print_screen("%c", 0x20);	
		}

		/* 缓存字符 */
		if (pData[index] >= 0x20 && pData[index] < 0x7F)
		{
			strBuf[indexAddr % 16] = pData[index];
		}
		/* 不属于打印范围但是要格式化补全的一行 */
		else if ((indexAddr < startAddr) 
			|| (indexAddr >= printEnd))
		{
			strBuf[indexAddr % 16] = ' ';
		}
		/* 非可显示字符 */
		else 
		{
			strBuf[indexAddr % 16] = '.';
		}

		/* 打印缓存的字符 */
		if ((0 == (indexAddr+1) % 16) && (indexAddr != lineStart))
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
	char *pTmp = NULL;
	int tmpLen = 0;
	int temp = 0;
	uint32 zeroNum = 0;
	char str[_TOOL_GET_STRING_LEN_];	/* 保证buffer长度，且清空   */
	
	if(NULL == pFmt){
		return -1;
	}
	va_start(pList, fmt);

	set_buffer(str, 0, sizeof(str));
	while('\0' != *pFmt){
		if('%' != *pFmt){
			tool_putChar(*pFmt++);
			continue;
		}
		pFmt++;		/* 跳过%，判断输出格式 */
		switch(*pFmt){
			case 'c':
				tool_putChar(va_arg(pList, int));
				break;
			case 'u':
				set_buffer(str, 0, sizeof(str));
				temp = va_arg(pList, int);
				/* 最大支持到2147483647的输出 */
				if (temp >> 31) {
					temp &= 0X7FFFFFFF;
				}
				tool_puts(tool_itoa(temp, str));
				break;
			case 'd':
				set_buffer(str, 0, sizeof(str));
				tool_puts(tool_itoa(va_arg(pList, int), str));
				break;
			case 'x':
			case 'X':
				set_buffer(str, 0, sizeof(str));
				tool_puts(tool_uitoxa(va_arg(pList, int), str));
				break;
			case 's':
				tool_puts(va_arg(pList, char*));
				break;
			case '0':
			{
				if ((*(pFmt+1) >= '2') && (*(pFmt+1) <= '9')) {
					if (*(pFmt+2) == 'u') {
						set_buffer(str, 0, sizeof(str));
						temp = va_arg(pList, int);
						if (temp >> 31) {
							temp &= 0X7FFFFFFF;
						}
						pTmp = tool_itoa(temp, str);
					}
					else if (*(pFmt+2) == 'd') {
						set_buffer(str, 0, sizeof(str));
						pTmp = tool_itoa(va_arg(pList, int), str);
					}
					else {
						/* 只支持%02d~%09d或%02u~%09u */
						tool_putChar(*--pFmt);
						break;
					}

					/* 格式化输出 */
					tmpLen = tool_strlen(pTmp);
					zeroNum = 0;
					while((tmpLen + zeroNum) < (*(pFmt+1) - '0')){
						tool_putChar('0');
						zeroNum++;
					}
					tool_puts(pTmp);

					/* 跳过%0xd的"xd"部分 */
					pFmt+=2;
				}
				break;
			}
			default:
				tool_putChar(*--pFmt);		/* 不是格式化输出，则输出百分号 */
				break;
		}
		pFmt++;
	}
	va_end(pList);
	return 0;
}

int print_screen_lcd(int x, int y, const char * fmt, ...)
{
	const char * pFmt = fmt;
	va_list pList = NULL;
	int temp = 0;
	int index = 0;
	int len = 0;
	int i = 0;
	char * pTmp = NULL;
	uint32 tmpLen = 0;
	uint32 zeroNum = 0;
	char * pStr = NULL;
	char str[_TOOL_GET_STRING_LEN_];	/* 保证buffer长度，且清空   */
	char strTmp[_TOOL_GET_STRING_LEN_];
	
	if(NULL == pFmt){
		return -1;
	}
	va_start(pList, fmt);

	set_buffer(strTmp, 0, sizeof(strTmp));
	set_buffer(str, 0, sizeof(str));
	while(('\0' != *pFmt) && (index < _TOOL_GET_STRING_LEN_)){
		if('%' != *pFmt){
			strTmp[index++] = *pFmt++;
			continue;
		}
		pFmt++;		/* 跳过%，判断输出格式 */
		switch(*pFmt){
			case 'c':
				strTmp[index++] = va_arg(pList, int);
				break;
			case 'u':
				i = 0;
				set_buffer(str, 0, sizeof(str));
				temp = va_arg(pList, int);
				/* 最大支持到2147483647的输出 */
				if (temp >> 31) {
					temp &= 0X7FFFFFFF;
				}
				tool_itoa(temp, str);
				len = tool_strlen(str);
				while ((len--) && (index < _TOOL_GET_STRING_LEN_))
				{
					strTmp[index++] = str[i++];
				}
				break;
			case 'd':
				i = 0;
				set_buffer(str, 0, sizeof(str));
				tool_itoa(va_arg(pList, int), str);
				len = tool_strlen(str);
				while ((len--) && (index < _TOOL_GET_STRING_LEN_))
				{
					strTmp[index++] = str[i++];
				}
				break;
			case 'x':
			case 'X':
				i = 0;
				set_buffer(str, 0, sizeof(str));
				tool_uitoxa(va_arg(pList, int), str);
				len = tool_strlen(str);
				while ((len--) && (index < _TOOL_GET_STRING_LEN_))
				{
					strTmp[index++] = str[i++];
				}
				break;
			case 's':
				i = 0;
				pStr = va_arg(pList, char*);
				len = tool_strlen(pStr);
				while ((len--) && (index < _TOOL_GET_STRING_LEN_))
				{
					strTmp[index++] = pStr[i++];
				}
				break;
			case '0':
			{
				if ((*(pFmt+1) >= '2') && (*(pFmt+1) <= '9')) {
					if (*(pFmt+2) == 'u') {
						set_buffer(str, 0, sizeof(str));
						temp = va_arg(pList, int);
						if (temp >> 31) {
							temp &= 0X7FFFFFFF;
						}
						pTmp = tool_itoa(temp, str);
					}
					else if (*(pFmt+2) == 'd') {
						set_buffer(str, 0, sizeof(str));
						pTmp = tool_itoa(va_arg(pList, int), str);
					}
					else {
						/* 只支持%02d~%09d或%02u~%09u */
						strTmp[index++] = *--pFmt;
						break;
					}

					/* 格式化输出 */
					i = 0;
					zeroNum = 0;
					len = tool_strlen(pTmp);
					while (((len + zeroNum) < (*(pFmt+1) - '0')) 
							&& (index < _TOOL_GET_STRING_LEN_))
					{
						strTmp[index++] = '0';
						zeroNum++;
					}
					while ((len--) && (index < _TOOL_GET_STRING_LEN_))
					{
						strTmp[index++] = pTmp[i++];
					}
					
					/* 跳过%0xd的"xd"部分 */
					pFmt+=2;
				}
				break;
			}
			default:
				strTmp[index++] = *--pFmt;
				break;
		}
		pFmt++;
	}
	va_end(pList);
	font_print_string(x,y,0xF,strTmp);
	return 0;
}

int print_screen_oled(int page, int col, const char * fmt, ...)
{
	const char * pFmt = fmt;
	va_list pList = NULL;
	int temp = 0;
	int index = 0;
	int len = 0;
	int i = 0;
	char * pTmp = NULL;
	uint32 tmpLen = 0;
	uint32 zeroNum = 0;
	char * pStr = NULL;
	char str[_TOOL_GET_STRING_LEN_];	/* 保证buffer长度，且清空   */
	char strTmp[_TOOL_GET_STRING_LEN_];
	
	if(NULL == pFmt){
		return -1;
	}
	va_start(pList, fmt);

	set_buffer(strTmp, 0, sizeof(strTmp));
	set_buffer(str, 0, sizeof(str));
	while(('\0' != *pFmt) && (index < _TOOL_GET_STRING_LEN_)){
		if('%' != *pFmt){
			strTmp[index++] = *pFmt++;
			continue;
		}
		pFmt++;		/* 跳过%，判断输出格式 */
		switch(*pFmt){
			case 'c':
				strTmp[index++] = va_arg(pList, int);
				break;
			case 'u':
				i = 0;
				set_buffer(str, 0, sizeof(str));
				temp = va_arg(pList, int);
				/* 最大支持到2147483647的输出 */
				if (temp >> 31) {
					temp &= 0X7FFFFFFF;
				}
				tool_itoa(temp, str);
				len = tool_strlen(str);
				while ((len--) && (index < _TOOL_GET_STRING_LEN_))
				{
					strTmp[index++] = str[i++];
				}
				break;
			case 'd':
				i = 0;
				set_buffer(str, 0, sizeof(str));
				tool_itoa(va_arg(pList, int), str);
				len = tool_strlen(str);
				while ((len--) && (index < _TOOL_GET_STRING_LEN_))
				{
					strTmp[index++] = str[i++];
				}
				break;
			case 'x':
			case 'X':
				i = 0;
				set_buffer(str, 0, sizeof(str));
				tool_uitoxa(va_arg(pList, int), str);
				len = tool_strlen(str);
				while ((len--) && (index < _TOOL_GET_STRING_LEN_))
				{
					strTmp[index++] = str[i++];
				}
				break;
			case 's':
				i = 0;
				pStr = va_arg(pList, char*);
				len = tool_strlen(pStr);
				while ((len--) && (index < _TOOL_GET_STRING_LEN_))
				{
					strTmp[index++] = pStr[i++];
				}
				break;
			case '0':
			{
				if ((*(pFmt+1) >= '2') && (*(pFmt+1) <= '9')) {
					if (*(pFmt+2) == 'u') {
						set_buffer(str, 0, sizeof(str));
						temp = va_arg(pList, int);
						if (temp >> 31) {
							temp &= 0X7FFFFFFF;
						}
						pTmp = tool_itoa(temp, str);
					}
					else if (*(pFmt+2) == 'd') {
						set_buffer(str, 0, sizeof(str));
						pTmp = tool_itoa(va_arg(pList, int), str);
					}
					else {
						/* 只支持%02d~%09d或%02u~%09u */
						strTmp[index++] = *--pFmt;
						break;
					}

					/* 格式化输出 */
					i = 0;
					zeroNum = 0;
					len = tool_strlen(pTmp);
					while (((len + zeroNum) < (*(pFmt+1) - '0')) 
							&& (index < _TOOL_GET_STRING_LEN_)) 
					{
						strTmp[index++] = '0';
						zeroNum++;
					}
					while ((len--) && (index < _TOOL_GET_STRING_LEN_))
					{
						strTmp[index++] = pTmp[i++];
					}
					
					/* 跳过%0xd的"xd"部分 */
					pFmt+=2;
				}
				break;
			}
			default:
				strTmp[index++] = *--pFmt;
				break;
		}
		pFmt++;
	}
	va_end(pList);
	oled_print(page,col,strTmp);
	return 0;
}

int tool_strlen(const char * str)
{	
	const char *p = str;
	int len = 0;
	
	if (NULL == p) {
		return ERROR;
	}

	while(*p != '\0') {
		len++;
		p++;
	}
	
	return len;
}

/*
 * 获取串口输入，以空格或回车结束
 * 这里有个坑，如果传入的值是大于255的，超过255的部分传进来就被截断了，先注释出来，随后排查原因；
 * 2020-2-16 12:11:38：uint8 loop_max = len截断了，我靠，太不细心了
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
	uint32 loop_max = len;
	uint32 i = 0;

	if(NULL == str){
		return NULL;
	}

	if (loop_max == 0)
	{
		print_screen("\r\n get_string error len :%d", loop_max);
		return NULL;
	}
	
	set_buffer(s, 0, len);
	while(loop_max > i+1){
		s[i] = (uint8)tool_getChar();

		/* 退格删除，清除buffer缓存，并重新获取 */
		if('\b' == s[i]){
			if(0 == i){
				continue;			/* 防止backspace删除超出范围 */
			}
			s[i] = 0;				/* 清除回退控制符buffer */
			s[--i] = 0;				/* 清除控制对象buffer */
			tool_putChar('\b');		/* 退格准备清除屏幕退格对象 */
			tool_putChar(32);		/* 清除屏幕退格对象	*/
			tool_putChar('\b');		/* 再退格，光标回退 */
			continue;
		}
		else if(('\r' == s[i]) 
			|| ('\n' == s[i]) 
			|| ((GET_STR_WORD == flag) && (32 == s[i]))){	
			/* 换行或空格(flag为获取单词，而非整行)结束输入 */
			tool_putChar(s[i]);
			if('\r' == s[i]){
				tool_putChar('\r');
			}else if('\n' == s[i]){
				tool_putChar('\n');
			}
			s[i] = '\0';
			break;
		}
		else{
			/* 正常回显,str[]录入不区分控制字符，所以输入时尽量不要用上下左右键之类的按键 */
			tool_putChar(s[i]);		
			i++;
		}
	}
	return s;
}

int scan_screen(const char * fmt, ...)
{

}

