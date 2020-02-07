#ifndef _UART_H_
#define _UART_H_

extern void uart_init();
extern int uart_getchar(void);
extern void uart_putchar(int ch);
extern int uart_puts(const char * s);

#endif