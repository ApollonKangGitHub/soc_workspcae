#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include <tools.h>

extern int frameBuffer_init(void);
extern int frameBuffer_set_point
(
	uint32 x, 
	uint32 y, 
	uint32 color
);

#endif	/* _FRAME_BUFFER_H_ */
