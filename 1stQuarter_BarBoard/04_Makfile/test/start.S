@
@ Time:2019年4月5日09:40:31
@ Author:Kangruojin
@
.text
.global _start

_start:
	@disable watchDog Timer reset function
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

	@auto choose start-up by Nor flash or Nand flash use Software
	@According to the position of hardware switch choose to change
	mov r1, #0
	ldr r0, [r1]				@Backup to save old values
	str r1, [r1]				@write 0 to addr 0
	ldr r2, [r1]				@read 0 from addr 0
	cmp r1, r2					@compare r1 with r2(Values at different times of addr 0)
	ldr sp, =0x40000000+4096	@set stack bottom for start-up by Nor flash
	moveq sp, #4096				@if is starp-up by Nand,change stack bottom is 4096
	streq r0, [r1]				@recover values before address 0 of backup

	@led light for C language
	bl led_light
	
_halt:
	b _halt
