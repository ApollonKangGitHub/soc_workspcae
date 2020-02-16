#include <lcd_controller_common.h>
#include <lcd_controller_s3c2440.h>
#include <soc_s3c2440_public.h>

#define HCLK (100)	/* 100MHZ */

static void lcd_jz2440_pin_init(void)
{
	/* 初始化引脚:背光引脚 */
	GPBCONr &= ~(GPBCON_GPB0_MASK << GPBCON_GPB0_START);
	GPBCONr |= ((GBPCON_GPB_OUTPUT & GPBCON_GPB0_MASK) << GPBCON_GPB0_START);

	/* 初始化引脚:LCD专用引脚 */
	GPCCONr = GPCCON_VEDIO_DATA_ENABLE;
	GPDCONr = GPCCON_VEDIO_DATA_ENABLE;

	/* Power Enable */
	GPGCONr |= (GPGCON_LCD_PWRDN << GPGCON_GPG4_CONF_START);
	
}

/* lcd控制器的初始化 */
void lcd_controller_init_s3c2440(lcd_parameters_t * p_lcd_para)
{
	uint32 clkval = 0;
	uint32 pnrmode = 0;
	uint32 bppmod = 0;
	uint32 vbpd = 0;
	uint32 lineval = 0;
	uint32 vfpd = 0;
	uint32 vspw = 0;
	uint32 hbpd = 0;
	uint32 hozval = 0;
	uint32 hfpd = 0;
	uint32 hspw = 0;
	uint32 vclk_polarity = 0;
	uint32 rgb_polarity = 0;
	uint32 hsync_polarity = 0;
	uint32 vsync_polarity = 0;
	uint32 dataEn_polarity = 0;
	uint32 pwrEn_polarity = 0;
	uint32 pixel_plane = 0;
	uint32 fb_base = 0;
	uint32 fb_end = 0;

	lcd_jz2440_pin_init();
	
	/* 
	 * 1、设置VCLK range(5~12)
 	 * VCLK = HCLK / [(CLKVAL+1) x 2] ( CLKVAL ≥ 0 )
 	 *    	  100M / ((CLKVAL+1) x 2)
 	 * 假设CLKVAL = 5, 则VCLK=8.3MHZ, 在范围内
 	 * 2、设置模式为TFT
 	 * 3、设置BPP
 	 */
 	clkval  = (uint32)((double)HCLK / p_lcd_para->vclk / 2 - 1 + 0.5);
	pnrmode = LCDCON1_PNR_MODE_TFT_LCD_PANEL;
	bppmod  = (p_lcd_para->pixel._bpp == bpp_type_8bits) ? _LCDCON1_TFT_BPP_8_ \
			: (p_lcd_para->pixel._bpp == bpp_type_16bits) ? _LCDCON1_TFT_BPP_16_ \
			: (p_lcd_para->pixel._bpp == bpp_type_24bits) ? _LCDCON1_TFT_BPP_24_ \
			: _LCDCON1_TFT_BPP_16_;
	LCDCON1r = (clkval << LCDCON1_CLKVAL_START) \
			 | (pnrmode << LCDCON1_PNR_MODE_START) \
		 	 | (bppmod << LCDCON1_BPP_MOD_START);
	
	/* 4、垂直方向时序配置 */
	vbpd = p_lcd_para->time_seq._tvb - 1;
	lineval = p_lcd_para->pixel._y_res - 1;
	vfpd = p_lcd_para->time_seq._tvf - 1;
	vspw = p_lcd_para->time_seq._tvp - 1;
	LCDCON2r = (vbpd << LCDCON2_VBPD_START) \
			 | (lineval << LCDCON2_LINEVAL_START) \
			 | (vfpd << LCDCON2_VFPD_START) \
			 | (vspw << LCDCON2_VSPW_START);

	/* 5、水平方向时序配置 */
	hbpd = p_lcd_para->time_seq._thb - 1;
	hozval = p_lcd_para->pixel._y_res - 1;
	hfpd = p_lcd_para->time_seq._thf - 1;
	hspw = p_lcd_para->time_seq._thp - 1;
	LCDCON3r = (hbpd << LCDCON3_HBPD_START) \
			 | (hozval << LCDCON3_HOZVAL_START) \
			 | (hfpd << LCDCON3_HFPD_START); 
	LCDCON4r = (vspw << LCDCON4_HSPW_START);

	/* 6、引脚极性 / RGB BBP 像素格式等 */
	vclk_polarity = (POLARITY_TYPE_NORMAL == p_lcd_para->pins_pol._vclk) ? LCDCON5_NORMAL: LCDCON5_INVERTED;
	rgb_polarity = (POLARITY_TYPE_NORMAL == p_lcd_para->pins_pol._rgb) ? LCDCON5_NORMAL: LCDCON5_INVERTED;
	hsync_polarity = (POLARITY_TYPE_NORMAL == p_lcd_para->pins_pol._hsync) ? LCDCON5_NORMAL: LCDCON5_INVERTED;
	vsync_polarity = (POLARITY_TYPE_NORMAL == p_lcd_para->pins_pol._vsync) ? LCDCON5_NORMAL: LCDCON5_INVERTED;
	dataEn_polarity = (POLARITY_TYPE_NORMAL == p_lcd_para->pins_pol._dataEn) ? LCDCON5_NORMAL: LCDCON5_INVERTED;
	pwrEn_polarity = (POLARITY_TYPE_NORMAL == p_lcd_para->pins_pol._pwrEn) ? LCDCON5_NORMAL: LCDCON5_INVERTED;
	
	if (p_lcd_para->pixel._bpp == bpp_type_24bits) {
		pixel_plane = (LCDCON5_SWAP_DISABLE << LCDCON5_BSWP_START) \
					| (LCDCON5_SWAP_DISABLE << LCDCON5_HWSWP_START) \
					| (LCDCON5_SWAP_DISABLE << LCDCON5_BPP24BL_START);
	}
	else if (p_lcd_para->pixel._bpp == bpp_type_16bits) {
		pixel_plane = (LCDCON5_SWAP_DISABLE << LCDCON5_BSWP_START) \
					| (LCDCON5_SWAP_ENABLE << LCDCON5_HWSWP_START);
	}
	else if (p_lcd_para->pixel._bpp == bpp_type_8bits) {
		pixel_plane = (LCDCON5_SWAP_ENABLE << LCDCON5_BSWP_START) \
					| (LCDCON5_SWAP_DISABLE << LCDCON5_HWSWP_START);
	}
					
	LCDCON5r = (vclk_polarity << LCDCON5_INVVCLK_START) \
			 | (rgb_polarity << LCDCON5_INVVD_START) \
			 | (hsync_polarity << LCDCON5_INVVLINE_START) \
			 | (vsync_polarity << LCDCON5_INVVFRAME_START) \
			 | (dataEn_polarity << LCDCON5_INVVD_START) \
			 | (pwrEn_polarity << LCDCON5_PWREN_START) \
			 | (LCDCON5_FORMAT_16_BBP_565 << LCDCON5_FRM565_START)
			 | (pixel_plane);


	/* 7、Frame Buffer地址
	 * 	LCDBANK [29:21] :A[30:22] for framebuffer base.
 	 * 	LCDBASEU [20:0]	:A[21:1] for framebuffer base.
	 */
	fb_base = (p_lcd_para->fb_base & 0x7FFFFFFF) >> 1;	/* A[30~1] */
	LCDSADDR1r = fb_base & 0x3FFFFFFF;					/* [29:0] = A[30~1] */

	/* LCDBASEL [20:0]	:A[21:1] for framebuffer end address */
	fb_end = p_lcd_para->fb_base + (p_lcd_para->pixel._x_res * p_lcd_para->pixel._y_res * p_lcd_para->pixel._bpp / 8);
	fb_end >>= 1;
	fb_end &= 0x1FFFFF;
	LCDSADDR2r = fb_end;
}

