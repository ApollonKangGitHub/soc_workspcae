#ifndef _SOC_S3C2440_REG_OPERATOR_
#define _SOC_S3C2440_REG_OPERATOR_

#include <soc_s3c2440_reg.h>
#include <soc_s3c2440_field.h>

#define __GET_BIT(sValue, bitPos)		(((sValue) >> (bitPos)) & (0x01))
#define __SET_BIT(sValue, bitPos, setBit)	((setBit) ? ((sValue) |= ((0x01) << (bitPos))) : ((sValue) &= ~((0x01) << (bitPos))))

#define _TEST_BIT_IS_ZERO(sValue, bitPos)	(__GET_BIT(sValue, bitPos) ? (0) : (1))
#define _TEST_BIT_IS_ONE(sValue, bitPos)	(__GET_BIT(sValue, bitPos) ? (1) : (0))

#endif
