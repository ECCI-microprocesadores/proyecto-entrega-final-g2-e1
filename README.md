[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=19652994&assignment_repo_type=AssignmentRepo)
# Proyecto final

## Integrantes

Sofy Parra

Carlos Sierra

Edisson Fonseca

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




## Diagramas

### Diagrama esquematico

![Esquematico proyecto](https://github.com/ECCI-microprocesadores/proyecto-entrega-final-g2-e1/blob/51fe4025bf56f4db71f0b184c6b1f751667cc83d/Imagenes/INVERNADERO.png)

### Diagrama de flujo


## Conclusiones


<!-- Crear una carpeta src e incluir en ella los códigos y/o el proyecto de mplab-->
