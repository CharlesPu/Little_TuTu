#ifndef __MOTION_H__
#define __MOTION_H__

#include "main.h"

#define CTRL_DATA_BUF_LEN 1

void motion_control_kinematics(uint16_t car_speed_x, uint16_t car_speed_y, uint16_t car_speed_z);
void motion_control_input_ble(uint8_t* b);
void motion_control_input_rc(uint16_t car_speed_x, uint16_t car_speed_y, uint16_t car_speed_z);
void motion_control_motor_pid(void);
void motion_control_stop(void);
void motion_control_guardian(void);

void motion_control_direction_x(int16_t speed);
void motion_control_direction_y(int16_t speed);
void motion_control_direction_z(int16_t speed);
// 左前、右后
void motion_control_direction_xy(int16_t speed);
// 右前、左后
void motion_control_direction_x_y(int16_t speed);


void motion_control_motor_ctrl_output(void);
void motion_control_test_direction(void);


#endif
