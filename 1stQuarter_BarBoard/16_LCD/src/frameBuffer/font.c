#include <font.h>
#include <tools.h>
#include <font_8x16.h>
#include <frameBuffer.h>

extern const uint8 gFontdata_8x16[FONT_DATAMAX];
extern fb_lcd_para gFb_LcdPara;

void font_init(void)
{

}

void font_print_char(uint32 x, uint32 y, paletee_256_type_t color, uint8 ch)
{
	uint8 * pChFont = NULL;
	uint32 line = 0;
	uint32 pixel = 0;
	uint8 data = 0;
	uint8 bit = 7;
	
	/* 获取字符点阵 */
	pChFont = (uint8*)(&gFontdata_8x16[ch * FONT_COL_MAX]);

	/* 将点阵值写入对应位置 */
	for (line = y; line < (y + FONT_COL_MAX); line++)
	{
		data = *pChFont++;
		bit = 7;
		for (pixel = x; pixel < (x + FONT_ROW_MAX); pixel++)
		{
			if (data & (0x1 << bit))
			{
				frameBuffer_set_point(pixel, line, color);
			}
			bit--;
		}
	}
}

void font_print_string(uint32 x, uint32 y, paletee_256_type_t color, char * str)
{
	uint32 i = 0;
	uint32 j = 0;
	uint32 x_tmp = x;
	uint32 y_tmp = y;
	
	while(str[i]) {
		if ('\n' == str[i]) {
			y_tmp += FONT_COL_MAX;
		}
		else if ('\r' == str[i]) {
			x_tmp = 0;
		}
		else {
			font_print_char(x_tmp, y_tmp, color, str[i]);
			x_tmp += FONT_ROW_MAX;
			if (x_tmp >= gFb_LcdPara.x_res)
			{
				x_tmp = 0;
				y_tmp += FONT_COL_MAX;
			}
		}

		i++;
	}
}