/* enble lcd */
void lcd_controller_enable_s3c2440(BOOL enable)
{
	/*
	 * 分为以下几部分：
	 * 1、Power enable:	AVDD 供电使能
	 * 2、背光LED引脚使能
	 * 3、LCD控制器输出信号使能
	 */
	 if (enable) {
		GPBDATr |= (GPBDATA_GPB_INPUT_ENABLE << GPIOCON_PIN0_START_BIT);
		LCDCON5r |= (LCDCON5_PWRED_ENABLE << LCDCON5_PWREN_START);
	 	LCDCON1r |= (LCDCON1_ENVID_ENABLE << LCDCON1_ENVID_START);
	 }
	 else {
		GPBDATr &= ~(GPBDATA_GPB_INPUT_ENABLE << GPIOCON_PIN0_START_BIT);
		LCDCON5r &= ~(LCDCON5_PWRED_ENABLE << LCDCON5_PWREN_START);
	 	LCDCON1r &= ~(LCDCON1_ENVID_ENABLE << LCDCON1_ENVID_START);
	 }
}

/* 驱动对象操作结构 */
lcd_controller_drv_t lcd_controller_s3c2440 = {
	.lcd_controller_drv_name 	= "SOC S3C2440",
	.lcd_init 					= lcd_controller_init_s3c2440,
	.lcd_enable 				= lcd_controller_enable_s3c2440
};


