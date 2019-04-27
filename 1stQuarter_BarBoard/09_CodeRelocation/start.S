/*
 * Time:2019年4月6日23:59:16
 * Author:Kangruojin
 */
.text
.global _start

_start:
set_watchDog_timer_disable:
	/* disable watchDog Timer reset function */
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

set_clockFor_FclkHclkPclk:
	/* 
	 * FCLK from 12MHz acorss PLL raise to 400MHz for CPU
	 * HCLK is high frequency for connected to  High-speed BUS such as LCD/Nand flash/Nor flash controller, etc.
	 * PCLK is low frequency for connect to Low-speed BUS such as I2C/GPIO/ADC/PWM, etc.
	*/
set_lockTime:
	/* set locktime to default value,LOCKTIME(0x4C000000) register */
	ldr r0, =0x4C000000
	ldr r1, =0xFFFFFFFF
	str r1, [r0]

set_HClkAndPClk:
	/* 
	 * set HCLK = FCLK/4 = 100MHz, set PCLK = HCLK/2 = 50MHz, FCLK:HCLK:PCLK=8:2:1
	 * should set CLKDIVN register(0x4C000014) for HDIVN[2:1]=0b'10,PDIVN[0]=0b'1
	 */
	ldr r0, =0x4C000014
	ldr r1, [r0]
	and r1, #0xF8		/* clear last 3 bits */
	orr r1, #0x5		/* set last 3 bits 3b'101*/
	str r1, [r0]
	/* when HDIVN is 0b'10,must set CSMDIVN(0x4C000018) bit 9 is 0, this bit default is 0 */
	ldr r0, =0x4C000018
	ldr r1, [r0]
	ldr r2, =0xFFFFFEFF
	and r1, r2
	str r1, [r0]

set_MMUAsyncBusMode:
	/*
	 * notes:If HDIVN is not 0, the CPU bus mode has to be changed from the fast bus mode
	 * to the asynchronous bus mode using following instructions(S3C2440 does not support synchronous bus mode).
	 * Otherwise,The CPU will working at HCLK not is FCLK
	 */
	mrc p15,0,r0,c1,c0,0 					/* coppocessor(协处理器) --> register */
	orr r0,r0, #0xC0000000					/* #R1_nF:OR:R1_iA  */
	mcr p15,0,r0,c1,c0,0					/* register --> coppocessor */

set_FClk:
	/* 
	 * set MPLLCON register(0x4C000004) for MDIV[19:12], PDIV[9:4], SDIV[1:0]
	 * FCLK = (2*m*Fin)/(p*2^s)
	 * m=(MDIV+8), p=(PDIV+2), s=SDIV
	 * When FCLK = 400MHz,should set MDIV=92, PDIV=1, SDIV=1
	 * Notes:should set FLCK after syncMode/HCLK/PCLK because FCLK set PLL
	 * and this time CPU is stop until frequency is change output from PLL
	 */
	ldr r0, =0x4C000004
	ldr r1, =((0x5C << 12) | (0x1 << 4) | (0x1 << 0))
	str r1, [r0]
	
set_stackFor_NandOrNor_startUp:
	/*
	 * auto choose start-up by Nor flash or Nand flash use Software
	 * According to the position of hardware switch choose to change
	 */
	mov r1, #0
	ldr r0, [r1]				/*Backup to save old values                                     */
	str r1, [r1]				/*write 0 to addr 0                                             */
	ldr r2, [r1]				/*read 0 from addr 0                                            */
	cmp r1, r2					/*compare "r1" with "r2"(Values at different times of addr 0)   */
	ldr sp, =0x40000000+4096	/*set stack bottom for start-up by Nor flash                    */
	moveq sp, #4096				/*if is starp-up by Nand,change stack bottom is 4096            */
	streq r0, [r1]				/*recover values before address 0 of backup                     */

	/* jump to soc_s3c2440_main run */
	bl soc_s3c2440_main
	
_halt:
	b _halt
