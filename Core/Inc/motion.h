#ifndef __MOTION_H__
#define __MOTION_H__

#include "main.h"


void car_motion_control(uint8_t _cAngularDirection, uint16_t _cAngularSpeed, uint8_t _cMotionDirection, uint16_t _sMotionSpeed);
void car_motion_control_test_motor(void);


#endif
