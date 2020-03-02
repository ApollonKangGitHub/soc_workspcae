#ifndef _SOC_S3C2440_MEMCTRL_FIELD_H_
#define _SOC_S3C2440_MEMCTRL_FIELD_H_

/* ################################### BWSCON register ################################### */
/* BWSCON register field */
#define MEMCTRL_BWSCON_ST7_BITSf		(0x1 << 31)
#define MEMCTRL_BWSCON_WS7_BITSf		(0x1 << 30)
#define MEMCTRL_BWSCON_DW7_BITSf		(0x3 << 28)
#define MEMCTRL_BWSCON_ST6_BITSf		(0x1 << 27)
#define MEMCTRL_BWSCON_WS6_BITSf		(0x1 << 26)
#define MEMCTRL_BWSCON_DW6_BITSf		(0x3 << 24)
#define MEMCTRL_BWSCON_ST5_BITSf		(0x1 << 23)
#define MEMCTRL_BWSCON_WS5_BITSf		(0x1 << 22)
#define MEMCTRL_BWSCON_DW5_BITSf		(0x3 << 20)
#define MEMCTRL_BWSCON_ST4_BITSf		(0x1 << 19)
#define MEMCTRL_BWSCON_WS4_BITSf		(0x1 << 18)
#define MEMCTRL_BWSCON_DW4_BITSf		(0x3 << 16)
#define MEMCTRL_BWSCON_ST3_BITSf		(0x1 << 15)
#define MEMCTRL_BWSCON_WS3_BITSf		(0x1 << 14)
#define MEMCTRL_BWSCON_DW3_BITSf		(0x3 << 12)
#define MEMCTRL_BWSCON_ST2_BITSf		(0x1 << 11)
#define MEMCTRL_BWSCON_WS2_BITSf		(0x1 << 10)
#define MEMCTRL_BWSCON_DW2_BITSf		(0x3 << 8)
#define MEMCTRL_BWSCON_ST1_BITSf		(0x1 << 7)
#define MEMCTRL_BWSCON_WS1_BITSf		(0x1 << 6)
#define MEMCTRL_BWSCON_DW1_BITSf		(0x3 << 4)
#define MEMCTRL_BWSCON_DW0_BITSf		(0x3 << 1)
/* BIT 3 is not used and BIT0 is Reserved */
#define MEMCTRL_BWSCON_RESERVED_BITSf	(0x01 << 0)

/* every fields start bit */
#define MEMCTRL_BWSCON_ST7_START		(31)
#define MEMCTRL_BWSCON_WS7_START		(30)
#define MEMCTRL_BWSCON_DW7_START		(28)
#define MEMCTRL_BWSCON_ST6_START		(27)
#define MEMCTRL_BWSCON_WS6_START		(26)
#define MEMCTRL_BWSCON_DW6_START		(24)
#define MEMCTRL_BWSCON_ST5_START		(23)
#define MEMCTRL_BWSCON_WS5_START		(22)
#define MEMCTRL_BWSCON_DW5_START		(20)
#define MEMCTRL_BWSCON_ST4_START		(19)
#define MEMCTRL_BWSCON_WS4_START		(18)
#define MEMCTRL_BWSCON_DW4_START		(16)
#define MEMCTRL_BWSCON_ST3_START		(15)
#define MEMCTRL_BWSCON_WS3_START		(14)
#define MEMCTRL_BWSCON_DW3_START		(12)
#define MEMCTRL_BWSCON_ST2_START		(11)
#define MEMCTRL_BWSCON_WS2_START		(10)
#define MEMCTRL_BWSCON_DW2_START		(8)
#define MEMCTRL_BWSCON_ST1_START		(7)
#define MEMCTRL_BWSCON_WS1_START		(6)
#define MEMCTRL_BWSCON_DW1_START		(4)
#define MEMCTRL_BWSCON_DW0_START		(1)
#define MEMCTRL_BWSCON_RESERVED_START	(0)

/* Determines SRAM for using UB/LB for bank_n */
#define MEMCTRL_BWSCON_STn_NO			(0x0)
#define MEMCTRL_BWSCON_STn_YES			(0x1)

/* Determines WAIT status for bank_n */
#define MEMCTRL_BWSCON_WSn_DIS			(0x0)
#define MEMCTRL_BWSCON_WSn_EN			(0x1)

/* Indicate data bus width for bank_n(read only). */
#define MEMCTRL_BWSCON_DWn_8			(0x0)	/* 8-bit width */
#define MEMCTRL_BWSCON_DWn_16			(0x1)	/* 16-bit width */
#define MEMCTRL_BWSCON_DWn_32			(0x2)	/* 32-bit width */
#define MEMCTRL_BWSCON_DWn_r			(0x3)	

/* ################################### BANK config register ################################### */
/* BANKCONn reigster field */
/* Determine the memory type for bank6 or bank7. */
#define MEMCTRL_BANKCON_MT_BITSf		(0x3 << 15)
/* 
 * use for all BANK(0~7)
 * Memory Type = ROM or SRAM [MT=00] (15-bit),only use @ ROM or SRAM
 */
