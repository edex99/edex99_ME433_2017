#include "vive.h"

/*void initVive() {
// initialize the sensor variables
    V1.prevMic = 0;
    V1.horzAng = 0;
    V1.vertAng = 0;
    V1.useMe = 0;
    V1.collected = 0;

    TRISBbits.TRISB3 = 1; // connect the TS3633 ENV pin to B3
    IC4Rbits.IC4R = 0b0001; // B3 is IC4 (input capture 4)
    IC4CONbits.ICM = 1; // detect rising and falling edges
    IC4CONbits.ICI = 0; // interrupt on an edge
    IC4CONbits.ICTMR = 1; // store the value of timer2, but we're actually just using the interrupt and ignoring timer2
    IC4CONbits.FEDGE = 0; // first event is falling edge, doesn't really matter
    IC4CONbits.ON = 1;
    IPC4bits.IC4IP = 5; // step 4: interrupt priority 5
    IPC4bits.IC4IS = 1; // step 4: interrupt priority 1
    IFS0bits.IC4IF = 0; // step 5: clear the int flag
    IEC0bits.IC4IE = 1; // step 6: enable INT0 by setting IEC0<3>
}

/*double get_x() {
    return tan((V1.vertAng - 90.0) * DEG_TO_RAD) * LIGHTHOUSEHEIGHT;
}

double get_y() {
    return tan((V1.horzAng - 90.0) * DEG_TO_RAD) * LIGHTHOUSEHEIGHT;
}*/
