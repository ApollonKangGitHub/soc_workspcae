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

int nor_flash_init(void)
{
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "start init nor flash!");

	/* DW0 Indicate data bus width for bank 0 (read only). */
	/* BWSCONr not use and can't set */
	if(0x01 == (BWSCONr >> 1) & 0x03){
		print_screen("\n\rNor flash data width is 16bits!");
	}
	else if(0x02 == (BWSCONr >> 1) & 0x03){
		print_screen("\n\rNor flash data width is 32bits!");
	}
	else{
		print_screen("\n\rNor flash data width is unavail!");
	}
	/* 设置HCLK=100MHZ时的最小TACC以快速访问 */
	nor_flash_start_squence_set(MEMCTRL_BANKCON_TACC_CLOCK_8);

	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "start init nor succeed!");
}

/* 设置Nor Flash 的时钟频率/周期，基于HCLK（高速总线时钟） */
int nor_flash_start_squence_set(uint32 choose)
{
	SOC_DEBUG_PRINT(SOC_DBG_NORMAL, "choose is %d", choose);
	switch(choose)
	{
		case MEMCTRL_BANKCON_TACC_CLOCK_1:	
		case MEMCTRL_BANKCON_TACC_CLOCK_2:
		case MEMCTRL_BANKCON_TACC_CLOCK_3:
		case MEMCTRL_BANKCON_TACC_CLOCK_4:
		case MEMCTRL_BANKCON_TACC_CLOCK_6:
		case MEMCTRL_BANKCON_TACC_CLOCK_8:
		case MEMCTRL_BANKCON_TACC_CLOCK_10:
		case MEMCTRL_BANKCON_TACC_CLOCK_14:
			/*
			 * 修改NOR flash的访问周期TACC时序，指定对象为BANKCON0r寄存器的相应位(0~7)
   		     * 以增强访问效率，但是必须大于对应Nor flash需要的最小TACC
			 */
			BANK_CONFN_TACC_SET(BANKCON0r, choose);
			break;
		default:
			BANK_CONFN_TACC_SET(BANKCON0r, MEMCTRL_BANKCON_TACC_CLOCK_14);
			break;
	}
	return 0;
}

/* 设置sdram的访问初始化设置，choose为选择的sdram枚举变量 */
int sdram_init(SDRAM_CTRL_t choose)
{
	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "start init sdram!");

	/* 
     * SDRAM refresh control register
	 */
	REFRESHr = 0x8404f5;
	/* 
	 * Flexible bank size register
	 * bit7 = 1 = Enable burst operation.
	 * bit5 = 1 = SDRAM power down mode enable
	 * bit4 = 1 = = SCLK is active only during the access
     * BANK6/7 memory map, bits[2:0] = 001 = 64MB/64MB
	 */
	BANKSIZEr = 0xb1;

	switch(choose){
		case SOC_MEMCTRL_BANK6_SDRAM_0:
			sdram0_init();
			break;
		case SOC_MEMCTRL_BANK7_SDRAN_1:
			sdram1_init();
			break;
		case SOC_MEMCTRL_BANK_SDRAM_ALL:
		deafult:
			sdram0_init();
			sdram1_init();
			break;
	}

	SYS_DEBUG_PRINT(SOC_DBG_NORMAL, "init sdram succeed!");
}

int sdram0_init()
{
	/*
 	 * set BWSCONr for BANK6(sdram0) Determines data bus width for bank 6.
 	 * DW6 bit[25:24] = 0b'10 ==> 32bit width
	 */
	BWSCONr |= 0x02000000;
	/* 
	 * set BANKCON6r for sdram0
	 * MT[16:15] = 0b'11 choose SDRAM
	 * TRCD[3:2] = 0b'00 choose RAS to CAS delay 2 clocks
	 * SCAN[1:0] = 0b'01 choose Column address number 9
	 */
	BANKCON6r = 0x18001;
	/*
	 * MRSR register must not be reconfigured while the code is running on SDRAM.
     * CAS latency = bits[6:4] = 0b'010 ==> 2clocks
     * other set by default are zero
	 */
	MRSRB6r = 0x20;
}

int sdram1_init()
{
	/*
 	 * set BWSCONr for BANK7(sdram1) Determines data bus width for bank 7.
 	 * DW6 bit[29:28] = 0b'10 ==> 32bit width
	 */
	BWSCONr |= 0x20000000;
	/* 
	 * set BANKCON7r for sdram1
	 * MT[16:15] = 0b'11 choose SDRAM
	 * TRCD[3:2] = 0b'00 choose RAS to CAS delay 2 clocks
	 * SCAN[1:0] = 0b'01 choose Column address number 9
	 */
	BANKCON7r = 0x18001;
	/*
	 * MRSR register must not be reconfigured while the code is running on SDRAM.
     * CAS latency = bits[6:4] = 0b'010 ==> 2clocks
     * other set by default are zero
	 */
	MRSRB7r = 0x20;
}

