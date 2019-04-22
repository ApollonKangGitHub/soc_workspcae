@
@ Time:2019年4月1日23:15:07 
@ Author:Apollon_krj
@ 所有的三个LED点灯（GPF4~GPF6）:1为灭，0为亮
@ 流水灯
@ 
@ arm的函数调用与返回，与8086差别还是蛮大的
@ 后者用stack存放函数地址，前者则直接是lr和pc两个寄存器来搞
@ 8086更趋向与软件逻辑，是两步：先call，再ret
@ 而arm则更随心所欲一些，是三步：先保存返回地址，再修改pc指针，执行完回复pc指针
@ 即call指令相当于“LDR lr, =label_ret”、“LDR lr,=label_call”两条指令
@ 而ret指令相当于“MOV pc,lr”一条指令，也即8086“MOV pc,ss:[sp]”
@ 注意：ARM中，pc寄存器的值，永远是当前执行指令地址+8，而不是当前指令地址
@ 因为：当前执行指令地址为A，A+4指令已经开始被解析，而A+8指令已经开始被读取（即PC）
@

.text
.global _start

_start:
	LDR r0, =0x00				@ 0000 0000,亮亮亮 
	LDR lr, =one_light_delay	@ 点灯完成后的地址，即返回地址
	LDR pc, =_led_light			@ 点灯指令起始地址，等价于 B _led_light
one_light_delay:
	LDR lr, =two_light			@ 延时完成后的指令地址
	B _delay					@ 延时指令起始地址
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
two_light:
	LDR r0,	=0x70				@ 0111 0000,灭灭灭
	LDR lr, =two_light_delay
	B _led_light
two_light_delay:
	LDR lr, =_start				@ 死循环
	B _delay
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
_led_light:
	LDR r1, =0x56000050
	LDR r2, =0x1500				@ choose GPF:0001 0101 0000 0000,选中三个GPIO的输入功能
	LDR r3, [r1]				@ 获取原来的设置
	ORR r2, r3					@ 将新配置（GPF4~GPF6 output）增加上去
	STR r2, [r1]

	LDR r1, =0x56000054
	MOV r2, r0					@ set bit:r0
	STR r2, [r1]
	MOV pc, lr					@ 回复函数返回地址，接着执行
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
_delay:
	LDR r3, =0x100				@ 延时256 * 256 = 65536个指令周期
loop_ext:
	LDR r4, =0x100
loop_int:
	sub r4, r4, #1
	cmp r4, #0
	BNE loop_int				@ 内层循环条件判断
	
	sub r3, r3, #1
	cmp r3, #0
	BNE loop_ext				@ 外层循环条件判断
	MOV pc, lr					@ 循环结束，返回执行
	