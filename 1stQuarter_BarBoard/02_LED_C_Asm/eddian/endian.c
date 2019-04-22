/*
 * Time:2019年4月3日21:21:46
 * Author:kangruojin
 * Mail:mailbox_krj@163.com
 */
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

void delay(int sec)
{
	int i = 0, j = 0;
	while(sec--){
		for(i = 0; i < 256; i++){
			for(j = 0; j < 256; j++){
				//do nothing;
			}
		}
	}
}
void __light_led(int choose)
{
	unsigned int * pGpfCon = (unsigned int * )GPFCON_ADDR;
	unsigned int * pGpfDat = (unsigned int * )GPFDAT_ADDR;
	
	/* 配置GPF4~GPF6为输出引脚 */
	GPFCON_GPF4_CONFIG(*pGpfCon, __GPFCON_GPF_OUTPUT);	
	GPFCON_GPF5_CONFIG(*pGpfCon, __GPFCON_GPF_OUTPUT);
	GPFCON_GPF6_CONFIG(*pGpfCon, __GPFCON_GPF_OUTPUT);
	
	/* 先全部灭掉 */
	GPFDAT_GPFn_SET_BIT(*pGpfDat, 4);
	GPFDAT_GPFn_SET_BIT(*pGpfDat, 5);
	GPFDAT_GPFn_SET_BIT(*pGpfDat, 6);
	
	while(1){
		switch(choose)
		{
			case 4:
				/* GPF4的灯一直闪 */
				GPFDAT_GPFn_CLI_BIT(*pGpfDat, 4);
				delay(1);
				GPFDAT_GPFn_SET_BIT(*pGpfDat, 4);
				delay(1);
			break;
			case 5:
				/* GPF5的灯一直闪 */
				GPFDAT_GPFn_CLI_BIT(*pGpfDat, 5);
				delay(1);
				GPFDAT_GPFn_SET_BIT(*pGpfDat, 5);
				delay(1);
			break;
			case 6:
				/* GPF6的灯一直闪 */
				GPFDAT_GPFn_CLI_BIT(*pGpfDat, 6);
				delay(1);
				GPFDAT_GPFn_SET_BIT(*pGpfDat, 6);
				delay(1);
			break;
			default:
			break;
		}
	}
}
int main(void)
{
	unsigned int a = 0x12345678;
	unsigned char *p = (unsigned char *)&a;

	if(0x12 == p[0]){
		__light_led(4);		//大端,GPF4的灯一直闪
	}
	else if(0x78 == p[0]){
		__light_led(5);		//小端,GPF5的灯一直闪
	}
	else{
		__light_led(6);		//异常,GPF6的灯一直闪
	}
	
	return 0;
}
