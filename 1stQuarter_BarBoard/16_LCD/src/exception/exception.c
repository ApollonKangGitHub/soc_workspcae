/*
 * exception.c
 * Description:用于控制异常处理
 * Time：2020年2月7日23:45:19
 * Author：Kangruojin
 * Version：V1.0
*/

#include <exception.h>
#include <tools.h>
#include <soc_s3c2440_public.h>

char * exception_cpuModeGet(uint32 psr)
{
	switch(psr & PSR_CPU_MODE_MASK)
	{
		case PSR_CPU_MODE_USRv:
			return "User";
		case PSR_CPU_MODE_FIQv:
			return "FIQ[Fast interrupt request]";
		case PSR_CPU_MODE_IRQv:
			return "IRQ[Interrupt request]";
		case PSR_CPU_MODE_SVCv:
			return "Supervisor";
		case PSR_CPU_MODE_ABTv:
			return "Abort";
		case PSR_CPU_MODE_UNDv:
			return "Undefined";
		case PSR_CPU_MODE_SYSv:
			return "System";
		default:
			return "Unknown";
	}

	return "Unknown";
}

/* 异常信息打印 */
void exception_printException(uint32 cpsr, uint32 spsr, char *str)
{
	char * pExceptionCpuMod = "Unknown";
	char * pOriginalCpuMod  = "Unknown";

	print_screen("\r\n---------------------------------------------------------------------------");
	print_screen("\r\n Exception!! CPSR:%x, SPSR:%x.", cpsr, spsr);
	
	pExceptionCpuMod = exception_cpuModeGet(cpsr);
	pOriginalCpuMod = exception_cpuModeGet(spsr);
		
	print_screen("\r\nException Mode:%s", pExceptionCpuMod);
	print_screen("\r\nOriginal  Mode:%s", pOriginalCpuMod);
	print_screen("\r\n %s", str);
	print_screen("\r\n---------------------------------------------------------------------------");
}

/* swi异常类型（值）打印 */
void exception_printSwiValue(uint32 * swi)
{
	uint32 val = *swi & ~SWI_INSTRUCTION_PREFIX_MASK;
	print_screen("\r\nswi(svc) instruction addr:%x, value is [%x-%x]\r\n", swi, *swi, val);
}

/* 打印未定义指令异常地址和指令 */
void exception_printUndefinedException(uint32 * addr, uint32 * addr2)
{
	print_screen("\r\nUndefined instruction addr:%x, instruction is [%x]", addr, *addr);
	if (0XE12FFF33 == *addr)
	{
		print_screen("\r\n Undefined instruction blx R3, R3[%x], R3 instruction[%x]\r\n", addr2, *addr2);
	}
}