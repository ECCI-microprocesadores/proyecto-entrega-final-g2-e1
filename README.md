[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=19652994&assignment_repo_type=AssignmentRepo)
# Proyecto final

## Integrantes

Sofy Parra

Carlos Sierra


## Nombre del proyecto:

### Invernadero con control automatico de temperatura, humedad y luminocidad

## implementacion:

### Prototipo

Se realizo un prototipo miniatura para simular el entorno de un invernadero y acercar los datos a la mas real posible.

![implementacion](https://github.com/ECCI-microprocesadores/proyecto-entrega-final-g2-e1/blob/fc6570c32afeaec9a5d922fe0b30306a31bc802b/Imagenes/implementacion.jpeg)

![implementacion1](https://github.com/ECCI-microprocesadores/proyecto-entrega-final-g2-e1/blob/fc6570c32afeaec9a5d922fe0b30306a31bc802b/Imagenes/implementacion1.jpeg)

### Datos obtenidos

Al obtener los datos de la temperatura con ayuda del graficador serial de python se obtubo grafica en tiempo real de la temperatura.

![python](https://github.com/ECCI-microprocesadores/proyecto-entrega-final-g2-e1/blob/fc6570c32afeaec9a5d922fe0b30306a31bc802b/Imagenes/python.png)

Utilizamos un software serial que nos permitiera visualzar los datos enviados por el microcontrolador en el PC

![serial](https://github.com/ECCI-microprocesadores/proyecto-entrega-final-g2-e1/blob/fc6570c32afeaec9a5d922fe0b30306a31bc802b/Imagenes/serial.png)

## Documentación

### Resumen de funcionamiento

Para la implementacion de este proyecto se penso en la automatizacion de un invernadero cuyas caracteristicas se explicaran a continuacion: 

#### 1. Sensado del Ambiente
El sistema recoge datos del entorno utilizando dos tipos de sensores:

- LDR (fotoresistencia):
        Mide la intensidad de luz ambiental, el valor se convierte a voltaje para poder visualizarse por UART y dependiendo del voltaje suministrado al ADC se concluye el estado del dia (soleado, nublado o noche).

- DHT11:
        Sensor digital que mide temperatura y humedad relativa del aire, se conecta a RA1 y se lee periódicamente. segun la toma de valores se toman decisiones sobre el ya sea bajar temperatura por medio del ventilador o realizar riego del cultivo si la humedad no es la correcta por medio de la bomba de agua.

#### 2. Control del Ventilador

- Si la temperatura medida supera los 25°C, el sistema enciende el ventilador por medio de el pin (RC0), paralelamente enciende un LED indicador (RC1) indicando que hay una alta temperatura y que el ventilador esta encendido.
-  la temperatura es 25°C o menor, se apagan ambos. Esto ayuda a mantener la temperatura del invernadero en niveles adecuados para el cultivo.

#### 3. Control de la Bomba de Agua (RC2 via PWM)
Dependiendo del valor de humedad relativa, el sistema ajusta el nivel de riego por medio del control del duty cicle del PWM hacia la bomba de agua del sistema de riego de la sigueinte forma:

| Humedad (%) | Acción                        | PWM (%) | LED Bomba (RC3) |
|-------------|-------------------------------|---------|------------------|
| > 30        | Riego máximo                  | 100%    | Encendido        |
| 30–39       | Riego medio                   | 75%     | Encendido        |
| 40–49       | Riego leve (debería ser menor)| 60%     | Encendido        |
| ≥ 50        | Sin riego                     | 0%      | Apagado          |

Esto mantiene una humedad adecuada para el cultivo generando un aprovechamiento optimo del recurso hidrico.

#### 4. Visualización por UART

Cada 5 segundos se envían los siguientes datos por UART:
- Intensidad luminica
- Temperatura (°C)
- Humedad (%)
Esto permite monitorear en tiempo real las condiciones del invernadero desde un computador o terminal  opteniendo graficas de temperatura en tiempo real.

#### 5. Ciclo de Operación

Cada 0.5 segundos, el sistema repite el siguiente proceso:
- Lee el valor del LDR.
- Lee temperatura y humedad del DHT11.
- Activa o desactiva ventilador y bomba según condiciones.
- Si es el décimo ciclo o han pasado 5 segundos, muestra los datos por UART.

#### Resumen del funcionamiento 

| Etapa        | Descripción                                                                 |
|--------------|------------------------------------------------------------------------------|
| Sensado      | Se leen los valores del LDR (luz) y DHT11 (temperatura y humedad).          |
| Decisión     | Se evalúan condiciones para activar ventilador o bomba según umbrales.      |
| Actuación    | Se encienden/apagan ventilador, bomba de riego (PWM) y LEDs.                |
| Reporte UART | Cada 10 ciclos se envían datos por UART: luz, temperatura (°C), humedad (%) |
| Retardo      | Se espera 500 ms antes de repetir el ciclo.                                 |

## Codigo

1. Incluimos bibliotecas
Aca estamos incluyendo las librerias necesarias para el funcionamiento del codigo, incluso usamos bibliotecas ya creadas anteriormente en clase

```
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "dht11.h"
#include "pwm.h"

```
```#include <xc.h>```: Incluye los registros y definiciones específicas del microcontrolador PIC usado.

```#include <stdint.h>```: Proporciona tipos de datos como uint8_t, uint16_t, etc.

```#include <stdio.h>```: Se usa para funciones como sprintf, que convierte datos a texto.

```"uart.h"```, ```"adc.h"```, ```"dht11.h"```,``` "pwm.h"```: Son archivos personalizados para controlar UART, ADC, sensor DHT11 y PWM. Definen funciones como ```UART_Init()```, ```ADC_Read_Average()```, etc.


2. Configuración del microcontrolador
```
#pragma config FOSC = INTIO67
#pragma config PLLCFG = OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRTEN = ON
#pragma config BOREN = OFF
#pragma config WDTEN = OFF
#pragma config LVP = OFF

```
```FOSC = INTIO67```: Usa el oscilador interno y habilita los pines RA6 y RA7 como E/S digitales.

```PLLCFG = OFF```: Desactiva el PLL.

```PRICLKEN = ON```: Habilita el reloj primario.

```FCMEN = OFF```: Desactiva el monitoreo del reloj externo.

```IESO = OFF```: Desactiva el cambio automático de osciladores.

```PWRTEN = ON```: Habilita el temporizador de encendido.

```BOREN = OFF```: Desactiva el reinicio por bajo voltaje.

```WDTEN = OFF```: Desactiva el Watchdog Timer.

```LVP = OFF```: Desactiva la programación en baja tensión.

3. Definiciones y pines
```
#define _XTAL_FREQ 16000000UL
#define VENTILADOR LATCbits.LATC0
#define LED_VENTILADOR LATCbits.LATC1
#define LED_BOMBA LATCbits.LATC3

```
```_XTAL_FREQ```: Define la frecuencia del oscilador (16 MHz). Es esencial para usar __delay_ms() correctamente.

```VENTILADOR```, ```LED_VENTILADOR```, ```LED_BOMBA```: Acceso a los pines LATC0, LATC1 y LATC3.

4. Función principal main()
```
TRISC0 = 0;
TRISC1 = 0;
TRISC3 = 0;
```
Configura RC0, RC1 y RC3 como salidas digitales al asignarles el 0:

RC0 controla el ventilador.

RC1 enciende un LED cuando el ventilador está activo.

RC3 enciende un LED cuando la bomba está activa.
```
char buffer[64];
uint8_t contador = 0;
uint8_t temp, hum;
```
```buffer```: Almacena el texto a enviar por UART.

```contador```: Se usa para enviar los datos cada 10 ciclos.

```temp```, ```hum```: Variables para almacenar la temperatura y humedad leídas del DHT11.

5. Bucle principal while(1)

```
uint16_t adc_val = ADC_Read_Average(10);
```

Lee el valor del sensor de luz (LDR) usando 10 lecturas para tomar la lectura mas real.

```
float volt = (adc_val * 5.0f) / 1023.0f;
```

Convierte el valor del ADC (0–1023) a voltaje casteando al valor flotante.

```
if (DHT11_Read(&temp, &hum)) {
```

Lee los valores de temperatura y humedad del sensor DHT11.

Control del ventilador:
```
if (temp > 25) {
    VENTILADOR = 1;
    LED_VENTILADOR = 1;
} else {
    VENTILADOR = 0;
    LED_VENTILADOR = 0;
}
```
Si la temperatura supera 25 °C, activa el ventilador y enciende el LED indicando que se encuenta activo el sistema de enfriamiento.

Si no, los apaga.

Control de la bomba de agua por PWM:

```
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

```
Dependiendo del valor de humedad:

Menor que 30%: activa la bomba al 100%.

Entre 30–39%: activa la bomba al 75%.

Entre 40–49%: activa la bomba al 60%.

Mayor o igual que 50%: apaga la bomba.

Envío de datos por UART:

```
if (contador >= 10) {
    sprintf(buffer, "Luz: %.2fV | T: %uC | H: %u%%\r\n", volt, temp, hum);
    UART_WriteString(buffer);
    contador = 0;
} else {
    contador++;
}

```
Cada 10 ciclos (cada 5 segundos aprox.):

Muestra el voltaje de  en forma de soleado, nublado o noche, temperatura y humedad por UART por medio de serial, graficando tambien los datos de temperatura por python.





## Diagramas

### Diagrama esquematico

![Esquematico proyecto](https://github.com/ECCI-microprocesadores/proyecto-entrega-final-g2-e1/blob/51fe4025bf56f4db71f0b184c6b1f751667cc83d/Imagenes/INVERNADERO.png)

### Diagrama de flujo


## Conclusiones


<!-- Crear una carpeta src e incluir en ella los códigos y/o el proyecto de mplab-->
