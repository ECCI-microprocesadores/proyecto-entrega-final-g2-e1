#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include <stdint.h>

void PWM_Init(void);
void PWM_SetDutyCycle(uint8_t duty); // duty de 0 a 100

#endif


