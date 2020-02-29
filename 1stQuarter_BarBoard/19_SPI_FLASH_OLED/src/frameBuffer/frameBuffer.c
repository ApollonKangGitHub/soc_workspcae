#include <frameBuffer.h>
#include <lcd_common.h>
#include <tools.h>
#include <paletee.h>

fb_lcd_para gFb_LcdPara = {
	.x_res = 0,
	.y_res = 0,
	.bpp = 0,
	.fb_base = 0
};

extern const rgb_256_info_t gRgb256Info[PALETEE_TYPE_NUM];

/* 枚举转为bpp实际占用framebuffer的bit值 */
static uint32 frameBuffer_bpp_convert(bpp_type_t bpp)
{
	switch(bpp)
	{
		case bpp_type_8bits:
			return 8;
		case bpp_type_16bits:
			return 16;
		case bpp_type_24bits:
		case bpp_type_32bits:
		default:
			return 32;
	}
	
	return 32;
}

/* RGB888转换为RGB565 */
static uint16 frameBuffer_32bpp_convert_to_16bpp(uint32 rgb888)
{
	uint32 r = (rgb888 >> 16) & 0xFF;
	uint32 g = (rgb888 >> 8) & 0xFF;
	uint32 b = (rgb888 >> 0) & 0xFF;
	uint16 rgb565 = 0x0;

	/* rgb888 -> rgb565 */
	r >>= (8-5);
	g >>= (8-6);
	b >>= (8-5);

	rgb565 = (uint16)((r << 11) | (g << 5) | (b));
	return rgb565;
}


/* 
 * 设置某一点的像素颜色,支持真彩色和伪彩色
 * color小于paletee_256_type_Max的用伪调色板的彩色（对8bpp）
 * 使用真彩色的24bpp可以用 RGB888_TRUE_COLOR 位或上真彩色RGB888数据即可
 */
void frameBuffer_set_point
(
	uint32 x, 
	uint32 y, 
	paletee_256_type_t color
)
{
	uint32 pixelBase = 0;
	uint8 * pByte = NULL;
	uint16 * pWord = NULL;
	uint32 * pDoubleWrod = NULL;
	
	if ((x < 0) || (x > gFb_LcdPara.x_res) 
		|| (y < 0) || (y > gFb_LcdPara.y_res))
	{
		return;
	}

	pixelBase = gFb_LcdPara.fb_base + (((gFb_LcdPara.x_res * y) + x) * gFb_LcdPara.bpp / 8); 
	switch (gFb_LcdPara.bpp)
	{
		case 8:
			pByte = (uint8 *)pixelBase;
			*pByte = color % PALETEE_TYPE_NUM;
			break;
		case 16:
			pWord = (uint16 *)pixelBase;
			*pWord = (color & RGB888_TRUE_COLOR) \
					? (frameBuffer_32bpp_convert_to_16bpp(color & ~RGB888_TRUE_COLOR)) \
					: (gRgb256Info[color % PALETEE_TYPE_NUM].rgb565);
			break;
		case 24:
		case 32:
		default:
			pDoubleWrod = (uint32 *)pixelBase;
			*pDoubleWrod = (color & RGB888_TRUE_COLOR) \
						? (color & ~RGB888_TRUE_COLOR) \
						: (gRgb256Info[color % PALETEE_TYPE_NUM].rgb888);
			break;
	}
}

/* 全屏指定颜色 */
void frameBuffer_fullScreen(paletee_256_type_t color)
{
	uint32 pixelBase;
	int x, y;

	pixelBase = gFb_LcdPara.fb_base;
	for (y = 0; y < gFb_LcdPara.y_res; y++)
	{
		for (x = 0; x < gFb_LcdPara.x_res; x++)
		{
			frameBuffer_set_point(x, y, color);
			switch (gFb_LcdPara.bpp)
			{
				case 8:
					pixelBase += 1;
					break;
				case 16:
					pixelBase += 2;
					break;
				case 24:
				case 32:
				default:
					pixelBase += 4;
					break;
			}
		}
	}
}

/* 清屏 */
void frameBuffer_clear(void)
{
	/* 清屏，全黑 */
	frameBuffer_fullScreen(paletee_256_type_Black_SYSTEM);
}


/* 初始化frameBuffer像素、分辨率参数 */
int frameBuffer_init(void)
{
	lcd_common_para_t lcdPara;

	/* 获取lcd参数：fb_base、xres、yres、bpp等 */
	(void)lcd_common_para_get(&lcdPara);
	gFb_LcdPara.fb_base = lcdPara._fb_base;
	gFb_LcdPara.x_res = lcdPara._x_res;
	gFb_LcdPara.y_res = lcdPara._y_res;
	gFb_LcdPara.bpp = frameBuffer_bpp_convert(lcdPara._bpp);
	frameBuffer_clear();

}

