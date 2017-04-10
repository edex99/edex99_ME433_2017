#include "spi.h"

void initSPI1(){
    //RPB15Rbits.RPB15R = 0b0011;
    TRISBbits.TRISB15 = 0; // SS1 (B15) set to output
    CS = 1; // set SS1  
    RPB13Rbits.RPB13R = 0b0011; // SDO1 assigned to RPB13
    SDI1Rbits.SDI1R = 0b0100; //SDI1 assigned to RPB8
    
    // setup spi1
    SPI1CON = 0;              // turn off the spi module and reset it
    SPI1BUF;                  // clear the rx buffer by reading from it
    SPI1BRG = 0x1;            // baud rate to 12 MHz [SPI4BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0;  // clear the overflow bit
    SPI1CONbits.CKE = 1;      // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1;    // master operation
    SPI1CONbits.ON = 1;       // turn on spi 1

}

unsigned char spi1_io(unsigned char o) {
    SPI1BUF = o;
    while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
        ;
    }
    return SPI1BUF;
}

void setVoltage(unsigned char channel, unsigned char voltage) {
    unsigned char least_sig = (voltage<<4);
    unsigned char most_sig = (voltage>>4);
    if (channel == 0) {
        most_sig |= 0x70;
    }
    else if (channel == 1) {
        most_sig |= 0xF0;
    }
    CS = 0;
    spi1_io(most_sig);
    spi1_io(least_sig);
    CS = 1;
    
}
