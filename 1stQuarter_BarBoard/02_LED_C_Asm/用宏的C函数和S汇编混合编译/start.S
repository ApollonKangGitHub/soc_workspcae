.text
.global _start

_start:
@ 设置内存栈空间
	@设置为Nand[4K]启动，将栈设置在Nand内存顶部
	ldr sp, =0x1000				
	@设置Nor[]启动，将栈设置在Nor内存顶部
	@ldr sp, =0x40001000		

	@调用main函数，并保存返回地址
	bl main
	
@返回地址
_halt:
	b _halt