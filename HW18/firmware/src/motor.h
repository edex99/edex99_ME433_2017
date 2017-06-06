#ifndef MOTOR__H__
#define MOTOR__H__

#include <xc.h>           // processor SFR definitions

#define PR 7999
#define PWM_MUL 1// ((PR+1)/100)

void initMotors();
void pwm_set(signed int pwm1, signed int pwm2);
void calc_control(int line_center);

#endif