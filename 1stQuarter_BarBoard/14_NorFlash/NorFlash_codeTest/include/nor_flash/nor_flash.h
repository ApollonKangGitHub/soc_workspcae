#ifndef _NOR_FLASH_H_
#define _NOR_FLASH_H_

#include <tools.h>

#define NOR_FLASH_MAX_ADDR					(1 << 21)

/* NOR接在cs0上， base_addr = 0 */
#define __NOR_FLASH_BASE__					(0x0)	

/* NOR FLASH 的ADDR A0管脚与CPU的A1管脚相连，即CPU操作NOR时地址需要左移1位 */
#define __NOR_FLASH_A0_OFFSET_CPU__			(1)		


/* 公共命令地址 */
#define __NOT_FLASH_ADDR_1st_BUS_CYCLE__			(0x555)
#define __NOT_FLASH_ADDR_2nd_BUS_CYCLE__			(0x2AA)
#define __NOT_FLASH_ADDR_3rd_BUS_CYCLE__			(0x555)

/* 发送解锁命令值 */
#define __NOR_FLASH_CMD_1st_BUS_CYCLE_UNLOCK__		(0xAA)
#define __NOR_FLASH_CMD_2nd_BUS_CYCLE_UNLOCK__		(0x55)

/* 发送读取芯片ID命令值 */
#define __NOR_FLASH_CMD_3rd_BUS_CYCLE_ID__			(0x90)
/* 发送CFI模式命令 */
#define __NOR_FLASH_CMD_1st_BUS_CYCLE_CFI__			(0x98)

/* 厂商ID和设备IDoffset */
#define __NOR_FLASH_VENDOR_ID_OFFSET		(0)
#define __NOR_FLASH_DEVICE_ID_OFFSET		(1)

/* Query-unique ASCII string "QRY" offset */
#define __NOR_FLASH_QRY_OFFSET__			(0x10)

/* CFI的部分信息offset */
#define __NOR_FLASH_VCC_MIN_OFFSET__		(0x1B)
#define __NOR_FLASH_VCC_MAX_OFFSET__		(0x1C)
#define __NOR_FLASH_DEV_SIZE_OFFSET__		(0x27)
#define __NOR_FLASH_REGION_NUM_OFFSET__		(0x2C)
#define __NOR_FLASH_REGION_INFO_OFFSET__	(0x2D)

#define NOR_FLASH_WRITE_WORD(base, offset, val) \
	do { \
		volatile uint16 * pAddr = (volatile uint16 * )(((base) + (offset)) << __NOR_FLASH_A0_OFFSET_CPU__); \
		/* 该语句可能会被编译器拆分成两个strb指令，而非一个strh指令，导致volatile的效果达不到 */ \
		/* 通过增加编译选项 -march=armv4 来解决该编译非预期化问题 */ \
		*pAddr = (uint16)(val);	 \
	}while(0)
	
#define NOR_FLASH_READ_WORD(base, offset) \
	(*(volatile uint16 * )(((base) + (offset)) << __NOR_FLASH_A0_OFFSET_CPU__))

/* 发送nor flash命令 */
#define NOR_FLASH_CMD(offset, val) 	NOR_FLASH_WRITE_WORD(__NOR_FLASH_BASE__, (offset), (val))

/* 读取nor flash数据 */
#define NOR_FLASH_DATA(offset) 		NOR_FLASH_READ_WORD(__NOR_FLASH_BASE__, offset)

/* 从其他命令模式回复到默认（reset）模式，只需要在任意地址写入0xF0即可 */
#define NOR_FLASH_RESUME_DEFAULT_MODE	NOR_FLASH_CMD(0x0, 0xF0)

extern void nor_flash_info_display(void);
extern void nor_flash_read_multi(uint8 * buf, int len, uint32 * addr);

#endif	/* _NOR_FLASH_H_ */
