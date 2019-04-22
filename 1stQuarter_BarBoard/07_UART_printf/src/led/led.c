/*
 * Description:
 *     三个按键控制流水灯的延时时间，有增加、减少、回复默认时间三种操作
 *     start.s中存在关闭看门狗和自动选择Nand启动/Nor启动并设置栈的操作
 * Time:2019年4月5日00:18:02
 * Author:Kangruojin
 * Version:1.2
*/

#include <led.h>
#include <soc_s3c2440_public.h>

void led_delay(volatile int time)
{
	while(time--);
}

void led_init()
{
	/* 清空GPF0、GPF2、GPF4~GPF6、GPG3的控制信息,同时设置了GPF0、GPF2、GPG3为输入引脚 */
	GPFCONr &= ~(GPFCON_GPF0_CONF_BITSf | GPFCON_GPF2_CONF_BITSf | GPFCON_GPF4_CONF_BITSf | GPFCON_GPF5_CONF_BITSf | GPFCON_GPF6_CONF_BITSf);
	GPGCONr &= ~(GPGCON_GPG3_CONF_BITSf);

	/* 配置GPF0、GPF2为输入引脚,GPG3为输入引脚,00;配置GPF4~GPF6为输出引脚 */
	GPFCONr |= ((GPFCON_OUTPUT << GPFCON_GPF4_CONF_START) | (GPFCON_OUTPUT << GPFCON_GPF5_CONF_START) | (GPFCON_OUTPUT << GPFCON_GPF6_CONF_START));
	/* 配置GPF0、GPF2、GPG3为输入引脚，上面清空过后，相当于已经配置为输入了，不需要再配一次 */
}

void led_running()
{
	/*
	 * 注意，此处的临时变量是实时更新的，所以防止CPU做优化，加上volatile，防止被不期望的优化
	 * CPU可能最开始还在取寄存器的值，到后来取了几次发现没变化就不取了，所以被坑了很久
	 */
	volatile unsigned int gpfTemp=0;
	volatile unsigned int gpgTemp=0;

	int delay_time = DEFAULT_DELAY_TIME; 
	unsigned int i = 0;
	unsigned int len = 0;
	const unsigned char led_info[] = {
		0x00, 0x40, 0x60, 0x70, 0x30, 0x10, 0x00, 0x70,
		0x00, 0x70, 0x00, 0x70, 0x00, 0x10, 0x30, 0x70, 
		0x60, 0x40, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70
	};
	len = sizeof(led_info) / sizeof(led_info[0]);

	/* 设置led为流水灯点亮 */
	while(1)
	{
		for(i = 0; i < len; i++)
		{
			gpfTemp = GPFDATr;
			gpgTemp = GPGDATr;

			/* 默认是高电平，按下按键为低电平 */
			if(_TEST_BIT_IS_ZERO(gpfTemp, 0)){
				if(delay_time < DELAY_TIME_MAX){			/* 上限 */
					delay_time += DEFAULT_ADD_DEALY_TIME;	/* 按键EINT0为增加延时 */
				}
			}
			if(_TEST_BIT_IS_ZERO(gpfTemp, 2)){
				if(delay_time > DELAY_TIME_MIN){			/* 下限 */
					delay_time -= DEFAULT_INC_DEALY_TIME;	/* 按键EINT2为减少延时 */
				}
			}
			if(_TEST_BIT_IS_ZERO(gpgTemp, 3)){
				delay_time = DEFAULT_DELAY_TIME;			/* 按键EINT11为回复默认延时 */
			}

			/* 
			 * 要点：不要用或运算直接给这3bit赋值，否则经过几次，这3bit就全被置为1了，不会再改变
			 * 非要或运算的话，每次需要清空，接着立即去或运算， 	 但是灯的切换过程，总会有点闪烁不定
			 * 所以，直接去获取新的状态，然后根据获取的每一位状态设置新的每一位状态即可。
			 */
			__SET_BIT(GPFDATr, 4, __GET_BIT(led_info[i], 4));
			__SET_BIT(GPFDATr, 5, __GET_BIT(led_info[i], 5));
			__SET_BIT(GPFDATr, 6, __GET_BIT(led_info[i], 6));

			led_delay(delay_time);
		}
	}
}