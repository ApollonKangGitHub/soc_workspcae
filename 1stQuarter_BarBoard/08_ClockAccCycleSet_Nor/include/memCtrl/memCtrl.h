#ifndef _MEM_CTRL_H_
#define _MEM_CTRL_H_

#include <soc_s3c2440_memCtrl_field.h>
#include <tools.h>

/*
	reg &= ~mask; \
	reg |= ((value << field) & mask); \
*/
#define BANK_CONFN_REG_FIELD_SET(reg, field, mask, value){\
	reg = (value << field) & mask;\
}

#define BANK_CONFN_TACS_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_0_5_TACS_START, MEMCTRL_BANKCON_0_5_TACS_BITSf, value)
#define BANK_CONFN_TCOS_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_0_5_TCOS_START, MEMCTRL_BANKCON_0_5_TCOS_BITSf, value)
#define BANK_CONFN_TACC_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_0_5_TACC_START, MEMCTRL_BANKCON_0_5_TACC_BITSf, value)
#define BANK_CONFN_TCOH_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_0_5_TCOH_START, MEMCTRL_BANKCON_0_5_TCOH_BITSf, value)
#define BANK_CONFN_TCAH_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_0_5_TCAH_START, MEMCTRL_BANKCON_0_5_TCAH_BITSf, value)
#define BANK_CONFN_TACP_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_0_5_TACP_START, MEMCTRL_BANKCON_0_5_TACP_BITSf, value)
#define BANK_CONFN_PMC_SET(reg, value)\
	BANK_CONFN_REG_FIELD_SET(reg, MEMCTRL_BANKCON_0_5_PMC_START, MEMCTRL_BANKCON_0_5_PMC_BITSf, value)

int nor_flash_start_squence_set(uint32 choose);

#endif
