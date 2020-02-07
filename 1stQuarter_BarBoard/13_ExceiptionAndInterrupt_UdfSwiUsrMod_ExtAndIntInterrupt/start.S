/*
 * Time:2020年2月7日23:46:09
 * Author:Kangruojin
 * Version:1.1
 */
.text
.global _start
.code 32	/* ARM指令，32bit宽度 */

_start:

@================================================================================
@异常处向量表
@================================================================================
	b __reset									/* 0x00000000:reset */
#if 0
	ldr pc, =__und_instruction					/* 0x00000004:未定义指令异常 */
	ldr pc, =__software_interrupt				/* 0x00000008:软中断 */
	ldr pc, =__abort_prefetch					/* 0x0000000C:指令预取终止 */
	ldr pc, =__abort_data						/* 0x00000010:数据访问终止 */
	ldr pc, =__reserved_vector					/* 0x00000014:保留 */
	ldr pc, =__interrupt_request				/* 0x00000018:IRQ */
	ldr pc, =__fast_interrupt_request			/* 0x0000001C:FIQ */
#else
	ldr pc, __und_instruction_addr				/* 0x00000004:未定义指令异常 */
	ldr pc, __software_interrupt_addr			/* 0x00000008:软中断 */
	ldr pc, __abort_prefetch_addr				/* 0x0000000C:指令预取终止 */
	ldr pc, __abort_data_addr					/* 0x00000010:数据访问终止 */
	ldr pc, __reserved_vector_addr 				/* 0x00000014:保留 */
	ldr pc, __interrupt_request_addr			/* 0x00000018:IRQ */
	ldr pc, __fast_interrupt_request_addr		/* 0x0000001C:FIQ */
#endif

/* 
 * 采用链接地址，而不是直接赋地址值给PC,
 * __reset在能够确保一定不会超过4K，因此直接用b跳转
 */
__reset_addr:
	.word __reset
__und_instruction_addr:
	.word __und_instruction
__software_interrupt_addr:
	.word __software_interrupt
__abort_prefetch_addr:
	.word __abort_prefetch
__abort_data_addr:
	.word __abort_data
__reserved_vector_addr:
	.word __reserved_vector
__interrupt_request_addr:
	.word __interrupt_request
__fast_interrupt_request_addr:
	.word __fast_interrupt_request

@================================================================================
@异常处理函数逻辑实现：复位，也是正常初始化逻辑
@================================================================================
__reset:
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

/*
 * 注：由于thumb指令集不支持 mrs 等指令，因此该段屏蔽掉
 * #if (SOC_S3C2440_THUMB_INSTRUCTION_TEST == TRUE)
 * 
 * 	@bx指令检测到地址的bit0=1时，CPU state为thumb state
 * 	adr r0, thumb_instruction
 * 	add r0, r0, #1
 * 	bx r0
 * 	
 * .code 16	@THUMB指令，16bit宽度
 * @即调用的C函数为thumb指令编译的机器码
 * thumb_instruction:
 * 
 * #endif
 */
 
@================================================================================
@SDRAM、重定位、UART初始化
@================================================================================
soc_s3c2440_prog_init_start:
	/* sdram init */
	bl sdram0_init
	bl sdram1_init
	
	/* relocate memry init */
	bl relocate_init

change_cpu_mode_to_usr:
	/* 
	 * 复位完成之后，CPU处于SVC管理模式，切换到USR模式
	 * 使能中断，并设置SP_usr即USR模式的栈
	 */
	mrs r0, cpsr					/* 读取CPSR */
	bic r0, r0, #0x3				/* 修改Bits[4:0]=0b'10000,进入USR模式,SVC模式时为0b'10011 */
	bic r0, r0, #(0x1 << 7)			/* 清除CPSR的IRQ disable位(Bit7)，以打开CPU的中断响应 */
	msr cpsr, r0					/* 修改后的结果写回到CPSR */

	ldr sp, =0x34000000				/* 设置SP_usr */

app_start:
	/* 
	 * 链接地址赋给PC，跳转到SDRAM中执行(不要用bl sdram_start)，
	 * thumb指令不能直接给pc赋某个地址，通过R0转换一下
	 */
	ldr r0, =sdram_start
	mov pc, r0
	
