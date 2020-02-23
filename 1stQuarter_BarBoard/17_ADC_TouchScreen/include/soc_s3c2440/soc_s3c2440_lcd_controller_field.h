#ifndef _SCO_S3C2440_LCD_CONTROLLER_FIELD_H_
#define _SCO_S3C2440_LCD_CONTROLLER_FIELD_H_

/************************************************ LCDCON1 *************************************************/

/*
 * [17:8] Determine the rates of VCLK and CLKVAL[9:0]. 
 * STN: VCLK = HCLK / (CLKVAL x 2) ( CLKVAL ≥2 ) 
 * TFT: VCLK = HCLK / [(CLKVAL+1) x 2] ( CLKVAL ≥ 0 )
 */
#define LCDCON1_CLKVAL_START	(8)
/*
 * [6:5] Select the display mode.
 * 00 = 4-bit dual scan display mode (STN)
 * 01 = 4-bit single scan display mode (STN) 
 * 10 = 8-bit single scan display mode (STN) 
 * 11 = TFT LCD panel
 */
#define LCDCON1_PNR_MODE_START				(5)
#define LCDCON1_PNR_MODE_STN_4BIT_DUAL		(0x0)
#define LCDCON1_PNR_MODE_STN_4BIT_SINGLE	(0x1)
#define LCDCON1_PNR_MODE_STN_8BIT_SINGLE	(0x2)
#define LCDCON1_PNR_MODE_TFT_LCD_PANEL		(0x3)

/*
 * [4:1] Select the BPP (Bits Per Pixel) mode.
 * 1000 = 1 bpp for TFT 
 * 1001 = 2 bpp for TFT 
 * 1010 = 4 bpp for TFT 
 * 1011 = 8 bpp for TFT 
 * 1100 = 16 bpp for TFT 
 * 1101 = 24 bpp for TFT
 */
#define LCDCON1_BPP_MOD_START		(1)
#define _LCDCON1_TFT_BPP_1_			(0x8)
#define _LCDCON1_TFT_BPP_2_			(0x9)
#define _LCDCON1_TFT_BPP_4_			(0xA)
#define _LCDCON1_TFT_BPP_8_			(0xB)
#define _LCDCON1_TFT_BPP_16_		(0xC)
#define _LCDCON1_TFT_BPP_24_		(0xD)

/* [0] LCD video output and the logic enable/disable */
#define LCDCON1_ENVID_START		(0)
#define LCDCON1_ENVID_MASK		(0x1)
#define LCDCON1_ENVID_ENABLE	(1)
#define LCDCON1_ENVID_DISABLE	(0)

/************************************************ LCDCON2 *************************************************/

/*
 * VBPD [31:24] 
 * 		TFT: 	Vertical back porch is the number of inactive lines at the start of a frame, 
 * 		 		after vertical synchronization period. 
 * 		STN: 	These bits should be set to zero on STN LCD. 
 * 
 * LINEVAL [23:14] 
 * 		TFT/STN: 
 				These bits determine the vertical size of LCD panel.  
 * VFPD [13:6] 
 * 		TFT: 	Vertical front porch is the number of inactive lines at the end of a frame, 
 * 	     		before vertical synchronization period. 
 * 		STN: 	These bits should be set to zero on STN LCD.  
 * VSPW [5:0]
 * 		TFT: 	Vertical sync pulse width determines the VSYNC pulse's high level 
 * 	     		width by counting the number of inactive lines.
 * 		STN: 	These bits should be set to zero on STN LCD. 
 */

#define LCDCON2_VBPD_START		(24)
#define LCDCON2_LINEVAL_START	(14)
#define LCDCON2_VFPD_START		(6)
#define LCDCON2_VSPW_START		(0)

/************************************************ LCDCON3 *************************************************/
/*
 * HBPD (TFT) [25:19]
 *		TFT:	
 *		 	Horizontal back porch is the number of VCLK periods between 
 *			the falling edge of HSYNC and the start of active data.
 * HOZVAL [18:8] 
 *		TFT/STN:
 *			These bits determine the horizontal size of LCD panel.
 * HFPD (TFT) [7:0]
 *		TFT: 	
 *			Horizontal front porch is the number of VCLK periods between 
 * 			the end of active data and the rising edge of HSYNC.
 */
#define LCDCON3_HBPD_START		(19)
#define LCDCON3_HOZVAL_START	(8)
#define LCDCON3_HFPD_START		(0)

/************************************************ LCDCON4 *************************************************/
/*
 * HSPW(TFT) [7:0] 
 * 		TFT:
 * 			Horizontal sync pulse width determines the HSYNC pulse's
 * 			high level width by counting the number of the VCLK.
*/

#define LCDCON4_HSPW_START		(0)

/************************************************ LCDCON5 *************************************************/

