/* 
 * File:   pixy.h
 * Author: seungleechoi
 *
 * Created on March 28, 2018, 11:16 PM
 */

#ifndef PIXY_H
#define	PIXY_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "i2c_protocol.h"
#include "timers.h"
//#include "debug.h"
#include "task.h"
#include "queue.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include <math.h>

#define PIXYADDR 0x54
// pixy configuration    
void setUpPixy(void);
//function to read signatures from pixy
uint16_t readSignature(unsigned int addr);
// IR sensor implementation
void setUpADC(void);
unsigned int readIR();
unsigned int convertIR();


#ifdef	__cplusplus
}
#endif

#endif	/* PIXY_H */

