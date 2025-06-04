#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void ADC_Init(void);
uint16_t ADC_Read(void);
uint16_t ADC_Read_Average(uint8_t muestras); // Nuevo prototipo

#endif