/*
 * VSTATUS [16:15]
 * 		TFT: Vertical Status (read only). 
 *      00 = VSYNC, 	01 = BACK Porch,	10 = ACTIVE,	11 = FRONT Porch
 * HSTATUS [14:13]
 *		TFT: Horizontal Status (read only).
 *		00 = HSYNC, 	01 = BACK Porch, 	10 = ACTIVE, 	11 = FRONT Porch
 * BPP24BL [12]
 *		TFT: This bit determines the order of 24 bpp video memory.
 *		0 = LSB valid,	1 = MSB Valid
 * FRM565 [11]
 *		TFT: This bit selects the format of 16 bpp output video data.
 *		0 = 5:5:5:1,	1 = 5:6:5
 * INVVCLK [10]
 *		STN/TFT: This bit controls the polarity of the VCLK active edge. 
 *		0 = The video data is fetched at VCLK falling edge 
 *		1 = The video data is fetched at VCLK rising edge
 * INVVLINE [9]
 *		STN/TFT: This bit indicates the VLINE/HSYNC pulse polarity [Notes 1].
 * INVVFRAME [8]
 *		STN/TFT: This bit indicates the VFRAME/VSYNC pulse polarity [Notes 1].
 * INVVD [7]
 *		STN/TFT: This bit indicates the VD (video data) pulse polarity.
 *		0 = Normal 1 = VD is inverted.
 * NVVDEN [6]
 *		TFT: This bit indicates the VDEN signal polarity [Notes 1].
 * INVPWREN [5]
 *		STN/TFT: This bit indicates the PWREN signal polarity [Notes 1].
 * INVLEND [4]
 *		TFT: This bit indicates the LEND signal polarity. 
 * PWREN [3]
 *		STN/TFT: LCD_PWREN output signal enable/disable. 
 *		0 = Disable PWREN signal，	1 = Enable PWREN signal
 * ENLEND [2]
 *		TFT: LEND output signal enable/disable. 
 *		0 = Disable LEND signal，	1 = Enable LEND signal
 * BSWP [1]
 *		STN/TFT: Byte swap control bit. 
 *		0 = Swap Disable，	1 = Swap Enable
 * HWSWP [0]
 *		STN/TFT: Half-Word swap control bit. 
 *		0 = Swap Disable，	1 = Swap Enable	
 * Notes 1:
 *		0 = normal 1 = inverted
 */

#define LCDCON5_VSTATUS_START	(15)
#define LCDCON5_HSTATUS_START	(13)
#define LCDCON5_BPP24BL_START	(12)
#define LCDCON5_FRM565_START	(11)
#define LCDCON5_INVVCLK_START	(10)
#define LCDCON5_INVVLINE_START	(9)
#define LCDCON5_INVVFRAME_START	(8)
#define LCDCON5_INVVD_START		(7)
#define LCDCON5_NVVDEN_START	(6)
#define LCDCON5_INVPWREN_START	(5)
#define LCDCON5_INVLEND_START	(4)
#define LCDCON5_PWREN_START		(3)
#define LCDCON5_ENLEND_START	(2)
#define LCDCON5_BSWP_START		(1)
#define LCDCON5_HWSWP_START		(0)

/*
 * 00 = VSYNC / HSYNC 
 * 01 = BACK Porch
 * 10 = ACTIVE 
 * 11 = FRONT Porch
 */
#define LCDCON5_VS_HS_STATUS_SYNC			(0x0)
#define LCDCON5_VS_HS_STATUS_BACK_PRCH		(0x1)
#define LCDCON5_VS_HS_STATUS_ACTIVE			(0x2)
#define LCDCON5_VS_HS_STATUS_FRONT_PRCH		(0x3)

/* 24bit BBP格式，低位有效还是高位有效 */
#define LCDCON5_FORMAT_24_BBP_LSB_VALID		(0)
#define LCDCON5_FORMAT_24_BBP_MSB_VALID		(1)

/* 16bit BBP格式 */
#define LCDCON5_FORMAT_16_BBP_5551			(0)
#define LCDCON5_FORMAT_16_BBP_565			(1)

/* VEDIO数据获取触发方式 */
#define LCDCON5_VEDIO_VCLK_FALLING_EDGE		(0)
#define LCDCON5_VEDIO_VCLK_RISING_EDGE		(1)

/* 极性是否反转 */
#define LCDCON5_NORMAL						(0)
#define LCDCON5_INVERTED					(1)

/* 供电使能 */
#define LCDCON5_PWRED_ENABLE				(1)
#define LCDCON5_PWREN_DISABLE				(0)

/* 字节是否交换 */
#define LCDCON5_SWAP_DISABLE				(0)
#define LCDCON5_SWAP_ENABLE					(1)

/************************************************ LCDSADDR1 *************************************************/
/*
 * LCDBANK [29:21] :
 * 		A[30:22] for framebuffer.
 * LCDBASEU [20:0]
 * 		A[21:1] for framebuffer.
 */


#endif	/* _SCO_S3C2440_LCD_CONTROLLER_FIELD_H_ */
