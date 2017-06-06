#include "motor.h"

void initMotors(){
    RPB15Rbits.RPB15R = 0b0101;   // OC1 at B15 (enable 1)
    RPB13Rbits.RPB13R = 0b0101;   // OC4 at B13 (enable 2)
    TRISBbits.TRISB8 = 0;       // B8 is output pin 
    TRISBbits.TRISB9 = 0;       // B9 is output pin 
    
    // initialize OC1
    OC1CONbits.OC32 = 0;				// Use either Timer2 or Timer3
    OC1CONbits.OCTSEL = 1;              // Use Timer3  
    //T2CONbits.TCKPS = 0;				// Timer3 prescaler N=1
    //PR3 = PR;							// period = (PR3+1) * N * 12.5 ns = 100 us, 10 kHz
    //TMR2 = 0;							// initial TMR3 count is 0
    OC1CONbits.OCM = 0b110;             // PWM mode without fault pin; other OC31On bits are default
    OC1RS = 0;						// duty cycle = OC1RS/(PR2+1) = 75%
    OC1R = 0;						// initialize before turning OC1 on; afterward it is read-only
    //T2CONbits.ON = 1;					// turn on Timer3
    OC1CONbits.ON = 1;					// turn on OC1
    
    // initialize OC2
    OC4CONbits.OC32 = 0;				// Use either Timer2 or Timer3
    OC4CONbits.OCTSEL = 1;              // Use Timer3  
    T3CONbits.TCKPS = 0;				// Timer3 prescaler N=1
    PR3 = PR;							// period = (PR3+1) * N * 12.5 ns = 50 us, 20 kHz
    TMR3 = 0;							// initial TMR3 count is 0
    OC4CONbits.OCM = 0b110;             // PWM mode without fault pin; other OC31On bits are default
    OC1RS = 0;						// duty cycle = OC1RS/(PR2+1) = 75%
    OC4R = 0;						// initialize before turning OC1 on; afterward it is read-only
    T3CONbits.ON = 1;					// turn on Timer3
    OC4CONbits.ON = 1;					// turn on OC1

}


void pwm_set(signed int pwm1, signed int pwm2) {
    // motor 1
    if (pwm1 <= 0) {
        LATBbits.LATB8 = 1;
        if (pwm1 >= -PR-1)
            OC1RS = -pwm1*PWM_MUL;
        else
            OC1RS = (PR+1)*PWM_MUL;
    }
    else {
        LATBbits.LATB8 = 0;
        if (pwm1 <= PR+1)
            OC1RS = pwm1*PWM_MUL;
        else
            OC1RS = (PR+1)*PWM_MUL;
    }
    // motor 2
    if (pwm2 <= 0) {
        LATBbits.LATB9 = 0;
        if (pwm2 >= -PR-1)
            OC4RS = -pwm2*PWM_MUL;
        else
            OC4RS = (PR+1)*PWM_MUL;
    }
    else {
        LATBbits.LATB9 = 1;
        if (pwm2 <= PR+1)
            OC4RS = pwm2*PWM_MUL;
        else
            OC4RS = (PR+1)*PWM_MUL;
    }
}

void calc_control(int motor_offset) {
    int pwm1 = 100;         // left or right?
    int pwm2 = 100;         // left or right?
    if (motor_offset >= 0)
        pwm2 -= motor_offset;
    else 
        pwm1 += motor_offset;
    
    pwm_set(pwm1, pwm2);
}


