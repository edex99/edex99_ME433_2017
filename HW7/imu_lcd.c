#include "imu_lcd.h"

void draw_accel_xy_LCD(signed short accel_x, signed short accel_y) {
    signed short length_x = (signed short) ((accel_x/32768.0)*60);
    signed short length_y = (signed short) ((accel_y/32768.0)*60);
    LCD_drawBar(length_x,3,64, 64,BLUE, GREEN);
}
