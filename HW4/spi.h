#ifndef SPI__H__
#define SPI__H__

#include <xc.h>           // processor SFR definitions

#define CS LATBbits.LATB15

void initSPI1();
unsigned char spi1_io(unsigned char o);
void setVoltage(unsigned char channel, unsigned char voltage);

#endif