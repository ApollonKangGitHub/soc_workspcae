#ifndef _SOC_S3C2440_REGISTER_OPERATOR_
#define _SOC_S3C2440_REGISTER_OPERATOR_

#include <tools.h>

/* 获取指定寄存器一个bit */
#define SOC_S3C2440_REG_BIT_GET(reg, bit) 	((reg >> bit) & 0x1)

/* 设置指定寄存器一个bit */
#define SOC_S3C2440_REG_BIT_SET(reg, bit, value) \
	((reg) &= ~(0x1 << (bit)), (reg) |= (((value) << (bit)) & 0x1))

/* 设置指定寄存器多个bit */
#define SOC_S3C2440_REG_BITS_SET(reg, fieldstart, mask, value) \
	((reg) &= ~(mask), (reg) |= (((value) << (fieldstart)) & (mask)))\

#endif	/* _SOC_S3C2440_REGISTER_OPERATOR_ */
