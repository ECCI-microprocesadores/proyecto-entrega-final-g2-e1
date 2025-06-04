#include <xc.h>
#include <stdint.h>
#include "adc.h"
#define _XTAL_FREQ 16000000UL
void ADC_Init(void) {
    ANSELA = 0x01;     // RA0 como analógico
    TRISA0 = 1;        // RA0 como entrada
    ADCON0 = 0x01;     // Canal AN0, ADC encendido
    ADCON1 = 0x0E;     // AN0 analógico, los demás digitales
    ADCON2 = 0xA9;     // Justificación derecha, Fosc/8, Tacq = 4TAD
}

uint16_t ADC_Read(void) {
    ADCON0bits.GO = 1;              // Inicia la conversión
    while (ADCON0bits.GO);          // Espera que termine
    return ((uint16_t)(ADRESH << 8) | ADRESL); // Resultado de 10 bits
}

uint16_t ADC_Read_Average(uint8_t muestras) {
    uint32_t suma = 0;
    for (uint8_t i = 0; i < muestras; i++) {
        suma += ADC_Read();
        __delay_ms(5); // Pequeño retardo entre lecturas
    }
    return (uint16_t)(suma / muestras);
}



