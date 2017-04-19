#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include "imu.h"
#include "ILI9163C.h"
#include <stdio.h>
#include "imu_lcd.h"

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // free up secondary osc pins
#pragma config FPBDIV = DIV_1 // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // slowest wdt
#pragma config WINDIS = OFF // no wdt window
#pragma config FWDTEN = OFF // wdt off by default
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the CPU clock to 48MHz
// 8 MHz input from crystal resonator
#pragma config FPLLIDIV = DIV_2 // divide input clock by 2 to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV by 24, 96 MHz
#pragma config FPLLODIV = DIV_2 // divide clock by 2 after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiply by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 00000000 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module


int main() {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    //initialize SPI1 and LCD
    SPI1_init();
    LCD_init();
    
    // initialize expander
    initIMU();

    __builtin_enable_interrupts();
    
    LCD_clearScreen(GREEN);
    
    // WHO_AM_I read test
    /*
    i2c_master_start();
    i2c_master_send(OPCODE_WRITE);
    i2c_master_send(0x0F);
    i2c_master_restart();
    i2c_master_send(OPCODE_READ);
    char c = i2c_master_recv();
    i2c_master_ack(1); 
    char str[15];
    sprintf(str,"%d",c);
    LCD_writeString(str,28,32,BLACK,GREEN); */
 
    unsigned char start_reg = 0x20;    //OUT_TEMP_L
    int length = 14;                   // read until OUTZ_H_XL
    unsigned char data[14];
    char str[15];
    
    _CP0_SET_COUNT(0);
    while(1) {
        if (_CP0_GET_COUNT() > (48000000/2)/5) { // 5 Hz read
            _CP0_SET_COUNT(0);
            I2C_read_multiple(OPCODE_WRITE>>1, start_reg, data, length);
            signed short temp = (data[1]<<8) | data[0];
            signed short pitch_x = (data[3]<<8) | data[2];
            signed short roll_y = (data[5]<<8) | data[4];
            signed short yaw_z = (data[7]<<8) | data[6];
            signed short accel_x = -(data[9]<<8) | data[8];
            signed short accel_y = -(data[11]<<8) | data[10];
            signed short accel_z = -(data[13]<<8) | data[12];
            
            //sprintf(str,"x: %d ",accel_x);
            //LCD_writeString(str,28,32,BLACK,GREEN);
            //sprintf(str,"y: %d ",accel_y);
            //LCD_writeString(str,28,80,BLACK,GREEN);
            draw_accel_xy_LCD(accel_x, accel_y);
        }
    }
    
}