sdram_start:
	/* UART init */
	bl uart_init

	/* jump to soc_s3c2440_main run */
	bl soc_s3c2440_main

/* 未定义指令异常测试 */
#if (TRUE == SOC_S3C2440_UND_INSTRUCTION_TEST)	
	/* 构造一条未定义指令, dead code */
	.word 0xdeadc0de
	
/* 软件中断异常测试 */
#elif (TRUE ==SOC_S3C2440_SWI_INSTRUCTION_TEST )
	/* 
	 * CPU软件中断异常模式通过swi指令来进入，
	 * swi指令紧接着的值可以用来区分服务类型
	 * swi指令和svc指令其硬编码一致，都是0XEFxxxxxx
	 */
	swi 0x1234
	svc 0x4321
	.word 0xefabcdef
#endif
	
soc_s3c2440_prog_over:
	bl soc_s3c2440_over

_halt:
	b _halt

@================================================================================
@异常处理函数逻辑实现：未定义指令异常
@================================================================================

/* 未知异常中断处理 */
__und_instruction:
	/* 先设置undefined模式的sp栈 */
	ldr sp, =0x34000000

	/* 
	 * 在undefined异常处理时，可能会修改r0-r12，LR_und保存返回地址
	 * r0-r12、LR_und(r14)先保存下来，stmdb指令先减后存
	 * LR寄存器区分不同模式，不同模式的LR寄存器物理上不是同一个寄存器
	 * LR_und 寄存器保存被中断模式中下一条指令地址
	 * SPSR_und 寄存器保存有被中断模式的CPSR寄存器值
	 * CPSR寄存器的Bits[4:0]保存有当前CPU模式 = 0b'11011
	 */
	stmdb sp!, {r0-r12, lr}

	/* 处理异常 */
	mrs r0, cpsr
	mrs r1, spsr
	ldr r2, =und_exception_string
	bl exception_printException

	/* 恢复现场，ldmia指令先取后加，LR_und 的赋给 PC */
	ldmia sp!, {r0-r12, pc}^	/* ^会把SPSR恢复到CPSR里 */
	
und_exception_string:
	.string "undefined instruction exception!"

	@保证string不会破坏四字节对齐（编译器没有对string进行四字节对齐）
	.align 4	
	
@================================================================================
@异常处理函数逻辑实现：未定义指令异常
@================================================================================
__software_interrupt:	
	/* 先设置swi软件中断模式(SVC管理模式)的sp栈 */
	ldr sp, =0x33e00000

	/* 
	 * 在swi异常处理时，可能会修改r0-r12，LR_svc保存返回地址
	 * r0-r12、LR_svc(r14)先保存下来，stmdb指令先减后存
	 * LR_svc 寄存器保存被中断模式中下一条指令地址
	 * SPSR_svc 寄存器保存有被中断模式的CPSR寄存器值
	 * CPSR寄存器的Bits[4:0]保存有当前CPU模式 = 0b'10011
	 */
	stmdb sp!, {r0-r12, lr}

	/* R4寄存器在调用函数时会保存现场，因此借用R4保存lr */
	mov r4, lr

	/* 处理异常 */
	mrs r0, cpsr
	mrs r1, spsr
	ldr r2, =swi_exception_string
	bl exception_printException

	/* 获取LR保存的返回地址的上一条指令，即swi指令地址 */
	sub r0, r4, #4
	bl exception_printSwiValue
	
	/* 恢复现场，ldmia指令先取后加，LR_svc 的赋给 PC */
	ldmia sp!, {r0-r12, pc}^	/* ^会把SPSR恢复到CPSR里 */
	
swi_exception_string:
	.string "software interrupt exception!"

	@保证string不会破坏四字节对齐（编译器没有对string进行四字节对齐）
	.align 4	

@================================================================================
@异常处理函数逻辑实现：终止指令预取
@================================================================================
__abort_prefetch:		

@================================================================================
@异常处理函数逻辑实现：终止数据访问
@================================================================================
__abort_data:			

@================================================================================
@异常处理函数逻辑实现：保留
@================================================================================
__reserved_vector:		

@================================================================================
@异常处理函数逻辑实现：中断请求处理IRQ
@================================================================================
__interrupt_request:

@================================================================================
@异常处理函数逻辑实现：快速中断请求处理FIQ
@================================================================================
__fast_interrupt_request:

