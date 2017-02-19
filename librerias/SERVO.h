#ifndef SERVO_H_   
#define SERVO_H_
#include "stdint.h"
void conf_servos( void );
void angle_servo(unsigned char servo, int16_t angle);

#endif