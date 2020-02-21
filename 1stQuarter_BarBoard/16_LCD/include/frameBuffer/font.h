#ifndef _FONT_H_
#define _FONT_H_

#include <tools.h>

extern void font_init(void);
extern void font_print_char(uint32 x, uint32 y, uint32 color, uint8 ch);
extern void font_print_string(uint32 x, uint32 y, uint32 color, char * str);

#endif	/* _FONT_H_ */