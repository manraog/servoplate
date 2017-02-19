#include "I2C.h"
#include "io430.h"

//Inicia I2C en USCI, falta pulir, ideal seria usar vectores de interrupción
//Basado en ejemplo de TI
void Init_I2C_USCI(unsigned char addr)
{
        P1SEL |= BIT6 + BIT7;                     //Asigna la funcion de I2C a los pines SCL > P1.6, SDA > 1.7 Pag 49 msp430g2553 datasheet
	P1SEL2 |= BIT6 + BIT7;                    //Asigna la funcion de I2C a los pines SCL > P1.6, SDA > 1.7
	UCB0CTL1 |= UCSWRST;                      // Detiene USCI
	UCB0CTL0 = UCMST+UCMODE_3+UCSYNC;         // Modo maestro sincrono
  	UCB0CTL1 = UCSSEL_2+UCSWRST;              // Usa SMCLK 2Mhz en este caso y mantiene detenido el modulo
	UCB0BR0 = 20;                            // Preescaler para velocidad de SCL SMCLK/100kHz >> 2Mhz/100Khz = ~20 
	UCB0BR1 = 0;
	UCB0I2CSA = addr;                         // Direccion del esclavo MPU6050 en este caso
	UCB0CTL1 &= ~UCSWRST;                     // Inicia USCI
        __bis_SR_register(GIE); // Habilita interrupciones
}

//Establece la dirección del dispositivo a comunicar
void Set_Address(unsigned char addr)
{
  	UCB0CTL1 |= UCSWRST;                            //Detiene USCI
  	UCB0I2CSA = addr;                     		// Guarda Direccion
  	UCB0CTL1 &= ~UCSWRST;                 		// Inicia USCI
}

//Lee un byte
int ByteRead_I2C_USCI(unsigned char address)
{ 	
	while (UCB0CTL1 & UCTXSTP); //Si se esta ocupado espera hasta que termine ( In master receiver mode the STOP condition is preceded by a NACK. UCTXSTP is automatically cleared after STOP is generated.) 
	UCB0CTL1 |= UCTR + UCTXSTT;             //Modo transmision y envia condicion de inicio y direccion de esclavo ( In master receiver mode a repeated START condition is preceded by a NACK. UCTXSTT is automatically cleared after START condition andaddress information is transmitted.)
	while (!(IFG2&UCB0TXIFG));		//Espera a que termine
	UCB0TXBUF = address;                    //Envía la dirección del registro a leer

	while (!(IFG2&UCB0TXIFG));	        //Espera a que termine 
	UCB0CTL1 &= ~UCTR;                      // Cambia a modo recepción
	UCB0CTL1 |= UCTXSTT;                    // Envia condicion de inicio
	IFG2 &= ~UCB0TXIFG;                     // Limpia bandera de interrupcion
        
	while (UCB0CTL1 & UCTXSTT);             //Espera a STT
	UCB0CTL1 |= UCTXSTP;                    //Envia condicion de stop
	return UCB0RXBUF;                       //Devuelve el valor del buffer (recepcion)
}

unsigned char ByteWrite_I2C_USCI(unsigned char address, unsigned char data)
{
	while (UCB0CTL1 & UCTXSTP);             // Si esta ocupado espera
	UCB0CTL1 |= UCTR + UCTXSTT;             // Modo transmision y envia condicion de inicio y direccion de esclavo
	while (!(IFG2&UCB0TXIFG));		//Espera a que termine
	UCB0TXBUF = address;		        //Envía dirección de registro

	while (!(IFG2&UCB0TXIFG));				// Espera a que termine transmision de direccion
	UCB0TXBUF = data;					// Envía byte a escribir

	while (!(IFG2&UCB0TXIFG));				// Espera a que termine transmision de datos
	UCB0CTL1 |= UCTXSTP;                    // Envía condicion de stop
	IFG2 &= ~UCB0TXIFG;                     // Limpia bandera de interrupcion
	return 0;
}





