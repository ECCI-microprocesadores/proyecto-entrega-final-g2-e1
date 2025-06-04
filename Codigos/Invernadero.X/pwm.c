#include "pwm.h"

void PWM_Init(void) {
    TRISC2 = 0; // RC2 como salida (CCP1)
    PR2 = 249;  // PWM a 50 Hz con Fosc = 16 MHz y prescaler 16
    CCP1CON = 0x0C; // PWM mode
    T2CON = 0x06;   // Timer2 ON, prescaler 16
}

void PWM_SetDutyCycle(uint8_t duty) {
    if (duty > 100) duty = 100;
    uint16_t pwm_val = (duty * (PR2 + 1)) / 100;
    CCPR1L = pwm_val >> 2;
    CCP1CON = (CCP1CON & 0xCF) | ((pwm_val & 0x03) << 4);
}

