
/*
 * weidongshan@qq.com, www.100ask.net
 *
 */

#include <common.h>
#include <command.h>
#include <def.h>
#include <nand.h>
#include <s3c24x0.h>

extern void s3c2440_cpu_suspend(void);

static void delay(volatile int d)
{
	while(d--);
}

int do_suspend (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	/* ����: */
	
	/* 1. ����GPIO: ������ά��LED������, ���ڻ���CPU������Ҫ��Ϊ�жϹ��� */
	/* ����NAND����: Ҫ����EINT23,22,21Ϊ�������� */
	rGPGCON &= ~((3<<30) | (3<<28) | (3<<26));

	/* JZ2440ֻ��S2/S3/S4����������Դ,�������Ƕ�Ӧ��GPIO�����ж�ģʽ */
	rGPFCON &= ~((3<<0) | (3<<4));
	rGPFCON |= ((2<<0) | (2<<4));

	rGPGCON &= ~(3<<6);
	rGPGCON |= (2<<6);

	/* 2. ����INTMSK���������ж�: ��sleepģʽ��,��Щ����ֻ�����ڻ���ϵͳ,��CPU��������ʱ������������INTMSK����Щ���������жϹ��� */	
	rINTMSK = ~0;

	/* 3. ���û���Դ */
	rEXTINT0 |= (6<<0) | (6<<8); /* EINT0,2˫���ش��� */
	rEXTINT1 |= (6<<12);   /* EINT11˫���ش��� */

	/* 4. ����MISCCR[13:12]=11b, ʹ��USBģ��������� */
	rMISCCR |= (3<<12);

	/* 5. ��GSTATUS[4:3]����ĳֵ, ���ǿ�����ϵͳ������ʱʹ�� */
	//rGSTATUS3 = ;  /* ����ʱ����ִ�еĺ����ĵ�ַ */
	//rGSTATUS4 = ;  /*  */

	/* 6. ���� MISCCR[1:0] ʹ���������ߵ��������� */
	rMISCCR &= ~(3);

	/* 7. ��� LCDCON1.ENVID ��ֹͣLCD */
	rLCDCON1 &= ~1;

	/* 8~12ʹ�û����ʵ��,�ο��ں�Դ��:
	 *    arch\arm\mach-s3c2410\sleep.S
	*/

	/* 8. ����2���Ĵ���: rREFRESH and rCLKCON, �Ա����TLB
	 *    �����ʹ��MMU�Ļ�,���Ŀ�Ŀ��Ժ��� 
	 */

	/* 9. ���� REFRESH[22]=1b,��SDRAM����self-refresh mode */

	/* 10. �ȴ�SDRAM�ɹ�����self-refresh mode  */

	/* 11.���� MISCCR[19:17]=111b�Ա���SDRAM�ź�(SCLK0,SCLK1 and SCKE) */

	/* 12. ����CLKCON��SLEEPλ��ϵͳ����sleep mode */	
	printf("suspend ...");
	delay(1000000);
	s3c2440_cpu_suspend();  /* ִ�е�����Ͳ��᷵��,ֱ��CPU������ */

	/* �ָ�����: ���³�ʼ��Ӳ�� */
	serial_init();
	printf("wake up\n");
	
	
    return 0;
}


U_BOOT_CMD(
	suspend,	1,	0,	do_suspend,
	"suspend - suspend the board\n",
	" - suspend the board"
);


