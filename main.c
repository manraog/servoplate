
#include "io430.h"
#include "librerias/SERVO.h"
#include "librerias/UART.h"
#include "librerias/MPU6050.h" //Ya incluye I2C
//#include "QmathLib.h" //Libreria para calculos optimizada por TI >> No funciono, falta investigar mas

////////////////// VARIABLES ////////////////////////////////////
float roll;
float pitch;

////////////////// PRINCIPAL ////////////////////////////////////
void main ()
{
  //////////////// INICIALIZACION
  //Desactiva Watch Dog
  WDTCTL = WDTPW + WDTHOLD;
  WDTCTL = WDTPW + WDTHOLD;                 
  //>>MCLCK
  if (CALBC1_1MHZ==0xFF)   // Si los valores de calibracion no estan
  {											
    while(1); //No inicia
  }
  DCOCTL = 0;                  
  BCSCTL1 = CALBC1_16MHZ; //Calibra el Master Clock a 16Mhz
  DCOCTL = CALDCO_16MHZ;
  //>>SMCLK
  BCSCTL2 = DIVS_3; //Sub Main Clock = MCLK/8 
  //>>UART
  UART_Init(); 
  UART_Write_String("UART bien");
  UART_Write_NL();
  //>>I2C y MPU 6050
  MPU6050_Init();
  UART_Write_String("MPU bien");
  UART_Write_NL();
  //>>SERVOS
  conf_servos();
  angle_servo(1, 90);
  angle_servo(2, 90);
  UART_Write_String("Servo bien");
  UART_Write_NL();
  __delay_cycles(16000000);


  //////////////// LOOP
  while (1)
  {
    Get_Angle_Values(&roll, &pitch); //Obtiene Roll y Pitch
    //=====>>> Esta mal, solo gira en sentido contrario, funciona si el sensor esta en la superficie que se gira. Debia usar PID sencillo como en energia y relimentación negativa
    int16_t error_roll = 0 - (int16_t)roll; //Calcula el error
    int16_t error_pitch = 0 -(int16_t) pitch;
    angle_servo(1, 90 + error_roll); //Cambia angulo de acuerdo al error //Debia ser resta
    angle_servo(2, 90 + error_pitch);
    //<<<===== Esta mal
    UART_Write_Float((int16_t)roll,0); UART_Write_String("  "); UART_Write_Float((signed int)pitch,0); //Debug en terminal serial: ROLL y PITCH
    UART_Write_NL();//Salto de linea
  }
}