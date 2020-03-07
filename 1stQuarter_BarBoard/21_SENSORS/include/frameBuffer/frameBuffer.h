#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include <tools.h>
#include <paletee.h>

typedef struct {
	uint32 x_res;
	uint32 y_res;
	uint32 bpp;
	uint32 fb_base;
}fb_lcd_para;

extern int frameBuffer_init(void);
extern void frameBuffer_fullScreen(paletee_256_type_t color);
extern void frameBuffer_clear(void);
extern void frameBuffer_set_point(uint32 x, uint32 y, paletee_256_type_t color);
extern void frameBuffer_clear_lines(int startLine, int endLine);

#endif	/* _FRAME_BUFFER_H_ */
