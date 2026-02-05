#include "motion.h"
#include "motor.h"
#include "pid.h"
#include "stdio.h"
#include "buzzer.h"
#include "hc_sr04.h"
#include "oled_u8g2.h"
#include "com_rc.h"
#include "nrf24l01.h"
#include "ws2812.h"

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
  // 使用正确的旋转半径计算电机速度
  motor_A1.speed_target = speed.speed_x - speed.speed_y - speed.speed_z * CAR_ROTATION_RADIUS;
  motor_A2.speed_target = speed.speed_x + speed.speed_y + speed.speed_z * CAR_ROTATION_RADIUS;
  motor_B1.speed_target = speed.speed_x + speed.speed_y - speed.speed_z * CAR_ROTATION_RADIUS;
  motor_B2.speed_target = speed.speed_x - speed.speed_y + speed.speed_z * CAR_ROTATION_RADIUS;
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

// 手柄控制 
// 以下写成函数有问题，还没有找到原因。。
uint16_t s_disconnect_cnt = 0;
void motion_control_input_rc(void)
{
  rc_data_t rc;
  rc_data_init(&rc);
  // 接受成功才会使用遥控数据，否则断连保护，速度都是0

  if(NRF24L01_RxPacket(rc.buf)==0)// 成功
  {
    s_disconnect_cnt = 0;
    uint8_t res_dec = rc_data_decode(&rc);
    // OLED_U8G2_draw_rc_com(&rc);
    if (res_dec) {
      ERR_LOG("rc decode fail!\r\n");
      motion_control_stop();
    }else
      motion_control_kinematics(motion_control_rc_to_kinematics(&rc));
  }else { // 说明遥控断了
    if (s_disconnect_cnt > 3) // 断了3次以上就停止控制
      motion_control_stop();
    else 
      s_disconnect_cnt++;
  }
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
  printf("motion_control_stop\r\n");
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

uint8_t g_is_guardian_enalbed = 0;
uint8_t g_bubble_enabled = 0;
// 按键边沿检测的上一次状态
static uint8_t s_key_l_last = 0;
static uint8_t s_key_r_last = 0;
static uint8_t s_rk_r_z_last = 0;

void motion_control_guardian(void)
{
  if (!g_is_guardian_enalbed) {
    BUZZER_beep_long_off();
    return;
  }
  
#ifdef MODULE_HC_SR04
  uint16_t mm = HC_SR04_sonar_mm();
  // printf("hc_sr04 sonar distance: %d mm\r\n",mm);
  if (mm < 300) {
    // OLED_U8G2_draw_hc_sr04(mm);
    BUZZER_beep_long_on();
    motion_control_stop();
  } else {
    BUZZER_beep_long_off();
  }
#endif
}

uint16_t my_abs(int16_t a)
{
  if (a >= 0) return a;
  else return -a;
}

void car_kinematics_speed_init(car_kinematics_speed_t* sp)
{
  sp->speed_x = 0;
  sp->speed_y = 0;
  sp->speed_z = 0;
}

car_kinematics_speed_t motion_control_rc_to_kinematics(rc_data_t *rc)
{
  car_kinematics_speed_t s;

  uint16_t car_speed_max_set_xy = CAR_SPEED_MAX_DEFAULT_XY;
  uint16_t  car_speed_max_set_z = CAR_SPEED_MAX_DEFAULT_Z;
  if (rc->sw_l_2) {
    car_speed_max_set_xy = CAR_SPEED_MAX_DEFAULT_XY-300;
    car_speed_max_set_z  = CAR_SPEED_MAX_DEFAULT_Z-2;
  }else if (rc->sw_l_3) {
    car_speed_max_set_xy = CAR_SPEED_MAX_DEFAULT_XY+300;
    car_speed_max_set_z  = CAR_SPEED_MAX_DEFAULT_Z+2;
  }
  // 功能开启 - 使用边沿检测，只在按键从0变为1时触发
  if (rc->rk_r_z && !s_rk_r_z_last) g_is_guardian_enalbed = !g_is_guardian_enalbed;
  if (rc->key_l && !s_key_l_last) g_rgb_led_enabled = !g_rgb_led_enabled;
  if (rc->key_r && !s_key_r_last) g_bubble_enabled = !g_bubble_enabled;

  // 保存当前状态用于下次边沿检测
  s_rk_r_z_last = rc->rk_r_z;
  s_key_l_last = rc->key_l;
  s_key_r_last = rc->key_r;

  // 根据状态控制吐泡泡机
  if (g_bubble_enabled) {
    HAL_GPIO_WritePin(BUBBLE_A_GPIO_Port, BUBBLE_A_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BUBBLE_B_GPIO_Port, BUBBLE_B_Pin, GPIO_PIN_RESET);
  } else
  {
    HAL_GPIO_WritePin(BUBBLE_A_GPIO_Port, BUBBLE_A_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BUBBLE_B_GPIO_Port, BUBBLE_B_Pin, GPIO_PIN_RESET);
  }

  // 先判断控制方式
  if (rc->sw_r_1) // keys
  {
    s.speed_x = (float)(2048 - rc->rk_l_y)/2048.0f * car_speed_max_set_xy;
    s.speed_y = (float)(2048 - rc->rk_l_x)/2048.0f * car_speed_max_set_xy;
    s.speed_z = (float)(2048 - rc->rk_r_x)/2048.0f * car_speed_max_set_z;
  }else if (rc->sw_r_2) { // imu
    int8_t pitch = rc->imu_pitch;
    if (pitch < -90) pitch = -90;else if (pitch > 90) pitch = 90;
    int8_t roll = rc->imu_roll;
    if (roll < -90) roll = -90;else if (roll > 90) roll = 90;
    int8_t yaw = rc->imu_yaw;
    if (yaw < -90) yaw = -90;else if (yaw > 90) yaw = 90;

    s.speed_x = (float)(pitch)/90.0f * car_speed_max_set_xy;
    s.speed_y = (float)(-roll)/90.0f * car_speed_max_set_xy;
    s.speed_z = (float)(yaw)/90.0f * car_speed_max_set_z;
  }

  // 防止0漂
  if (my_abs(s.speed_x) <= 20) s.speed_x = 0;
  if (my_abs(s.speed_y) <= 20) s.speed_y = 0;
  if (my_abs(s.speed_z) <= 2) s.speed_z = 0;  // 旋转速度死区（提升到2以匹配更高的最大速度）

  // INF_LOG("rc result: x=%d, y=%d, z=%d, xy_max=%d, z_max=%d\r\n",s.speed_x, s.speed_y, s.speed_z,car_speed_max_set_xy,car_speed_max_set_z);
  return s;
}
