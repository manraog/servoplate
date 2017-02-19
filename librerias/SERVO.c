#include "SERVO.h"
#include "stdint.h"
#include "math.h"
#include "io430.h"

void conf_servos( void )
{
    //Configura pines >> Uso TA1 por que algunos pinde de TA0 estan ocupados
    P2DIR |= BIT1; //Salida servo 1 > P2.1 
    P2SEL |= BIT1; //Salida TA1.1
    P2DIR |= BIT4; //Salida servo 2 > P2.4
    P2SEL |= BIT4; //Salida TA1.2
    TA1CCR0 = 39999; //Perido de 20ms para Servomotor  >> 20ms/(1/2Mhz)=40,000
    TA1CCTL1 |= OUTMOD_7; //Modo Reste/Set >> Compara la cuenta si  cuenta < TA1CCTL1 TA1.1 en alto, cuenta > TA1CCTL1 TA1.1 en bajo
    TA1CCTL2 |= OUTMOD_7; //Modo Reste/Set >> Compara la cuenta si  cuenta < TA1CCTL1 TA1.2 en alto, cuenta > TA1CCTL1 TA1.2 en bajo
    //TA1CCR1 = 2799; //0°-1159   45°-1979   90°-2799   135°-3769   180°-4739     
    //TA1CCR2 = 2799; //0°-1159   45°-1979   90°-2799   135°-3769   180°-4739     
    TA1CTL |= TASSEL_2 + MC_1; //Modo UP: Cuenta hasta TA1CCR0, Usa el REloj SMLCK en este caso 2Mhz
	return;
}

void angle_servo(unsigned char servo, int16_t angle)
{
	int16_t val = 1159 + (angle*20); //Convierte angulo a ancho de pulso
	if (servo == 1)
		TA1CCR1 = val;
	if (servo == 2)
		TA1CCR2 = val;
	return;
}