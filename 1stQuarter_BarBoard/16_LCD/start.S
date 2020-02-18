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
	B __reset									/* 0x00000000:reset */
#if 0
	@LDR PC, =__und_inSTRuction					/* 0x00000004:未定义指令异常 */
	@LDR PC, =__software_interrupt				/* 0x00000008:软中断 */
	@LDR PC, =__abort_prefetch					/* 0x0000000C:指令预取终止 */
	@LDR PC, =__abort_data						/* 0x00000010:数据访问终止 */
	@LDR PC, =__reserved_vector					/* 0x00000014:保留 */
	@LDR PC, =__interrupt_request				/* 0x00000018:IRQ */
	@LDR PC, =__fast_interrupt_request			/* 0x0000001C:FIQ */
#else
	LDR PC, __und_inSTRuction_addr				/* 0x00000004:未定义指令异常 */
	LDR PC, __software_interrupt_addr			/* 0x00000008:软中断 */
	LDR PC, __abort_prefetch_addr				/* 0x0000000C:指令预取终止 */
	LDR PC, __abort_data_addr					/* 0x00000010:数据访问终止 */
	LDR PC, __reserved_vector_addr 				/* 0x00000014:保留 */
	LDR PC, __interrupt_request_addr			/* 0x00000018:IRQ */
	LDR PC, __fast_interrupt_request_addr		/* 0x0000001C:FIQ */
#endif

/* 
 * 采用链接地址，而不是直接赋地址值给PC,
 * __reset在能够确保一定不会超过4K，因此直接用b跳转
 */
__reset_addr:
	.WORD __reset
__und_inSTRuction_addr:
	.WORD __und_inSTRuction
__software_interrupt_addr:
	.WORD __software_interrupt
__abort_prefetch_addr:
	.WORD __abort_prefetch
__abort_data_addr:
	.WORD __abort_data
__reserved_vector_addr:
	.WORD __reserved_vector
__interrupt_request_addr:
	.WORD __interrupt_request
__fast_interrupt_request_addr:
	.WORD __fast_interrupt_request

@================================================================================
@异常处理函数逻辑实现：复位，也是正常初始化逻辑
@================================================================================
__reset:
set_watchDog_timer_disable:
	/* disable watchDog Timer reset function */
	LDR R0, =0x53000000
	LDR R1, =0
	STR R1, [R0]

set_clockFor_FclkHclkPClk:
	/* 
	 * FCLK from 12MHz acorss PLL raise to 400MHz for CPU
	 * HCLK is high frequency for connected to  High-SPeed BUS such as LCD/NAND flash/Nor flash controller, etc.
	 * PCLK is low frequency for connect to Low-SPeed BUS such as I2C/GPIO/ADC/PWM, etc.
	*/
set_lockTime:
	/* set locktime to default value,LOCKTIME(0x4C000000) register */
	LDR R0, =0x4C000000
	LDR R1, =0xFFFFFFFF
	STR R1, [R0]

set_HClkANDPClk:
	/* 
	 * set HCLK = FCLK/4 = 100MHz, set PCLK = HCLK/2 = 50MHz, FCLK:HCLK:PCLK=8:2:1
	 * should set CLKDIVN register(0x4C000014) for HDIVN[2:1]=0b'10,PDIVN[0]=0b'1
	 */
	LDR R0, =0x4C000014
	LDR R1, [R0]
	AND R1, #0xF8		/* clear last 3 bits */
	ORR R1, #0x5		/* set last 3 bits 3b'101*/
	STR R1, [R0]
	/* when HDIVN is 0b'10,must set CSMDIVN(0x4C000018) bit 9 is 0, this bit default is 0 */
	LDR R0, =0x4C000018
	LDR R1, [R0]
	LDR R2, =0xFFFFFEFF
	AND R1, R2
	STR R1, [R0]

set_MMUAsyncBusMode:
	/*
	 * notes:If HDIVN is not 0, the CPU bus mode has to be changed from the fast bus mode
	 * to the asynchronous bus mode using following inSTRuctions(S3C2440 does not support synchronous bus mode).
	 * Otherwise,The CPU will working at HCLK not is FCLK
	 */
	MRC p15,0,R0,c1,c0,0 					/* coppocessor(协处理器) --> register */
	ORR R0,R0, #0xC0000000					/* #R1_nF:OR:R1_iA  */
	MCR p15,0,R0,c1,c0,0					/* register --> coppocessor */

