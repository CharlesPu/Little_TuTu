#include "motion.h"
#include "motor.h"
#include "pid.h"
#include "stdio.h"
#include "buzzer.h"
#include "hc_sr04.h"
#include "oled_u8g2.h"

extern motor_info_t motor_A1;
extern motor_info_t motor_A2;
extern motor_info_t motor_B1;
extern motor_info_t motor_B2;

//       ^ x  mm/s
//       |         
// y <-- |  mm/s
// 逆时针 z  rad/s
// 小车整体的运动学控制
void motion_control_kinematics(car_kinematics_speed_t speed)
{
  if((speed.speed_x == 0) && (speed.speed_y == 0)&& (speed.speed_z == 0) &&(motor_A1.speed_current == 0))
  {
    PID_Param_SetZero(&(motor_A1.pid_para));
    PID_Param_SetZero(&(motor_A2.pid_para));
    PID_Param_SetZero(&(motor_B1.pid_para));
    PID_Param_SetZero(&(motor_B2.pid_para));
  }
  motor_A1.speed_target = speed.speed_x - speed.speed_y - speed.speed_z * (CAR_WIDTH_X_HALF + CAR_HEIGHT_Y_HALF);
  motor_A2.speed_target = speed.speed_x + speed.speed_y + speed.speed_z * (CAR_WIDTH_X_HALF + CAR_HEIGHT_Y_HALF);
  motor_B1.speed_target = speed.speed_x + speed.speed_y - speed.speed_z * (CAR_WIDTH_X_HALF + CAR_HEIGHT_Y_HALF);
  motor_B2.speed_target = speed.speed_x - speed.speed_y + speed.speed_z * (CAR_WIDTH_X_HALF + CAR_HEIGHT_Y_HALF);
}

int16_t speed_default = 1000;
// 蓝牙控制
void motion_control_input_ble(uint8_t* b)
{
  switch (b[0])
  {
  case '1':
    motion_control_direction_x(speed_default);
    break;
  case '2':
    motion_control_direction_x(-speed_default);
    break;
  case '3':
    motion_control_direction_y(speed_default);
    break;
  case '4':
    motion_control_direction_y(-speed_default);
    break;
  case '5':
    motion_control_direction_z(speed_default);
    break;
  case '6':
    motion_control_direction_z(-speed_default);
    break;
  case '7':
    speed_default+=500;
    break;
  case '8':
    speed_default-=500;
    if (speed_default < 0) speed_default = 0;
    break;
  case 'a':
    {car_kinematics_speed_t s = {
      .speed_x = speed_default,
      .speed_y = 0,
      .speed_z = 0,
    };
    motion_control_kinematics(s);}
    break;
  case 'b':
   { car_kinematics_speed_t s = {
      .speed_x = 0,
      .speed_y = speed_default,
      .speed_z = 0,
    };
    motion_control_kinematics(s);}
    break;
  case 'c':
    {car_kinematics_speed_t s = {
      .speed_x = 0,
      .speed_y = 0,
      .speed_z = speed_default/200,
    };
    motion_control_kinematics(s);}
    break;
  case 'd':
    {car_kinematics_speed_t s = {
      .speed_x = speed_default,
      .speed_y = speed_default/2,
      .speed_z = 0,
    };
    motion_control_kinematics(s);}
    break;
  default:
    motion_control_stop();
    break;
  }
}

// 手柄控制 todo
void motion_control_input_rc(uint16_t car_speed_x, uint16_t car_speed_y, uint16_t car_speed_z)
{
  
}
// 输出控制和指令输入的处理分离
void motion_control_motor_ctrl_output()
{
  motion_control_motor_pid();
}

void motion_control_test_direction(void)
{
  uint32_t it = 3000;
  int16_t speed = 500;
  motion_control_direction_x(speed);
  HAL_Delay(it);

  motion_control_direction_x(-speed);
  HAL_Delay(it);
  // 左平移
  motion_control_direction_y(speed);
  HAL_Delay(it);
  // 右平移
  motion_control_direction_y(-speed);
  HAL_Delay(it);

  // 原地左转
  motion_control_direction_z(speed);
  HAL_Delay(it);

  // 原地右转
  motion_control_direction_z(-speed);
  HAL_Delay(it);

  // 左前
  motion_control_direction_xy(speed);
  HAL_Delay(it);

  // 右前
  motion_control_direction_x_y(speed);
  HAL_Delay(it);

  // 左后
  motion_control_direction_x_y(-speed);
  HAL_Delay(it);

  // 右后
  motion_control_direction_xy(-speed);
  HAL_Delay(it);

  motion_control_stop();
}

