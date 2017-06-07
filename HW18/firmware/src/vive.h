#ifndef VIVE_H
#define	VIVE_H

#include <xc.h>
#include <math.h>

#define DEG_PER_CORE 0.0009 // equal to (180 deg) / (8333 us) / (24 core tick per us)
#define LIGHTHOUSEHEIGHT 7.0 // in feet
#define DEG_TO_RAD 0.01745 // pi/180
// structure to store the sensor data
typedef struct {
  long changeTime[11];
  long prevMic; // used to detect timer overflow
  double horzAng;
  double vertAng;
  int useMe;
  int collected; // is changeTime full
} viveSensor;

volatile viveSensor V1;

void initVive();
double get_x();
double get_y();

#endif

