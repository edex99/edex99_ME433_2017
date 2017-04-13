#include "expander.h"

void initExpander() {
    ANSELBbits.ANSB2 = 0;   // turn off Analog on PIC32 for B2 and B3
    ANSELBbits.ANSB3 = 0;
    
    i2c_master_setup();     // setup I2C on PIC32
    
    // Set IO direction of pins
    i2c_master_start();
    i2c_master_send(OPCODE_WRITE);  // device write opcode
    i2c_master_send(0x00);          // I/O direction register
    i2c_master_send(0b11110000);    // IO7 is input, IO0 is an output, rest are inputs
    i2c_master_stop();
    
    // Set internal pull-up resistor on GPIO7 via GPPU register
    i2c_master_start();
    i2c_master_send(OPCODE_WRITE);  // device write opcode
    i2c_master_send(0x06);          // GPPU register
    i2c_master_send(0b10000000);    // IO7 has pull-up resistor enabled
    i2c_master_stop(); 
    
}

char getExpander() {
    i2c_master_start();
    i2c_master_send(OPCODE_WRITE);  // device write opcode
    i2c_master_send(0x09);          // PORT (GPIO) register
    i2c_master_restart();           // restart bit
    i2c_master_send(OPCODE_READ);   // device read opcode
    char c = i2c_master_recv();     // get character returned
    i2c_master_ack(1);              // acknowledge to slave
    i2c_master_stop();
    return c;
    
}

void setExpander(char pin, char level) {
    // pin is the pin number 0 to 7, level is 0 or 1
    // first read current state
    char c = getExpander();
    // set or clear bit depending on level, anything other than 0 set to 1
    if (level==0) {
        c &= ~(1<<pin);             // clears bit
    }
    else {
        c |= (1<<pin);              // sets bit  
    }
    
    i2c_master_start();
    i2c_master_send(OPCODE_WRITE);  // device write opcode
    i2c_master_send(0x09);          // PORT (GPIO) register
    i2c_master_send(c);             // byte to write to register
    i2c_master_stop();
}
