#include <uart.h>
#include <tools.h>
#include <nor_flash.h>
#include <log.h>

/*
 * 如果：在nor flash测试时同时打开了定时器中断的测试用例，CPU可能挂死
 * 原因是读取NOR厂商信息以及CFI信息等信息时不处于reset模式，
 * 此时NOR中的数据也不是reset模式的数据，此时如果产生中断，
 * CPU跳转到NorFlash0地址开始的位置取中断向量表的指令
 * 而NOR中的0开始的数据并不是实际的中断向量表，CPU可能会因为指令错误而卡死
 */
void nor_flash_info_display(void)
{
	char qryStr[4] = {'0'};
	uint16 vccmin = 0;
	uint16 vccmax = 0;
	uint32 size = 0;
	uint32 gb_bytes = 0;
	uint32 mb_bytes = 0;
	uint32 kb_bytes = 0;
	uint32 bytes = 0;
	uint16 regionNum = 0;
	uint16 regionInfoBase = 0x0;
	uint16 blockNum = 0;
	uint32 blockSize = 0;
	uint32 blockIndex = 0;
	uint32 regionIndex = 0;
	uint32 blockAddr = __NOR_FLASH_BASE__;
	uint32 blkCnt = 0;
	uint16 venId = 0x0;
	uint16 devId = 0x0;
	
	print_screen("\r\n --NOR FLASH VENDOR INFO AND WORKING PARAMETERS:");
	print_screen("\r\n -------------------------------------------------------------");

	/* 进入ID读取模式 */
	NOR_FLASH_CMD(__NOT_FLASH_ADDR_1st_BUS_CYCLE__, __NOR_FLASH_CMD_1st_BUS_CYCLE_UNLOCK__);
	NOR_FLASH_CMD(__NOT_FLASH_ADDR_2nd_BUS_CYCLE__, __NOR_FLASH_CMD_2nd_BUS_CYCLE_UNLOCK__);
	NOR_FLASH_CMD(__NOT_FLASH_ADDR_3rd_BUS_CYCLE__, __NOR_FLASH_CMD_3rd_BUS_CYCLE_ID__);

	/* 读取厂商ID和设备ID */
	venId = NOR_FLASH_DATA(__NOR_FLASH_VENDOR_ID_OFFSET);
	devId = NOR_FLASH_DATA(__NOR_FLASH_DEVICE_ID_OFFSET);
	
	print_screen("\r\n Manufacturer code : %x.", venId);
	print_screen("\r\n Device code       : %x.", devId);

	/* 退出ID读取模式(reset) */
	NOR_FLASH_RESUME_DEFAULT_MODE;

	/* 进入CFI模式 */
	NOR_FLASH_CMD(__NOT_FLASH_ADDR_1st_BUS_CYCLE__, __NOR_FLASH_CMD_1st_BUS_CYCLE_CFI__);

	/* 取出"QRY" */
	qryStr[0] = NOR_FLASH_DATA(__NOR_FLASH_QRY_OFFSET__ + 0);
	qryStr[1] = NOR_FLASH_DATA(__NOR_FLASH_QRY_OFFSET__ + 1);
	qryStr[2] = NOR_FLASH_DATA(__NOR_FLASH_QRY_OFFSET__ + 2);
	qryStr[3] = '\0';

	print_screen("\r\n Query-unique ASCII qryString : %s.", qryStr);

	/* 取出电压范围，浮点数，低4bit为小数位，其他bit为整数位 */
	vccmin = NOR_FLASH_DATA(__NOR_FLASH_VCC_MIN_OFFSET__);
	vccmax = NOR_FLASH_DATA(__NOR_FLASH_VCC_MAX_OFFSET__);
	
	print_screen("\r\n VCC supply range[%d.%dV ~ %d.%dV].", 
		vccmin >> 4, (vccmin >> 0) & 0x0F, vccmax >> 4, (vccmax >> 0) & 0x0F);

	/* 取出容量大小 */
	size = (uint32)NOR_FLASH_DATA(__NOR_FLASH_DEV_SIZE_OFFSET__);	
	size = (2 << size);
	tool_calc_mem_size((uint32)size, &gb_bytes, &mb_bytes, &kb_bytes, &bytes);
	
	print_screen("\r\n Size[%x]:%d[GB]-%d[MB]-%d[KB]-%d[Bytes].", 
		size, gb_bytes, mb_bytes, kb_bytes, bytes);

	/*
	 * 打印各扇区起始地址
	 * 名词解释：
	 * 1. Erase block region：一个region含有一个/多个block，其大小一致
	 * 即一个NOR FLASH中有一个/多个region，不同region区分不同大小的block
     *
	 * 2. Erase block region information (refer to the CFI publication 100)
	 * 共4个Bytes:
	 * 前两个Bytes值 + 1		  : 表示该region有多少个bolck
	 * 后两个Bytes值 * 256	   : 表示该region中block的大小
	 */
	regionNum = NOR_FLASH_DATA(__NOR_FLASH_REGION_NUM_OFFSET__);
	print_screen("\r\n Region number is %d.", regionNum);

	/* 读取每个region的信息 */
	print_screen("\r\n Block/Sector start Address information:");
	regionInfoBase = __NOR_FLASH_REGION_INFO_OFFSET__;
	for (regionIndex = 0; regionIndex < regionNum; regionIndex++)
	{

		/* 
		 * ARM架构注意字节序调整，此外这里四字节必须按照各自的地址读取，
		 * 不能认为一次读取一个word，这个word就是连续两个地址各自的值
		 * 每个地址读出来的是两个字节，但是只有低八位有效
		 */
		blockNum = 1 + NOR_FLASH_DATA(regionInfoBase) + (NOR_FLASH_DATA(regionInfoBase+1) << 8);
		blockSize = 256 * (NOR_FLASH_DATA(regionInfoBase+2) + (NOR_FLASH_DATA(regionInfoBase+3) << 8));
#if 0
		blkCnt = 0;
		print_screen("\r\n Region[%d], baseAddr:%x, blockNum:%d, blockSize:%d[B], Block start addr:", \
			regionIndex, blockAddr, blockNum, blockSize);
#endif
		/* 打印每个block的起始地址 */
		for (blockIndex = 0; blockIndex < blockNum; blockIndex++)
		{
			if (blkCnt % 5 == 0)
			{
				print_screen("\r\n");
			}
			print_screen(" %x ", blockAddr);
			blockAddr += blockSize;
			blkCnt++;
		}
		
		regionInfoBase += 4;
	}
	
	/* 退出CFI模式(reset) */
	NOR_FLASH_RESUME_DEFAULT_MODE;
}

void nor_flash_read_multi(uint8 * buf, int len, uint32 * addr)
{
	volatile uint8 * pAddr = 0x0;
	int cnt = 0;
	
	for (pAddr = (volatile uint8 * )addr; cnt < len; cnt++)
	{
		buf[cnt] = *pAddr;
		pAddr++;
	}
}


