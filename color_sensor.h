/* 
 * File:   color_sensor.h
 * Author: seungleechoi
 *
 * Created on March 21, 2018, 6:21 PM
 */

#include "i2c_protocol.h"
#ifndef COLOR_SENSOR_H
#define	COLOR_SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SENSOR_ADDR 0x29
void sensorInit();
void resetSensor();

int getRed();
int getGreen();
int getBlue();
void i2c_test();

#ifdef	__cplusplus
}
#endif

#endif	/* COLOR_SENSOR_H */

