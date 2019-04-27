# 1 "src/tools/tools.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/tools/tools.c"
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
# 2 "src/tools/tools.c" 2
# 1 "./include/uart/uart.h" 1



extern void uart_init();
extern int uart_getchar(void);
extern void uart_putchar(int ch);
extern int uart_puts(const char * s);
# 3 "src/tools/tools.c" 2
# 1 "./include/log/log.h" 1





typedef enum soc_debug_level{
 SOC_DBG_NORMAL,
 SOC_DBG_WARN,
 SOC_DBG_ERROR,
 SOC_DBG_LOG,
 SOC_DBG_MAX
}SOC_DBG_LEV;
# 37 "./include/log/log.h"
extern int gSysDebugFlag;
# 4 "src/tools/tools.c" 2


char * uitoa(uint32 value, char * str)
{
 int high = value;
 int low = 0;
 int i = 0, j = 0, temp = 0;

 if(!value){
  str[0] = '0';
  return str;
 }


 while(high){
  low = high % 10;
  high = high / 10;
  str[i++] = low + '0';
 }
 --i;

 while(i > j){
  temp = str[i];
  str[i--] = str[j];
  str[j++] = temp;
 }
 return str;
}


uint32 atoui(const char * str)
{
 const char *s = str;
 uint32 value = 0;
 while(*s){
  if(*s < '0' || *s > '9'){
   return -1;
  }
  value = (value << 3) + (value << 1);
  value += (*s - '0');

  s++;
 }
 return value;
}


char * uitoxa(uint32 value, char * str)
{
 char * s = str;
 char base[] = "0123456789ABCDEF";
 int high = value, low = 0;

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
 return ((void *)(0));
}


uint8 * set_buffer(uint8 * const str, uint8 set, uint32 len)
{
 int i = 0;
 if(!str){
  return ((void *)(0));
 }
 for(i = 0; i < len; i++){
  str[i] = set;
 }
 return str;
}


int print_screen(const char * fmt, ...)
{
 const char * pFmt = fmt;
 va_list pList = ((void *)(0));
 char str[(12)] = {0};

 if(((void *)(0)) == pFmt){
  return -1;
 }
 (pList = (va_list)&fmt + ((sizeof(fmt) + sizeof(int) - 1) & ~(sizeof(int) - 1)));

 set_buffer(str, 0, sizeof(str));
 while('\0' != *pFmt){
  if('%' != *pFmt){
   uart_putchar(*pFmt++);
   continue;
  }
  pFmt++;
  switch(*pFmt){
   case 'c':
    uart_putchar((*(int*)((pList += ((sizeof(int) + sizeof(int) - 1) & ~(sizeof(int) - 1))) - ((sizeof(int) + sizeof(int) - 1) & ~(sizeof(int) - 1)))));
    break;
   case 'u':
   case 'd':
    set_buffer(str, 0, sizeof(str));
    uart_puts(uitoa((*(int*)((pList += ((sizeof(int) + sizeof(int) - 1) & ~(sizeof(int) - 1))) - ((sizeof(int) + sizeof(int) - 1) & ~(sizeof(int) - 1)))), str));
    break;
   case 'x':
   case 'X':
    set_buffer(str, 0, sizeof(str));
    uart_puts(uitoxa((*(int*)((pList += ((sizeof(int) + sizeof(int) - 1) & ~(sizeof(int) - 1))) - ((sizeof(int) + sizeof(int) - 1) & ~(sizeof(int) - 1)))), str));
    break;
   case 's':
    uart_puts((*(char**)((pList += ((sizeof(char*) + sizeof(int) - 1) & ~(sizeof(int) - 1))) - ((sizeof(char*) + sizeof(int) - 1) & ~(sizeof(int) - 1)))));
    break;
   default:

    uart_putchar(*--pFmt);
    break;
  }
  pFmt++;
 }
 (pList = (va_list)(0));
 return 0;
}


char * get_word(char * str, uint8 len)
{
 return get_string(str, len, GET_STR_WORD);

}


char * get_line(char * str, uint8 len)
{
 return get_string(str, len, GET_STR_LINE);
}


char * get_string(char * str, uint8 len, GET_STRING_FLAG flag)
{
 char *s = str;
 char ch = 0;
 uint8 loop_max = len;
 uint8 i = 0;

 if(((void *)(0)) == str){
  return ((void *)(0));
 }
 set_buffer(s, 0, len);
 while(loop_max > i+1){
  s[i] = (uint8)uart_getchar();

  if('\b' == s[i]){
   if(0 == i){
    continue;
   }
   s[i] = 0;
   s[--i] = 0;
   uart_putchar('\b');
   uart_putchar(32);
   uart_putchar('\b');

   continue;
  }
  else if(('\r' == s[i]) || ('\n' == s[i]) || ((GET_STR_WORD == flag) && (32 == s[i]))){

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

   uart_putchar(s[i]);
   i++;
  }
 }
 return s;
}

int scan_screen(const char * fmt, ...)
{

}