set_FClk:
	/* 
	 * set MPLLCON register(0x4C000004) for MDIV[19:12], PDIV[9:4], SDIV[1:0]
	 * FCLK = (2*m*Fin)/(p*2^s)
	 * m=(MDIV+8), p=(PDIV+2), s=SDIV
	 * When FCLK = 400MHz,should set MDIV=92, PDIV=1, SDIV=1
	 * Notes:should set FLCK after syncMode/HCLK/PCLK because FCLK set PLL
	 * AND this time CPU is stop until frequency is change output from PLL
	 * FCLK:HCLK:PCLK = 400M:100M:50M
	 */
	LDR R0, =0x4C000004
	LDR R1, =((0x5C << 12) | (0x1 << 4) | (0x1 << 0))
	STR R1, [R0]
	
set_stackFor_NANDOrNor_startUp:
	/*
	 * auto choose start-up by Nor flash or NAND flash use Software
	 * According to the position of hardware switch choose to change
	 */
	MOV R1, #0
	LDR R0, [R1]				/*Backup to save old values                                     */
	STR R1, [R1]				/*write 0 to addr 0                                             */
	LDR R2, [R1]				/*read 0 from addr 0                                            */
	CMP R1, R2					/*compare "R1" with "R2"(Values at different times of addr 0)   */
	LDR SP, =0x40000000+4096	/*set stack bottom for start-up by Nor flash                    */
	MOVEQ SP, #4096				/*if is starp-up by NAND,change stack bottom is 4096            */
	STREQ R0, [R1]				/*recover values before address 0 of backup                     */

/*
 * 注：由于thumb指令集不支持 MRS 等指令，因此该段屏蔽掉
 * #if (SOC_S3C2440_THUMB_INSTRUCTION_TEST == TRUE)
 * 
 * 	@bx指令检测到地址的bit0=1时，CPU state为thumb state
 * 	adr R0, thumb_inSTRuction
 * 	add R0, R0, #1
 * 	bx R0
 * 	
 * .code 16	@THUMB指令，16bit宽度
 * @即调用的C函数为thumb指令编译的机器码
 * thumb_inSTRuction:
 * 
 * #endif
 */
 
@================================================================================
@重定位、CPU模式修改、UART初始化、主函数启动
@================================================================================
soc_s3c2440_prog_init_start:
	/* relocate memry init */
	BL relocate_init

change_cpu_mode_to_usr:
	/* 
	 * 复位完成之后，CPU处于SVC管理模式，切换到USR模式
	 * 使能中断，并设置SP_usr即USR模式的栈
	 */
	MRS R0, CPSR					/* 读取CPSR */
	BIC R0, R0, #0xF				/* 修改Bits[4:0]=0b'10000,进入USR模式,SVC模式时为0b'10011 */
	BIC R0, R0, #(0x1 << 7)			/* 清除CPSR的IRQ disable位(Bit7)，以打开CPU的中断响应 */
	MSR CPSR, R0					/* 修改后的结果写回到CPSR */

	LDR SP, =0x34000000				/* 设置SP_usr */

app_start:
	/* 
	 * 链接地址赋给PC，跳转到SDRAM中执行(不要用bl sdram_start)，
	 * thumb指令不能直接给PC赋某个地址，通过R0转换一下
	 */
	LDR R0, =sdram_start
	MOV PC, R0
	
sdram_start:
	/* UART init */
	BL uart_init	

	/* jump to soc_s3c2440_main run */
	BL soc_s3c2440_main
	BL test_end
	
/* 未定义指令异常测试 */
#if (TRUE == SOC_S3C2440_UND_INSTRUCTION_TEST)	
	/* 构造一条未定义指令, dead code */
	.WORD 0xdeadc0de
	
/* 软件中断异常测试 */
#elif (TRUE == SOC_S3C2440_SWI_INSTRUCTION_TEST)
	/* 
	 * CPU软件中断异常模式通过SWI指令来进入，
	 * SWI指令紧接着的值可以用来区分服务类型
	 * SWI指令和SVC指令其硬编码一致，都是0XEFxxxxxx
	 */
	SWI 0x1234
	SVC 0x4321
	.WORD 0xefabcdef
#endif
	
soc_s3c2440_prog_over:
	BL soc_s3c2440_over

_halt:
	B _halt

@================================================================================
@异常处理函数逻辑实现：未定义指令异常
@================================================================================

