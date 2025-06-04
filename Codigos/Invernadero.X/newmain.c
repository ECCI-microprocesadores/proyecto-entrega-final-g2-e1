#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "dht11.h"
#include "pwm.h"

// CONFIGURACION DEL PIC
#pragma config FOSC = INTIO67
#pragma config PLLCFG = OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRTEN = ON
#pragma config BOREN = OFF
#pragma config WDTEN = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 16000000UL
#define VENTILADOR LATCbits.LATC0
#define LED_VENTILADOR LATCbits.LATC1
#define LED_BOMBA LATCbits.LATC3

void main(void) {
    OSCCON = 0b01110000;       // Oscilador interno a 16 MHz
    UART_Init();               // Inicializa UART
    ADC_Init();                // Inicializa ADC (para LDR)
    DHT11_Init();              // Inicializa DHT11 (RA1)
    PWM_Init();                // Inicializa PWM (RC2) para bomba de agua

    TRISC0 = 0;                // RC0 como salida para el ventilador
    TRISC1 = 0;                // RC1 como salida para LED del ventilador
    TRISC3 = 0;                // RC3 como salida para LED de la bomba

    char buffer[64];
    uint8_t contador = 0;
    uint8_t temp, hum;

    while (1) {
        uint16_t adc_val = ADC_Read_Average(10);          // Lectura de LDR
        float volt = (adc_val * 5.0f) / 1023.0f;           // Convertir a voltaje (V)

        if (DHT11_Read(&temp, &hum)) {
            // Control del ventilador
            if (temp > 25) {
                VENTILADOR = 1;
                LED_VENTILADOR = 1;
            } else {
                VENTILADOR = 0;
                LED_VENTILADOR = 0;
            }

            // Control por duty cycle de la bomba de agua según humedad
            if (hum < 30) {
                PWM_SetDutyCycle(100); // riego máximo
                LED_BOMBA = 1;
            } else if (hum < 40) {
                PWM_SetDutyCycle(55);  // riego medio
                LED_BOMBA = 1;
            } else if (hum < 50) {
                PWM_SetDutyCycle(60);  // riego leve
                LED_BOMBA = 1;
            } else {
                PWM_SetDutyCycle(0);   // apagado
                LED_BOMBA = 0;
            }

            // Clasificación de luz ambiente
            const char* luz_estado;
            if (volt <= 0.5f) {
                luz_estado = "Noche";
            } else if (volt <= 1.0f) {
                luz_estado = "Nublado";
            } else {
                luz_estado = "Soleado";
            }

            // Mostrar valores por UART cada 10 ciclos
            if (contador >= 10) {
                sprintf(buffer, "%s | T: %uC | H: %u%%\r\n", luz_estado, temp, hum);
                UART_WriteString(buffer);
                contador = 0;
            } else {
                contador++;
            }
        }

        __delay_ms(500);
    }
}
