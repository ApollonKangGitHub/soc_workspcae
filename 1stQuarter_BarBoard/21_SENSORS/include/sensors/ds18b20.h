#ifndef _DS18B20_H_
#define _DS18B20_H_

#include <tools.h>

extern int ds18b20_read_rom(uint8 * buf, uint32* len);
extern int ds18b20_read_temperature(double * temperature);
extern void ds18b20_init(void);

#endif	/* _DS18B20_H_ */