void motion_control_motor_pid(void)
{
// 不运行时清零，当系统判断没有运行时，主动将积分I\微分D清零
  if((motor_A1.speed_target == 0)&&(motor_A1.speed_current == 0))    PID_Param_SetZero(&(motor_A1.pid_para));
  if((motor_A2.speed_target == 0)&&(motor_A2.speed_current == 0))    PID_Param_SetZero(&(motor_A2.pid_para));
  if((motor_B1.speed_target == 0)&&(motor_B1.speed_current == 0))    PID_Param_SetZero(&(motor_B1.pid_para));
  if((motor_B2.speed_target == 0)&&(motor_B2.speed_current == 0))    PID_Param_SetZero(&(motor_B2.pid_para));
  
  int16_t _sMotorA_PWM = PID_wheelspeed_calc(&motor_A1.pid_para, motor_A1.speed_current, motor_A1.speed_target);
  int16_t _sMotorB_PWM = PID_wheelspeed_calc(&motor_A2.pid_para, motor_A2.speed_current, motor_A2.speed_target);
  int16_t _sMotorC_PWM = PID_wheelspeed_calc(&motor_B1.pid_para, motor_B1.speed_current, motor_B1.speed_target);
  int16_t _sMotorD_PWM = PID_wheelspeed_calc(&motor_B2.pid_para, motor_B2.speed_current, motor_B2.speed_target);

  // printf("PID_wheelspeed_calc: cur: %d, tar: %d, pwm: %d\r\n", motor_A1.speed_current, motor_A1.speed_target, _sMotorA_PWM);
  // printf("PID_wheelspeed_calc: cur: %d, tar: %d, pwm: %d\r\n", motor_A2.speed_current, motor_A2.speed_target, _sMotorB_PWM);
  // printf("PID_wheelspeed_calc: cur: %d, tar: %d, pwm: %d\r\n", motor_B1.speed_current, motor_B1.speed_target, _sMotorC_PWM);
  // printf("PID_wheelspeed_calc: cur: %d, tar: %d, pwm: %d\r\n", motor_B2.speed_current, motor_B2.speed_target, _sMotorD_PWM);

  motor_set_speed_pwm(&motor_A1, _sMotorA_PWM);
  motor_set_speed_pwm(&motor_A2, _sMotorB_PWM);
  motor_set_speed_pwm(&motor_B1, _sMotorC_PWM);
  motor_set_speed_pwm(&motor_B2, _sMotorD_PWM);
}

void motion_control_direction_x(int16_t speed)
{
  motor_A1.speed_target = speed;
  motor_A2.speed_target = speed;
  motor_B1.speed_target = speed;
  motor_B2.speed_target = speed;
}
void motion_control_direction_y(int16_t speed)
{
  motor_A1.speed_target = -speed;
  motor_A2.speed_target = speed;
  motor_B1.speed_target = speed;
  motor_B2.speed_target = -speed;
}
void motion_control_direction_z(int16_t speed)
{
  motor_A1.speed_target = -speed;
  motor_A2.speed_target = speed;
  motor_B1.speed_target = -speed;
  motor_B2.speed_target = speed;
}
// 左前、右后
void motion_control_direction_xy(int16_t speed)
{
  motor_A1.speed_target = 0;
  motor_A2.speed_target = speed;
  motor_B1.speed_target = speed;
  motor_B2.speed_target = 0;
}
// 右前、左后
void motion_control_direction_x_y(int16_t speed)
{
  motor_A1.speed_target = speed;
  motor_A2.speed_target = 0;
  motor_B1.speed_target = 0;
  motor_B2.speed_target = speed;
}

void motion_control_stop(void)
{
  motor_A1.speed_target = 0;
  motor_A2.speed_target = 0;
  motor_B1.speed_target = 0;
  motor_B2.speed_target = 0;
  // motion_control_motor_pid();

// 不运行时清零，当系统判断没有运行时，主动将积分I\微分D清零
  // PID_Param_SetZero(&(motor_A1.pid_para));
  // PID_Param_SetZero(&(motor_A2.pid_para));
  // PID_Param_SetZero(&(motor_B1.pid_para));
  // PID_Param_SetZero(&(motor_B2.pid_para));
}

void motion_control_guardian(void)
{
#ifdef MODULE_HC_SR04
  uint16_t mm = HC_SR04_sonar_mm();
  // printf("hc_sr04 sonar distance: %d mm\r\n",mm);
  OLED_U8G2_draw_hc_sr04(mm);
  if (mm < 300) {
    BUZZER_beep_long_on();
    motion_control_stop();
  } else {
    BUZZER_beep_long_off();
  }
#endif
}

car_kinematics_speed_t motion_control_rc_to_kinematics(rc_data_t *rc)
{
  car_kinematics_speed_t s;

  s.speed_x = (float)(2048 - rc->rk_l_y)/2048.0f * 800;
  s.speed_y = (float)(2048 - rc->rk_l_x)/2048.0f * 800;
  s.speed_z = (float)(2048 - rc->rk_r_y)/2048.0f * 6;

  if ((s.speed_x >0 &&s.speed_x < 20) || s.speed_x < 0 &&s.speed_x > -20) s.speed_x = 0;
  if ((s.speed_y >0 &&s.speed_y < 20) || s.speed_y < 0 &&s.speed_y > -20) s.speed_y = 0;

  printf("aaa %d %d %d \r\n",s.speed_x,s.speed_y,s.speed_z);
  return s;
}
