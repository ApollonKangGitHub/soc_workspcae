#include <tools.h>
#include <uart.h>
#include <lcd_common.h>

#define MMU_ENTRY_SIZE			(0x100000)
#define MMU_ENTRY_PA_MASK		(0xFFF00000)

/* 权限检查均设为默认值，需要设置DOMAIN寄存器(CP15'C3')默认值 */
#define MMU_ENTRY_AP_DLF		(0x3 << 10)
#define MMU_ENTRY_DOMAIN_DLF	(0x0 << 5)

/* data cache和write buffer */
#define MMU_ENTRY_CB_DISABLE	(0x0 << 2)
#define MMU_ENTRY_CB_ENABLE		(0x3 << 2)

/* 其他字段 */
#define MMU_ENTRY_TYPE_DLF		((0x1 << 4) | (0x1 << 1))	

#define MMU_ENTRY_FOR_IO		(MMU_ENTRY_AP_DLF | MMU_ENTRY_DOMAIN_DLF | MMU_ENTRY_CB_DISABLE | MMU_ENTRY_TYPE_DLF)
#define MMU_ENTRY_FOR_MEM		(MMU_ENTRY_AP_DLF | MMU_ENTRY_DOMAIN_DLF | MMU_ENTRY_CB_ENABLE | MMU_ENTRY_TYPE_DLF)
#define MMU_ENTRY_IS_IO			(TRUE)
#define MMU_ENTRY_IS_MEM		(FALSE)

void mmu_create_entry_by_va_pa
(
	uint32 * ttb,
	uint32 va, 
	uint32 pa,
	BOOL isIo
)
{
	uint32 index = 0;

	index = va / MMU_ENTRY_SIZE;
	
	if (isIo) {
		ttb[index] = (pa & MMU_ENTRY_PA_MASK) | MMU_ENTRY_FOR_IO;
	}
	else {
		ttb[index] = (pa & MMU_ENTRY_PA_MASK) | MMU_ENTRY_FOR_MEM;
	}
}

/*
 * 创建一级页表(1M):
 * VA			PA 			AP	D	C	B
 * 
 * SRAM mapping:
 * 0  			0			N  	N 	1	1
 * 
 * Nor FLash mapping:
 * 0x40000000	0x40000000	N  	N 	1 	1
 * 
 * 64M SDRAM mapping:
 * 0x30000000	0x30000000	N  	N 	1 	1 
 * ...			...			...	...	...	...		
 * 0x33F00000	0x33F00000	N  	N 	1 	1
 *
 * register mapping:
 * 0x48000000	0x48000000	N	N	0	0
 * ...			...			...	...	...	...
 * 0x5B000000	0x5B000000	N	N	0	0
 *
 * LCD frameBuffer(属于SDRAM的一部分，但是需要单独映射):
 * 为了数据实时性，不用data cache和write buffer
 * 0x33C00000	0x33C00000	N	N	0	0
 *
 * Link Address mapping:
 * 0xB0000000	0x30000000	N	N	1	1
 *
 * 名词解释:
 * VA:Virutal Address
 * PA:Physical Address
 * AP:Address Permissions
 * D:Domain
 * C:Cache For Data
 * B:Write Buffer

 * N:No care
 */
