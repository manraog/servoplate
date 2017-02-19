#ifndef UART_H_   
#define UART_H_

//*****************************************************************************
// Configura e Inicia  UART
//*****************************************************************************
void UART_Init();	

//*****************************************************************************
// Enviar UART
//*****************************************************************************					       	
void UART_Write_Char(unsigned char data);	   	
void UART_Write_String(char* string);		  	
void UART_Write_Int(unsigned long n);		   	
void UART_Write_Reg(char *name, int n);			
void UART_Write_Float(float x,int coma);		//coma<=4
void UART_Write_NL(void);

//*****************************************************************************
// Recibir UART
//*****************************************************************************
char UART_Read_Char();							
void UART_Read_String(char *s);					
char UART_Data_Ready();							

#endif
//