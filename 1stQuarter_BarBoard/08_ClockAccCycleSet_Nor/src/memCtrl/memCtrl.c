/*
 * FileName：memCtrl.c
 * Description:用于设置内存控制器的CS片选与相应片选设备的属性，比如差异化的时钟频率
 * Time：2019年4月14日17:08:26
 * Author：Kangruojin
 * Version：V1.0
*/
#include <soc_s3c2440_memCtrl_field.h>
#include <soc_s3c2440_reg.h>
#include <memCtrl.h>
#include <tools.h>
#include <log.h>

/* 设置Nor Flash 的时钟频率/周期，基于HCLK（高速总线时钟） */
int nor_flash_start_squence_set(uint32 choose)
{
	SOC_DEBUG_PRINT(SOC_DBG_NORMAL, "choose is %d\r\n", choose);
	switch(choose)
	{
		case MEMCTRL_BANKCON_0_5_TACC_CLOCK_1:	
		case MEMCTRL_BANKCON_0_5_TACC_CLOCK_2:
		case MEMCTRL_BANKCON_0_5_TACC_CLOCK_3:
		case MEMCTRL_BANKCON_0_5_TACC_CLOCK_4:
		case MEMCTRL_BANKCON_0_5_TACC_CLOCK_6:
		case MEMCTRL_BANKCON_0_5_TACC_CLOCK_8:
		case MEMCTRL_BANKCON_0_5_TACC_CLOCK_10:
		case MEMCTRL_BANKCON_0_5_TACC_CLOCK_14:
			/* 修改NOR flash的访问周期时序，指定对象为BANKCON0r寄存器的相应位(0~7) */
			BANK_CONFN_TACC_SET(BANKCON0r, choose);
			break;
		default:
			return -1;
	}
	return 0;
}