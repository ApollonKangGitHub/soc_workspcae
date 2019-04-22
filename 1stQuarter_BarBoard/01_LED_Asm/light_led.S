
/* 
 * 2019年3月31日23:16:24
 * 点亮LED1:设置GPF4为输出引脚，写入bit4=1为灭，bit=0为亮
 */
.text
.global _start

_start:
/* 
 * GFPCON [GPF4]: 
 * address[Bit GPF4]:0x5600 0050 [9:8]
 * input:00
 * output:01
 * EINT:10
 * Reserved:11
 * 选择GPF4为输出引脚，即bit9:8置为01，即GPFCON=0x100
 */
	LDR r0, =0x56000050
	LDR r1, =0x100			/* MOV R1,#0x100 */
	STR r1, [r0]

/*
 * GFPDATA [GPF4]:
 * address[Bit GPF4]:0x5600 0054 [4]
 * 1:high
 * 0:low
 * 点亮即将bit4置为0，因为LED是上拉的
*/
	LDR r0, =0x56000054
	LDR r1, =0x0			/* MOV R1,#0 */
	STR r1, [r0]

/* loop */
_wait:
	B _wait
