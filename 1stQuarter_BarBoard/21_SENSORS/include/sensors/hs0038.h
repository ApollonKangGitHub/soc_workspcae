#ifndef _HS0038_H_
#define _HS0038_H_

#include <tools.h>
#include <necDataQueue.h>

extern void hs0038_init(void);
extern void hs0038_detach(void);
extern void hs0038_raw_data_dump(void);
extern necReadRet_t hs0038_read_nec_data(uint8 * necAddr, uint8 * necData);

#endif	/* _HS0038_H_ */