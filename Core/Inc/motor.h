#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "pid.h"

#define PI (3.1415926f)
#define REDUCTION_RATIO 30
#define WHEEL_D 65
#define ENCODER_LINE_NUM 11
#define ENCODER_FREQ_DOUBLE 4

#define AUTO_SPEED_MAX 600
#define TIRE_SPACE 200//轮胎宽度方向距离 mm

#define CAR_WIDTH_X  200//轮胎宽度方向距离 mm
#define CAR_HEIGHT_Y 200//轮胎高度方向距离 mm
#define CAR_WIDTH_X_HALF  CAR_WIDTH_X / 2
#define CAR_HEIGHT_Y_HALF CAR_HEIGHT_Y/ 2

typedef enum
{
  MOTOR_DIR_STOP = 1U,
  MOTOR_DIR_FORWARD,
  MOTOR_DIR_BACKWARD
} MOTOR_DIRECTION;

typedef enum
{
  MOTOR_A1 = 0U,
  MOTOR_A2,
  MOTOR_B1,
  MOTOR_B2
} MOTOR_NAME;

typedef enum
{
  ENCODER_A1 = 0U,
  ENCODER_A2,
  ENCODER_B1,
  ENCODER_B2
} ENCODER_NAME;

typedef struct {
  MOTOR_DIRECTION dir; // todo 暂时没用到
  int32_t raw_value;
  int16_t overflow_cnt;
  float calc_value;
}encoder_data_t;

typedef struct {
  MOTOR_NAME m_name;
  ENCODER_NAME e_name;

  GPIO_TypeDef *dir_port_x, *dir_port_y;
  uint16_t dir_pin_x,dir_pin_y;
  int dir_forward_mode; // 0/1翻转
// 输出控制
  TIM_HandleTypeDef *tim_pwm;
  unsigned int tim_pwm_channel;
// 编码器
  TIM_HandleTypeDef *tim_encoder;
  encoder_data_t encoder_data;
// pid
  Pid_t pid_para;

  int16_t speed_target;
  int16_t speed_current;
}motor_info_t;

extern motor_info_t motor_A1;
extern motor_info_t motor_A2;
extern motor_info_t motor_B1;
extern motor_info_t motor_B2;

void motor_set_direction(motor_info_t*, MOTOR_DIRECTION);
void motor_set_pwm(motor_info_t *m, uint32_t s);
void motor_set_speed_pwm(motor_info_t *m, int32_t s);

void motor_init(void);
void motor_test_pwm(void);
void motor_test_encoder(void);
void motor_encoder_init(void);
void motor_encoder_overflow_IRQHandler(TIM_HandleTypeDef *htim);
void motor_encoder_parse(void);


void motor_kdr_cmd(uint8_t* b);
void motor_kdr_data(void);

#endif
