
#include "io430.h"
#include "MPU6050.h"
#include "math.h"
#include "stdint.h"

//Configura MPU
void MPU6050_Init()
{
        //Inicia I2C con direccion de MPU
        Init_I2C_USCI(MPU6050_ADDRESS);
        
        //Desabilita modo SLEEP >> Siempre inicia en modo sleep, hay que desactivarlo
	while(ByteWrite_I2C_USCI(MPU6050_RA_PWR_MGMT_1, 0x00));  
        
        //Filtro pasa bajas Frecuencia de corte = 184Hz >> Menor sensibilidad a vibraciones a altas frecuencias
	while(ByteWrite_I2C_USCI(MPU6050_RA_CONFIG, 0x06));
        
        //Velocidad de muestreo = 10Hz
	while(ByteWrite_I2C_USCI(MPU6050_RA_SMPLRT_DIV,0x00));
               
	//Escala +-2g
	while(ByteWrite_I2C_USCI(MPU6050_RA_ACCEL_CONFIG, 0x00));
}

//El MPU6050 responde con su dirección por default 0x68
unsigned char MPU6050_Test_I2C()
{
	unsigned char i;	
	i = ByteRead_I2C_USCI(MPU6050_RA_WHO_AM_I);
        
	if(i == MPU6050_ADDRESS)
		return 1;
	else
		return 0;
}	

//Datos crudos
vector Get_Accel_Values(){	
        vector accel_out; //Tipo de dato propio declarado en MPU6050.h
        unsigned char a[2];
        a[1] =ByteRead_I2C_USCI(MPU6050_RA_ACCEL_XOUT_H); //Une el registro alto de 8 bits y el bajo de 8 bits para forma los 16 de salida del ADC del MPU
        a[0] =ByteRead_I2C_USCI(MPU6050_RA_ACCEL_XOUT_L);
        accel_out.x = a[0]<<8|a[1];
        
        a[1] =ByteRead_I2C_USCI(MPU6050_RA_ACCEL_YOUT_H);
        a[0] =ByteRead_I2C_USCI(MPU6050_RA_ACCEL_YOUT_L);
        accel_out.y =  a[0]<<8|a[1];
        
        a[1] =ByteRead_I2C_USCI(MPU6050_RA_ACCEL_ZOUT_H);
        a[0] =ByteRead_I2C_USCI(MPU6050_RA_ACCEL_ZOUT_L);
        accel_out.z =  a[0]<<8|a[1];          
	return accel_out;
}	

//Calculo del angulo 
void Get_Angle_Values(float *roll, float *pitch) //Recibe el apuntador a una variable declarada en main y modifica sus valores como resultado
{
  vector accel = Get_Accel_Values(); //Tipo de dato propio declarado en MPU6050.h
  *roll = (atan2f(accel.y, accel.z))*(57.295779);
  *pitch = (atan2f(accel.x,sqrt(pow(accel.y,2)+pow(accel.z,2)) ))*(57.295779);
  return;
}
        	