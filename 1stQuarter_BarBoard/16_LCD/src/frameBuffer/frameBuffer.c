#include <frameBuffer.h>
#include <lcd_common.h>
#include <tools.h>

typedef struct {
	uint32 x_res;
	uint32 y_res;
	uint32 bpp;
	uint32 fb_base;
}fb_lcd_para;

static fb_lcd_para gFb_LcdPara = {
	.x_res = 0,
	.y_res = 0,
	.bpp = 0,
	.fb_base = 0
};

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


/* 设置某一点的像素颜色 */
int frameBuffer_set_point
(
	uint32 x, 
	uint32 y, 
	uint32 color
)
{
	uint32 pixelBase = 0;
	uint8 * pByte = NULL;
	uint16 * pWord = NULL;
	uint32 * pDoubleWrod = NULL;
	
	if ((x < 0) || (x > gFb_LcdPara.x_res) 
		|| (y < 0) || (y > gFb_LcdPara.y_res))
	{
		return ERROR;
	}

	pixelBase = gFb_LcdPara.fb_base + (((gFb_LcdPara.x_res * y) + x) * gFb_LcdPara.bpp / 8); 
	switch (gFb_LcdPara.bpp)
	{
		case 8:
			pByte = (uint8 *)pixelBase;
			*pByte = color;
			break;
		case 16:
			pWord = (uint16 *)pixelBase;
			*pWord = frameBuffer_32bpp_convert_to_16bpp(color);
			break;
		case 32:
		default:
			pDoubleWrod = (uint32 *)pixelBase;
			*pDoubleWrod = color;
			break;
	}
	return OK;	
}
