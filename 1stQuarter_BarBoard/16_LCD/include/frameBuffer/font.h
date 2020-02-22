#ifndef _FONT_H_
#define _FONT_H_

#include <tools.h>
#include <paletee.h>

extern void font_init(void);
extern void font_print_char(uint32 x, uint32 y, paletee_256_type_t color, uint8 ch);
extern void font_print_string(uint32 x, uint32 y, paletee_256_type_t color, char * str);

#endif	/* _FONT_H_ */