/* 未知异常中断处理 */
__und_inSTRuction:
	/* 先设置undefined模式的SP栈 */
	LDR SP, =0x33f00000

	/* 
	 * 在undefined异常处理时，可能会修改R0-R12，LR_und保存返回地址
	 * R0-R12、LR_und(R14)先保存下来，STMDB指令先减后存
	 * LR寄存器区分不同模式，不同模式的LR寄存器物理上不是同一个寄存器
	 * LR_und 寄存器保存被中断模式中下一条指令地址
	 * SPSR_und 寄存器保存有被中断模式的CPSR寄存器值
	 * CPSR寄存器的Bits[4:0]保存有当前CPU模式 = 0b'11011
	 */
	STMDB SP!, {R0-R12, LR}

	/* R4寄存器在调用函数时会保存现场 */
	MOV R4, LR
	
	/* 处理异常 */
	MRS R0, CPSR
	MRS R1, SPSR
	LDR R2, =und_exception_string
	BL exception_printException
	
	/* 获取LR保存的返回地址的上一条指令 */
	SUB R0, R4, #4
	BL exception_printUndefinedException

	/* 恢复现场，LDMIA指令先取后加，LR_und 的赋给 PC */
	LDMIA SP!, {R0-R12, PC}^	/* ^会把SPSR恢复到CPSR里 */
	
und_exception_string:
	.string "undefined inSTRuction exception!"

	@保证string不会破坏四字节对齐（编译器没有对string进行四字节对齐）
	.ALIGN 4	
	
@================================================================================
@异常处理函数逻辑实现：未定义指令异常
@================================================================================
__software_interrupt:	
	/* 先设置SWI软件中断模式(SVC管理模式)的SP栈 */
	LDR SP, =0x33e00000

	/* 
	 * 在SWI异常处理时，可能会修改R0-R12，LR_svc保存返回地址
	 * R0-R12、LR_svc(R14)先保存下来，STMDB指令先减后存
	 * LR_svc 寄存器保存被中断模式中下一条指令地址
	 * SPSR_svc 寄存器保存有被中断模式的CPSR寄存器值
	 * CPSR寄存器的Bits[4:0]保存有当前CPU模式 = 0b'10011
	 */
	STMDB SP!, {R0-R12, LR}

	/* R4寄存器在调用函数时会保存现场，因此借用R4保存LR */
	MOV R4, LR

	/* 处理异常 */
	MRS R0, CPSR
	MRS R1, SPSR
	LDR R2, =swi_exception_string
	BL exception_printException

	/* 获取LR保存的返回地址的上一条指令，即SWI指令地址 */
	SUB R0, R4, #4
	BL exception_printSwiValue
	
	/* 恢复现场，LDMIA指令先取后加，LR_svc 的赋给 PC */
	LDMIA SP!, {R0-R12, PC}^	/* ^会把SPSR恢复到CPSR里 */
	
swi_exception_string:
	.string "software interrupt exception!"

	@保证string不会破坏四字节对齐（编译器没有对string进行四字节对齐）
	.ALIGN 4	

@================================================================================
@异常处理函数逻辑实现：终止指令预取
@================================================================================
__abort_prefetch:		
	/* 先设置abort prefetch模式的SP栈 */
	LDR SP, =0x33d00000

	b _halt
@================================================================================
@异常处理函数逻辑实现：终止数据访问
@================================================================================
__abort_data:	
	/* 先设置abort prefetch模式的SP栈 */
	LDR SP, =0x33c00000
	b _halt

@================================================================================
@异常处理函数逻辑实现：保留
@================================================================================
__reserved_vector:		
	/* 先设置reserved模式的SP栈 */
	LDR SP, =0x33b00000
	b _halt

@================================================================================
@异常处理函数逻辑实现：中断请求处理IRQ
@================================================================================
__interrupt_request:
	/* 先设置irq模式的SP栈 */
	LDR SP, =0x33a00000

	/* 
	 * 在irq异常处理时，可能会修改R0-R12，LR_irq保存返回地址
	 * R0-R12、LR_irq(R14)先保存下来，STMDB指令先减后存
	 * LR_irq 寄存器保存被中断模式中下一条指令地址
	 * SPSR_irq 寄存器保存有被中断模式的CPSR寄存器值
	 * CPSR寄存器的Bits[4:0]保存有当前CPU模式 = 0b'10010
	 */
	SUB LR, LR, #4			@对于IRQ异常，LR寄存器减4才是返回地址
	STMDB SP!, {R0-R12, LR}

	/* 处理异常 */
	MRS R0, CPSR
	MRS R1, SPSR
	LDR R2, =irq_exception_string
	/* BL exception_printException */
	BL interrupt_irq_deal_start

	/* 恢复现场，LDMIA指令先取后加，LR_irq 的赋给 PC */
	LDMIA SP!, {R0-R12, PC}^	/* ^会把SPSR恢复到CPSR里 */

irq_exception_string:
	.string "interrupt rquest exception!"

	@保证string不会破坏四字节对齐（编译器没有对string进行四字节对齐）
	.ALIGN 4	

@================================================================================
@异常处理函数逻辑实现：快速中断请求处理FIQ
@================================================================================
__fast_interrupt_request:
	/* 先设置fiq模式的SP栈 */
	LDR SP, =0x33900000
	b _halt

