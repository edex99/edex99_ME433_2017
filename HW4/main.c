#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include "math.h"
#include "spi.h"

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
    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1;   // Button is input
    TRISAbits.TRISA4 = 0;   // LED is output
    LATAbits.LATA4 = 1;     // Turn LED on by default
    // initialize SPI
    initSPI1();

    __builtin_enable_interrupts();

    double v_sawtooth = 0;
    double v_sin = 0;
    unsigned char sample_num = 0;
                
    _CP0_SET_COUNT(0);
    while(1) {
        // Clock is at 48 MHz, ticks occur at 24 MHz, 24 Mhz/1 kHz = 24,000 counts
        if (_CP0_GET_COUNT() > 24000) {
            _CP0_SET_COUNT(0);
            
            //CS = 0;
            //spi1_io(255);
            //CS = 1;
            //setVoltage(1,128);
            
            v_sawtooth = sample_num*(255.0/200.0);
            //if (v_sawtooth > 255.0)
            //    v_sawtooth = 0.0;
            setVoltage(0, (unsigned char) v_sawtooth);
            
            v_sin = sin(2.0*M_PI*( (double) (sample_num%100))/100.0);
            setVoltage(1, (unsigned char) ((v_sin+1)*255/2) );
            if (sample_num >= 200) {
                sample_num = 0;
                //LATAbits.LATA4 = !LATAbits.LATA4;
            }
            sample_num++;
        }  
    }
}