#ifndef IMU_LCD_H
#define	IMU_LCD_H

#include <xc.h>           // processor SFR definitions
#include "imu.h"
#include "ILI9163C.h"

void draw_accel_xy_LCD(signed short accel_x, signed short accel_y);


#endif	/* IMU_LCD_H */

