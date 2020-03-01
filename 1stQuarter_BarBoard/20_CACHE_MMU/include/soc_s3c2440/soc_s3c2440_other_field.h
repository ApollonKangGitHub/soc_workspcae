#ifndef _SOC_S3C2440_OTHER_FIELD_H_
#define _SOC_S3C2440_OTHER_FIELD_H_

/* The suffix 'f' means that the macro is the bits of a field in a register. */

#define WATCH_DOG_TIMER_RESETf			(0x1 << 0)	/* 0:disable timer reset function, 1:enable timer reset function */

/* Program Status Register */
#define PSR_CPU_MODE_MASK				(0x1F)
#define PSR_CPU_MODE_USRv				(0x10)
#define PSR_CPU_MODE_FIQv				(0x11)
#define PSR_CPU_MODE_IRQv				(0x12)
#define PSR_CPU_MODE_SVCv				(0x13)
#define PSR_CPU_MODE_ABTv				(0x17)
#define PSR_CPU_MODE_UNDv				(0x1b)
#define PSR_CPU_MODE_SYSv				(0x1F)

/* SWI指令前缀 */
#define SWI_INSTRUCTION_PREFIX_MASK		(0xff000000)

/* 12MHZ */
#define CRYSTAL_OSCILLATOR_FREQUENCY	(12000000)

#endif
