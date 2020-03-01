#ifndef _MEM_CTRL_H_
#define _MEM_CTRL_H_

#include <soc_s3c2440_memCtrl_field.h>
#include <tools.h>

#define BWSCON_REG_FIELD_SET(reg, field, mask, set){\
	reg &= ~mask;\
	reg |= ((value << field) & mask);\
}

/*
 * 注意：对于BANKCON该寄存器来说,当前设置的控制寄存器，是当前运行设备的控制寄存器时
 * 由于值设置的小于指定clock周期，设备可能不能正确运行
 * 导致下一条指令无法读取，所以不能先清空再设置
*/
#define BANK_CONFN_REG_FIELD_SET(reg, field, mask, value){\
	reg = (value << field) & mask;\
}

/* only for BANK6~BANK7 */
#define BANK_CONFN_MT_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_MT_START, MEMCTRL_BANKCON_MT_BITSf, value)
/* 
 * use for all BANK(0~7)
 * Memory Type = ROM or SRAM [MT=00] (15-bit),only use @ ROM or SRAM when is BANK6~BANK7 
 */
#define BANK_CONFN_TACS_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_TACS_START, MEMCTRL_BANKCON_TACS_BITSf, value)
#define BANK_CONFN_TCOS_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_TCOS_START, MEMCTRL_BANKCON_TCOS_BITSf, value)
#define BANK_CONFN_TACC_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_TACC_START, MEMCTRL_BANKCON_TACC_BITSf, value)
#define BANK_CONFN_TCOH_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_TCOH_START, MEMCTRL_BANKCON_TCOH_BITSf, value)
#define BANK_CONFN_TCAH_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_TCAH_START, MEMCTRL_BANKCON_TCAH_BITSf, value)
#define BANK_CONFN_TACP_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_TACP_START, MEMCTRL_BANKCON_TACP_BITSf, value)
#define BANK_CONFN_PMC_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_PMC_START, MEMCTRL_BANKCON_PMC_BITSf, value)
/* Memory Type = SDRAM [MT=11] (4-bit),only use @ SDRAM when is BANK6~BANK7 */
#define BANK_CONFN_TRCD_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_TRCD_START, MEMCTRL_BANKCON_TRCD_BITSf, value)
#define BANK_CONFN_SCAN_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_SCAN_START, MEMCTRL_BANKCON_SCAN_BITSf, value)


typedef enum{
	SOC_MEMCTRL_BANK6_SDRAM_0,
	SOC_MEMCTRL_BANK7_SDRAN_1,
	SOC_MEMCTRL_BANK_SDRAM_ALL
}SDRAM_CTRL_t;
extern int sdram_init(SDRAM_CTRL_t choose);

#endif
