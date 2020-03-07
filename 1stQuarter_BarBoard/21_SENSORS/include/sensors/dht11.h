#ifndef _DHT11_H_
#define _DHT11_H_

extern int dht11_read(uint32 * humidity, uint32 * temperature);
extern void dht11_init(void);

#endif	/* _DHT11_H_ */