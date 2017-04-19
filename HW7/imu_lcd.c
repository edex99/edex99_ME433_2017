#include "imu_lcd.h"

void draw_accel_xy_LCD(signed short accel_x, signed short accel_y) {
    signed short length_x = (signed short) ((accel_x/32768.0)*60);
    signed short length_y = (signed short) ((accel_y/32768.0)*60);
    char dir_x = 'h';
    char dir_y = 'v';
    LCD_drawBar(dir_x,length_x,5,64, 64,BLUE, GREEN);
    LCD_drawBar(dir_y,length_y,5,64, 64,RED, GREEN);
}
