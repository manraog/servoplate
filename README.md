# ServoPlate
Plataforma que replica el movimiento mediante servomotores

[![Video](https://img.youtube.com/vi/kQwWf2_LYjU/0.jpg)](https://www.youtube.com/watch?v=kQwWf2_LYjU)

--------------------------------------------

## Material
- Texas Instruments MSP430 Launchpad
- 2 Servo motores
- MPU6050
- Estructura de madera

--------------------------------------------

## Funcionamiento 
Se implementaron librerías de comunicación I2C para comunicarse con el MPU6050 y UART para debug por serial.

Al no existir una librería para el MPU6050 que pueda ser usada en el MSP430 se implemento una con una función de inicialización. La función de inicialización configura el MPU6050 para desactivar el modo sleep, activa un filtro pasa bajas integrado y pone la frecuencia de corte en 184Hz lo que añade inmunidad a altas vibraciones, reduce la velocidad de muestreo a 10Hz para evitar correcciónes frecuentes de la posición y pone la escala a +-2g.

No existe mucha documentación sobre los registros, es cuestión de probar con los que se encuentran en [esta hoja de datos](https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf) y compararlos con los usados en la librería de Arduino que fue hecha con mucha prueba y error. Se puede encontrar más información en [Arduino Playground](https://playground.arduino.cc/Main/MPU-6050) y en [I2Cdevlib](https://www.i2cdevlib.com/devices/mpu6050#source)

**El workspace se abre con el IDE de IAR para MSP430.**
