#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include <tools.h>

#define AT24CXX_DEVICE_ADDR		(0x50)
#define AT24CXX_WORD_ADDR_LEN	(1)

extern int at24cxx_read_random(uint8 offset, uint8 * data, int len);
extern int at24cxx_write_bytes(uint8 offset, uint8 * data, int len);

#endif	/* _AT24CXX_H_ */
