#ifndef __MOTION_H__
#define __MOTION_H__

#include "main.h"
#include "com_rc.h"

typedef struct 
{
  int16_t speed_x;
  int16_t speed_y;
  int16_t speed_z;
}car_kinematics_speed_t;


#define CTRL_DATA_BUF_LEN 1

#define CAR_WIDTH_X  180 //轮胎宽度方向距离 mm
#define CAR_HEIGHT_Y 120 //轮胎高度方向距离 mm
#define CAR_WIDTH_X_HALF  CAR_WIDTH_X / 2
#define CAR_HEIGHT_Y_HALF CAR_HEIGHT_Y/ 2

void motion_control_kinematics(car_kinematics_speed_t speed);
void motion_control_input_ble(uint8_t* b);
void motion_control_input_rc(uint16_t car_speed_x, uint16_t car_speed_y, uint16_t car_speed_z);
void motion_control_motor_pid(void);
void motion_control_stop(void);
void motion_control_guardian(void);
car_kinematics_speed_t motion_control_rc_to_kinematics(rc_data_t *rc);

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
