#include "imu_lcd.h"

void draw_accel_xy_LCD(signed short accel_x, signed short accel_y) {
    signed short length_x = (signed short) ((accel_x/32768.0)*60);  //scale bar length
    signed short length_y = (signed short) ((accel_y/32768.0)*60);
    char dir_x = 'h';   //horizontal bar to align with IMU
    char dir_y = 'v';   //vertical bar
    LCD_drawBar(dir_x,length_x,3,64, 64,BLUE, GREEN);
    LCD_drawBar(dir_y,length_y,3,65, 65,RED, GREEN);
}