#define MEMCTRL_BANKCON_TACS_BITSf	(0x3<<13)
#define MEMCTRL_BANKCON_TCOS_BITSf	(0x3<<11)
#define MEMCTRL_BANKCON_TACC_BITSf	(0x7<<8)
#define MEMCTRL_BANKCON_TCOH_BITSf	(0x3<<6)
#define MEMCTRL_BANKCON_TCAH_BITSf	(0x3<<4)
#define MEMCTRL_BANKCON_TACP_BITSf	(0x3<<2)
#define MEMCTRL_BANKCON_PMC_BITSf	(0x3<<0)
/* Memory Type = SDRAM [MT=11] (4-bit),only use @ SDRAM */
#define MEMCTRL_BANKCON_TRCD_BITSf	(0x3 << 2)
#define MEMCTRL_BANKCON_SCAN_BITSf	(0x3 << 0)

/* 
 * use for all BANK(0~7)
 */
#define MEMCTRL_BANKCON_MT_START	(15)
#define MEMCTRL_BANKCON_TACS_START	(13)
#define MEMCTRL_BANKCON_TCOS_START	(11)
#define MEMCTRL_BANKCON_TACC_START	(8)
#define MEMCTRL_BANKCON_TCOH_START	(6)
#define MEMCTRL_BANKCON_TCAH_START	(4)
#define MEMCTRL_BANKCON_TACP_START	(2)
#define MEMCTRL_BANKCON_PMC_START	(0)
#define MEMCTRL_BANKCON_TRCD_START	(2)
#define MEMCTRL_BANKCON_SCAN_START	(0)
/* Address set-up time before nGCSn */
#define MEMCTRL_BANKCON_TACS_CLOCK_0	(0x0)
#define MEMCTRL_BANKCON_TACS_CLOCK_1	(0x1)
#define MEMCTRL_BANKCON_TACS_CLOCK_2	(0x2)
#define MEMCTRL_BANKCON_TACS_CLOCK_4	(0x3)
/* Chip selection set-up time before nOE */
#define MEMCTRL_BANKCON_TCOS_CLOCK_0	(0x0)
#define MEMCTRL_BANKCON_TCOS_CLOCK_1	(0x1)
#define MEMCTRL_BANKCON_TCOS_CLOCK_2	(0x2)
#define MEMCTRL_BANKCON_TCOS_CLOCK_4	(0x3)
/* Access cycle,default is 0b'111 */
#define MEMCTRL_BANKCON_TACC_CLOCK_1	(0x0)
#define MEMCTRL_BANKCON_TACC_CLOCK_2	(0x1)
#define MEMCTRL_BANKCON_TACC_CLOCK_3	(0x2)
#define MEMCTRL_BANKCON_TACC_CLOCK_4	(0x3)
#define MEMCTRL_BANKCON_TACC_CLOCK_6	(0x4)
#define MEMCTRL_BANKCON_TACC_CLOCK_8	(0x5)
#define MEMCTRL_BANKCON_TACC_CLOCK_10	(0x6)
#define MEMCTRL_BANKCON_TACC_CLOCK_14	(0x7)
/* Chip selection hold time after nOE */
#define MEMCTRL_BANKCON_TCOH_CLOCK_0	(0x0)
#define MEMCTRL_BANKCON_TCOH_CLOCK_1	(0x1)
#define MEMCTRL_BANKCON_TCOH_CLOCK_2	(0x2)
#define MEMCTRL_BANKCON_TCOH_CLOCK_4	(0x3)
/* Address hold time after nGCSn */
#define MEMCTRL_BANKCON_TCAH_CLOCK_0	(0x0)
#define MEMCTRL_BANKCON_TCAH_CLOCK_1	(0x1)
#define MEMCTRL_BANKCON_TCAH_CLOCK_2	(0x2)
#define MEMCTRL_BANKCON_TCAH_CLOCK_4	(0x3)
/* Page mode access cycle @ Page mode */
#define MEMCTRL_BANKCON_TACP_CLOCK_2	(0x0)
#define MEMCTRL_BANKCON_TACP_CLOCK_3	(0x1)
#define MEMCTRL_BANKCON_TACP_CLOCK_4	(0x2)
#define MEMCTRL_BANKCON_TACP_CLOCK_6	(0x3)
/* Page mode configuration */
#define MEMCTRL_BANKCON_PMC_CLOCK_1		(0x0)
#define MEMCTRL_BANKCON_PMC_CLOCK_4		(0x1)
#define MEMCTRL_BANKCON_PMC_CLOCK_8		(0x2)
#define MEMCTRL_BANKCON_PMC_CLOCK_16	(0x3)

/* 
 *only use for BANK6~BANK7
 */
#define MEMCTRL_BANKCON_MT_ROM_OR_SRAM		(0x0)
/* 0b'01 and 0b'10 are reserved do not use*/
#define MEMCTRL_BANKCON_MT_SYNC_DRAM		(0x3)
/* RAS to CAS delay,default is 0x2, 0x3 is reserved */
#define MEMCTRL_BANKCON_SDRAM_TRCD_2		(0x0)
#define MEMCTRL_BANKCON_SDRAM_TRCD_3		(0x1)
#define MEMCTRL_BANKCON_SDRAM_TRCD_4		(0x2)
#define MEMCTRL_BANKCON_SDRAM_TRCD_r		(0x3)
/* Column address number,default is 0x0, 0x3 is reserved */
#define MEMCTRL_BANKCON_SDRAM_SCAN_8		(0x0)
#define MEMCTRL_BANKCON_SDRAM_SCAN_9		(0x1)
#define MEMCTRL_BANKCON_SDRAM_SCAN_10		(0x2)
#define MEMCTRL_BANKCON_SDRAM_SCAN_r		(0x3)

#endif
