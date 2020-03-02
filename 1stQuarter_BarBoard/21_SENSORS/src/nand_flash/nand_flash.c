#include <uart.h>
#include <tools.h>
#include <nand_flash.h>
#include <log.h>
#include <soc_s3c2440_public.h>

/*
 * 仅仅Nand FLash init和read data需要在重定位完成前使用
 * 因此其他功能另外编译文件，只保证nand init和nand read data在编译时处于前4K即可
 */
void nand_flash_cmd_byte(uint8 cmd)
{
	volatile uint8 i = 0;
	
	NFCMDr = cmd; 
	for(i = 0; i < 10; i++);	/* 延时保证数据稳定 */
}

void nand_flash_addr_byte(uint8 addr)
{
	volatile uint8 i = 0;
	
	NFADDRr = addr; 
	for(i = 0; i < 10; i++);	/* 延时保证数据稳定 */
}

/* 发出地址(读操作、写操作) */
void nand_flash_addr_page_offset(uint32 page, uint32 offset)
{
	/* 2Byte offset Addr,3Byte Page Addr */
	
	/* offset Addr */
	nand_flash_addr_byte(offset & 0xFF);
	nand_flash_addr_byte((offset >> 8) & 0xFF);
	
	/* Page Addr */
	nand_flash_addr_byte(page & 0xFF);
	nand_flash_addr_byte((page >> 8) & 0xFF);
	nand_flash_addr_byte((page >> 16) & 0xFF);
}

void nand_flash_init(void)
{
	/* 
	 * 设置NADN FLASH时序,在HCLK为100MHZ的前提下（stat.S）
	 * 即HCLK的周期为10ns，因此TWRPH0 取最小保证值1，TWRPH1取最小保证0即可，
	 * 由 Nand Flash K9F2G08U0C 的时序图可知，TACLS设置为0即可
	 * 对应 SOC_S3C2440 寄存器 NFCONF 的配置:
	 * TACLS : [13:12] : CLE & ALE duration setting value (0~3), Duration = HCLK x TACLS
	 * TWRPH0: [10:8]  : TWRPH0 duration setting value (0~7) Duration = HCLK x ( TWRPH0 + 1 ) 
	 * TWRPH1: [6:4]   : TWRPH1 duration setting value (0~7) Duration = HCLK x ( TWRPH1 + 1 ) 
	 */
	NFCONFr = (NAND_K9F2G08U0C_TACLS << 12) 
			| (NAND_K9F2G08U0C_TWRPH0 << 8) 
			| (NAND_K9F2G08U0C_TWRPH1 << 4);


	/* 初始化ECC，禁止片选，使能Nand Flash控制器 */
	NFCONTr = (NAND_INIT_ECC_DECODER_ENCODER << 4) 
			| (NAND_nFCE_CTRL_DISABLE << 1) 
			| (NAND_NAND_CONTOLLER_ENABLE << 0);
}

/*
 * Nand Flash结构
 * 
 * Page   Page Data(2048Bytes)   OOB区(64Bytes)
 * ...           ...                  ...
 *  3   |0 1 2 3 ... 2046 2047 | 2048 ... 2111 |
 *  2   |0 1 2 3 ... 2046 2047 | 2048 ... 2111 |
 *  1   |0 1 2 3 ... 2046 2047 | 2048 ... 2111 |
 *  0   |0 1 2 3 ... 2046 2047 | 2048 ... 2111 |
 *
 * 由于Nand Flash读取的时候可能会发生位反转，OOB存放的是写每一个Page时的校验码
 * 因此读取每一页的时候如果发现位反转，则与校验码不匹配就可以进行数据的校验
 * 校验是Nand Flash硬件为位反转的缺陷而设计的，也是由硬件完成校验计算和修正的，
 * CPU/软件操作不关心OOB，OOB对CPU也不可见。OOB:Out Of Bank
 * 发一次读命令最多读取1页的数据，多出一页的内容需要再次发出读命令
 * 注意：NFDATAr寄存器在发出读命令、地址、以及确认命令之后，再去读，
 * 每读一次，NFDATAr寄存器返回当前地址数据，并且自动加载下一个地址数据
 */
void  nand_flash_data_read(uint32 addr, uint8 * buf, uint32 len)
{
	uint32 index = 0;
	uint32 page = addr / NAND_FLASH_PAGE_SIZE;
	uint32 offset = addr % NAND_FLASH_PAGE_SIZE;

	/* 启用片选 */
	NAND_FALSH_SELECT();

	while (index < len)
	{
		/* 发出读命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_CMD__);

		/* 发出页地址和偏移地址 */
		nand_flash_addr_page_offset(page, offset);
		
		/* 发出读数据地址确认命令 */
		nand_flash_cmd_byte(__NAND_FLASH_READ_DATA_ADDR_CMD__);
		
		/* 等待数据 */
		NAND_FALSH_WAIT_READY();

		/* 读page的所有数据数据 */
		while ((offset < NAND_FLASH_PAGE_SIZE) && (index < len))
		{
			buf[index] = NAND_FLASH_READ_BYTE();
			offset++;
			index++;
		}

		/* 一个page读取完毕读取下一个Page */
		offset = 0;
		page++;
	}

	/* 禁用片选 */
	NAND_FALSH_DESELECT();
}

