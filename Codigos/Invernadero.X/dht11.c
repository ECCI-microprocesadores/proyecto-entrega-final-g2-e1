// === dht11.c ===
#include "dht11.h"
#define DHT_PIN PORTAbits.RA1
#define DHT_DIR TRISAbits.TRISA1
#define _XTAL_FREQ 16000000UL
void DHT11_Start(void) {
    DHT_DIR = 0;
    LATAbits.LATA1 = 0;
    __delay_ms(20);
    LATAbits.LATA1 = 1;
    DHT_DIR = 1;
    __delay_us(40);
}

uint8_t DHT11_CheckResponse(void) {
    uint8_t response = 0;
    __delay_us(40);
    if (!DHT_PIN) {
        __delay_us(80);
        if (DHT_PIN) response = 1;
        __delay_us(40);
    }
    return response;
}

uint8_t DHT11_ReadByte(void) {
    uint8_t i, byte = 0;
    for (i = 0; i < 8; i++) {
        while (!DHT_PIN);
        __delay_us(30);
        if (DHT_PIN) byte |= (1 << (7 - i));
        while (DHT_PIN);
    }
    return byte;
}

uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity) {
    uint8_t rh_int, rh_dec, t_int, t_dec, checksum;

    DHT11_Start();
    if (DHT11_CheckResponse()) {
        rh_int = DHT11_ReadByte();
        rh_dec = DHT11_ReadByte();
        t_int  = DHT11_ReadByte();
        t_dec  = DHT11_ReadByte();
        checksum = DHT11_ReadByte();

        if (checksum == (rh_int + rh_dec + t_int + t_dec)) {
            *temperature = t_int;
            *humidity = rh_int;
            return 1;
        }
    }
    return 0;
}

void DHT11_Init(void) {
    DHT_DIR = 1;
}

