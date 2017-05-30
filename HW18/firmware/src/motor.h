#ifndef MOTOR__H__
#define MOTOR__H__

#include <xc.h>           // processor SFR definitions

#define PR 7999
#define PWM_MUL (PR+1)/100 

void initMotors();
void pwm_set(signed int pwm1, signed int pwm2);

#endif