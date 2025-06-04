// === dht11.h ===
#ifndef DHT11_H
#define DHT11_H

#include <xc.h>
#include <stdint.h>

void DHT11_Init(void);
uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity);

#endif