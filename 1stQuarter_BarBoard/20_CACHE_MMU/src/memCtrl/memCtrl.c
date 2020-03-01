/*
 * FileName：memCtrl.c
 * Description:SDRAM内存操作
 * Time：2019年4月14日17:08:26
 * Author：Kangruojin
 * Version：V1.0
*/
#include <soc_s3c2440_memCtrl_field.h>
#include <soc_s3c2440_reg.h>
#include <memCtrl.h>

/* 设置sdram的访问初始化设置，choose为选择的sdram枚举变量 */
int sdram_init(SDRAM_CTRL_t choose)
{
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