void mmu_create_page_table_level_1(void)
{
	/* 
	 * 寻找一块儿不会使用的地址作为页表（VA:0~4G）, 每个表项32Byte
	 * 4G/1MB * 32Byte = 4K * 32 = 16K
	 * ttb:translation table base
	 */
	uint32 * ttb = (uint32 *)(0x32000000);
	uint32 va = 0;
	uint32 pa = 0;

	/* 根据VA、PA设置页表 */
	
	/* 
	 * 1. SRAM or Nor Flash only 1M mapping
	 * 由于0地址被用来做Nand、Nor启动的判断，因此这块儿地址也不能用DCACHE
	 */
	mmu_create_entry_by_va_pa(ttb, 0, 0, MMU_ENTRY_IS_IO);
	
	/* 2. stack for Nor Flash boot mapping */
	mmu_create_entry_by_va_pa(ttb, 0x40000000, 0x40000000, MMU_ENTRY_IS_MEM);

	/* 3. 64M SDRAM mapping */
	va = 0x30000000;
	pa = 0x30000000;
	while (va < 0x34000000)
	{
		mmu_create_entry_by_va_pa(ttb, va, pa, MMU_ENTRY_IS_MEM);
		va += MMU_ENTRY_SIZE;
		pa += MMU_ENTRY_SIZE;
	}

	/* 4. reg mapping */
	va = 0x48000000;
	pa = 0x48000000;
	while (va <= 0x5B000000)
	{
		mmu_create_entry_by_va_pa(ttb, va, pa, MMU_ENTRY_IS_IO);
		va += MMU_ENTRY_SIZE;
		pa += MMU_ENTRY_SIZE;
	}

	/* 5.FrameBuffer mapping */
	mmu_create_entry_by_va_pa(ttb, LCD_COMMON_FRAMEBUFFER_BASE, LCD_COMMON_FRAMEBUFFER_BASE, MMU_ENTRY_IS_IO);
	
	/* 6.Link address mapping */
	mmu_create_entry_by_va_pa(ttb, 0XB0000000, 0x30000000, MMU_ENTRY_IS_MEM);
}

/* 使能MMU */
void mmu_enable(void)
{
	__asm(
		/* 把页表基址赋给协处理器CP15的寄存器C2 */
		"ldr r0, =0x32000000\n"
		"mcr p15, 0, r0, c2, c0, 0\n"

		/* 设置Dmain为0xffffffff, 不进行权限检查 */
		"ldr r0, =0xffffffff\n"
		"mcr p15, 0, r0, c3, c0, 0\n"
		/* 使能icache,dcache,mmu */
		"mrc p15, 0, r0, c1, c0, 0\n"	/* 读取协处理器 CP15 的控制寄存器 C1 */
		"orr r0, r0, #(1<<12)\n"  		/* enable icache */
		"orr r0, r0, #(1<<2)\n"  		/* enable dcache */
		"orr r0, r0, #(1<<0)\n"  		/* enable mmu */
		"mcr p15, 0, r0, c1, c0, 0\n"	/* 修改后的 CP15 的控制寄存器写回去 */
		);
}

void mmu_init(void)
{
	mmu_create_page_table_level_1();
	mmu_enable();
}

void mmu_dump(uint32 addr)
{
	int index = 0; 
	uint32 * ttb = (uint32 *)(0x32000000);

	index = addr/MMU_ENTRY_SIZE;
	print_screen("ttb[%d]:%x  ", index, ttb[index]);
}

/* 页表打印 */
void mmu_page_table_dump(void)
{
	uint32 va = 0;
	int num = 0;
	
	print_screen("\r\n\n page table for SRAM or NOR FLASH:\r\n");
	mmu_dump(0);
	
	print_screen("\r\n\n page table for user stack:\n\n");
	mmu_dump(0x40000000);

	print_screen("\r\n\n page table for SDRAM:\r\n");
	va = 0x30000000;
	num = 0;
	while (va < 0x34000000)
	{
		mmu_dump(va);
		va += MMU_ENTRY_SIZE;
		num++;
		if (num % 4 == 0)
		{
			print_screen("\r\n");
		}
	}

	print_screen("\r\n\n page table for soc reg:\r\n");
	va = 0x48000000;
	num = 0;
	while (va <= 0x5B000000)
	{
		mmu_dump(va);
		va += MMU_ENTRY_SIZE;
		num++;
		if (num % 4 == 0)
		{
			print_screen("\r\n");
		}
	}

	print_screen("\r\n\n page table for link address:\r\n");
	mmu_dump(0XB0000000);
}

