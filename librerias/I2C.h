#ifndef I2C_H_   
#define I2C_H_

void Init_I2C_USCI(unsigned char addr);
void Set_Address(unsigned char addr);
int ByteRead_I2C_USCI(unsigned char address);
unsigned char ByteWrite_I2C_USCI(unsigned char address, unsigned char Data);
unsigned char WordRead_I2C_USCI(unsigned char Addr_Data,unsigned char *Data, unsigned char Length);

#endif // I2C_H_