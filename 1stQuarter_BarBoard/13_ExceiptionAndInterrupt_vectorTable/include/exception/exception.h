#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <tools.h>

extern void exception_printException(uint32 cpsr,uint32 spsr, char *str);
extern void exception_printSwiValue(uint32 * swi);
#endif /* _EXCEPTION_H_ */