#ifndef EXPANDER__H__
#define EXPANDER__H__

#include <xc.h>           // processor SFR definitions
#include "i2c_master_noint.h"

#define OPCODE_WRITE 0b01000000     //most significant byte of slave address
                                    //A0,A1,A2 tied to ground, lsb 0 is write operation
#define OPCODE_READ 0b01000001     //most significant byte of slave address
                                    //A0,A1,A2 tied to ground, lsb 1 is read operation

void initExpander();
char getExpander();
void setExpander(char pin, char level);

#endif