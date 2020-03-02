#include <tools.h>
#include <log.h>
#include <memCtrl.h>
#include <nand_flash_lib.h>
#include <soc_s3c2440_init.h>

/* 如果0地址可以直接写则是Nand启动，否则是Nor启动 */
BOOL isBootFromNorFlash(void)
{
	volatile uint32 * pTest = (volatile uint32 * )(0x0);
	uint32 valBak = *pTest;
	uint32 valTest = 0x12345678;
	*pTest = valTest;

	if (valTest == *pTest)
	{
		/* 写成功则是Nand启动 */
		*pTest = valBak;
		return FALSE;
	}

	return TRUE;
}

/* 
 * 注意：在汇编中，lds脚本变量的值可以直接引用，
 * 而在C中，对lds脚本变量取地址才表示该脚本变量的值，此处的地址并非真正的地址
 * 原理是：lds变量（实际为连接时就确定的常量）存放在符号表里，和全局变量相似，
 * 不过全局变量在符号表中存放的是（变量名+变量地址），访问全局变量为 *(&name)，
 * 而lds变量在符号表中存放的是（变量名+变量值），访问该值的方式自然就是 &name，
 *
 * 优化：改为4字节访问，因为Flash为16bit位宽，SDRAM为32bit位宽，
 * ldrb/strb的方式访问硬件次数过多，因此采用ldr/str
 * 同时需要修改lds脚本，保证为4字节对齐，防止BSS段清零和DATA段复制访问越界，互相影响
 * 此外所有section都重定位（但是当前仍然不支持Nand Flash大于4K的重定位）
 */
void code_relocation(void)
{
	extern uint32 _code_start_;
	extern uint32 _bss_start_;

	volatile uint32 * src = (volatile uint32 *)(0x0);
	volatile uint32 * dst = (volatile uint32 *)(&_code_start_);
	volatile uint32 * end = (volatile uint32 *)(&_bss_start_);
	int len = ((int)(&_bss_start_) - (int)(&_code_start_));

	/* 重定位内存初始化 */
	if (isBootFromNorFlash())
	{
		/* Nor数据复制到SDRAM */
		while(dst < end)
		{
			*dst++ = *src++;
		}
	}
	else
	{
		/* Nand Flash初始化 */
		nand_flash_init();
		/* Nand数据复制到SDRAM */
		nand_flash_data_read((uint32)src, (uint8 *)dst, len);
	}

}

void bss_clear(void)
{
	extern uint32 _bss_start_;
	extern uint32 _bss_end_;

	volatile uint32 * bss_start = (volatile uint32 *)(&_bss_start_);
	volatile uint32 * bss_end = (volatile uint32 *)(&_bss_end_);

	/* BSS段初始化为0 */
	while (bss_start < bss_end)
	{
		*bss_start++ = 0;
	}
}

void relocate_init(void)
{
#if	(FALSE == SOC_S3C2440_MMU_CACHE_ENABLE)
	sdram_init(SOC_MEMCTRL_BANK_SDRAM_ALL);
#else
	/* MMU开启时sdram初始化放在前面做 */
#endif
	code_relocation();
	bss_clear();
}
