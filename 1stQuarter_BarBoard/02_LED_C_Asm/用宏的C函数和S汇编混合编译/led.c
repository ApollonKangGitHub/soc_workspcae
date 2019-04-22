#define GPFCON_ADDR 				(0x56000050)
#define GPFDAT_ADDR 				(0x56000054)

#define __GPFCON_GPF_INPUT			(0x00)
#define __GPFCON_GPF_OUTPUT			(0x01)
#define __GPFCON_GPF_EINT			(0x10)
#define __GPFCON_GPF_REVERSE		(0x11)

#define _GPFCON_GPFn_CONFIG(shift, choose) 			 	((choose) << (shift))	

#define GPFCON_GPF0_CONFIG(value, choose)				((value) |= _GPFCON_GPFn_CONFIG(0, (choose)))
#define GPFCON_GPF1_CONFIG(value, choose)				((value) |= _GPFCON_GPFn_CONFIG(2, (choose)))
#define GPFCON_GPF2_CONFIG(value, choose)				((value) |= _GPFCON_GPFn_CONFIG(4, (choose)))
#define GPFCON_GPF3_CONFIG(value, choose)				((value) |= _GPFCON_GPFn_CONFIG(6, (choose)))
#define GPFCON_GPF4_CONFIG(value, choose)				((value) |= _GPFCON_GPFn_CONFIG(8, (choose)))
#define GPFCON_GPF5_CONFIG(value, choose)				((value) |= _GPFCON_GPFn_CONFIG(10, (choose)))
#define GPFCON_GPF6_CONFIG(value, choose)				((value) |= _GPFCON_GPFn_CONFIG(12, (choose)))
#define GPFCON_GPF7_CONFIG(value, choose)				((value) |= _GPFCON_GPFn_CONFIG(14, (choose)))

#define GPFDAT_GPFn_SET_BIT(value, offset)				((value) |= (0x01 << (offset)))
#define GPFDAT_GPFn_CLI_BIT(value, offset)				((value) &= ~(0x01 << (offset)))

int main(void)
{
	unsigned int *pGpfCon = (unsigned int *)GPFCON_ADDR;
	unsigned int *pGpfDat = (unsigned int *)GPFDAT_ADDR;
											
	/* 配置GPF4~GPF6为输出引脚 */
	GPFCON_GPF4_CONFIG(*pGpfCon, __GPFCON_GPF_OUTPUT);
	GPFCON_GPF5_CONFIG(*pGpfCon, __GPFCON_GPF_OUTPUT);
	GPFCON_GPF6_CONFIG(*pGpfCon, __GPFCON_GPF_OUTPUT);
	
	/* 点亮所有的三个led灯 */
	GPFDAT_GPFn_CLI_BIT(*pGpfDat, 4);	
	GPFDAT_GPFn_SET_BIT(*pGpfDat, 5);
	GPFDAT_GPFn_CLI_BIT(*pGpfDat, 6);	

	return 0;
}
