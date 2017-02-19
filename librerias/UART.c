#include "io430.h"
#include "UART.h"

////// Inicializa el UART lo ideal sería usar vectores de interrupción
// Modificado de ejemplo de TI
void UART_Init()
{
        P1SEL |= BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
	P1SEL2 |= BIT1 + BIT2;
	UCA0CTL0 =0 ;		//Tat Parity ,LSB first,8-bit data,one stop bits
	UCA0CTL1 |= UCSSEL_2; // Utiliza SMCLK
        UCA0BR0 = 208;  //Preescaler para obtener 9600 Bauds >> SMCLK/Baudrate >> Chip en Launchpad permite un maximo de 9600 >> 2Mhz/9600= ~208
        UCA0BR1 = 0;
        UCA0MCTL |=UCBRS_5;	// Modulation UCBRSx = 5
	UCA0CTL1 &=~ UCSWRST; // **Initialize USCI state machine**
	//IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	__bis_SR_register(GIE); // Interrupts enabled
}

//////////////////// TRANSMISION /////////////////////////////////
//Escribe un solo caracter
void UART_Write_Char(unsigned char data)
{
	while (!(IFG2&UCA0TXIFG)); // Espera a que termine de enviar el caracter anterior
	UCA0TXBUF = data; // Envia el caracter
}

//Escribe una cadena de caracteres
void UART_Write_String(char *string)
{        
	while(*string) //Recorre el string
		{
		while (!(IFG2&UCA0TXIFG)); // Espera a que termine de enviar el caracter anterior
		UCA0TXBUF= *string; // Manda el siguiente caracter
		string++; // Cambia el apuntador al siguiente caracter
		}	     
}

//Salto de linea
void UART_Write_NL( void )
{
  UART_Write_String("\n\r");
}

//Escribe entero sin signo
void UART_Write_Int(unsigned long n)
{
     unsigned char buffer[16];
     unsigned char i,j;

     if(n == 0) 
     {
    	 UART_Write_Char('0');
          return;
     }
      //>Separa numeros
     for (i = 15; i > 0 && n > 0; i--) 
     {
          buffer[i] = (n%10)+'0';
          n /= 10;
     }
     //>Envia numeros individuales
     for(j = i+1; j <= 15; j++) 
     {
    	 UART_Write_Char(buffer[j]);
     }
}

//Escribe un flotante segun determinada cantidad de decimales  
void UART_Write_Float(float x, int coma)
{
	unsigned long temp; //Puede usarse para enteros con signo
	if(coma>4)coma=4;

	if(x<0) //Si el numero es negativo
	{
		UART_Write_Char('-');	//Envía signo		
		x*=-1;
	}
	temp = (unsigned long)x; //Envía parte entera 					
	UART_Write_Int(temp);

	x = (float)(x-temp); //Obtiene solo parte decimal
	if(coma>0) //Si coma es mayor a 0 imprime decimales
        { 
          UART_Write_Char('.');	//Envía punto decimal
	  while(coma>0) 
	    {
		x*=10; //Desplaza deciamles a la izquierda del punto                              
		coma--;
	    }
          temp = (unsigned long)x+1;	
	  UART_Write_Int(temp); //Envía el decimal como entero
        }
}


///////////////////// RECEPCION /////////////////////////////////////

// Leee caracter
char UART_Read_Char()
{
	while (!(IFG2&UCA0RXIFG)); //Espera a que se reciba algo en el buffer
	return UCA0RXBUF; //Devuelve el caracter
}

// Wait to receive a string through UART
// Note that the string is ended with '\0' (or 0x00)

void UART_Read_String(char *s)
{
       *s = UART_Read_Char();
	while(*s!='\0')
	{
		s++;
		*s = UART_Read_Char();
	}
}
