#include "main.h"
#include "tim.h"
#include "motor.h"
#include "stdio.h"
#include <string.h>
#include "com_kdr.h"

// motor_info_t motor_A1 = {
//   .m_name = MOTOR_A1,
//   .e_name = ENCODER_A1,
//   .dir_port_x = MA1_DX_GPIO_Port,.dir_port_y = MA1_DY_GPIO_Port,
//   .dir_pin_x = MA1_DX_Pin,.dir_pin_y = MA1_DY_Pin,
//   .tim_pwm = &TIM_MOTOR_PWM,.tim_pwm_channel=TIM_CHANNEL_1,
//   .tim_encoder = &TIM_ENCODER_A1,
// };
// motor_info_t motor_A2 = {
//   .m_name = MOTOR_A2,
//   .e_name = ENCODER_A2,
//   .dir_port_x = MA2_DX_GPIO_Port,.dir_port_y = MA2_DY_GPIO_Port,
//   .dir_pin_x = MA2_DX_Pin,.dir_pin_y = MA2_DY_Pin,
//   .tim_pwm = &TIM_MOTOR_PWM,.tim_pwm_channel=TIM_CHANNEL_2,
//   .tim_encoder = &TIM_ENCODER_A2,
// };
// motor_info_t motor_B1 = {
//   .m_name = MOTOR_B1,
//   .e_name = ENCODER_B1,
//   .dir_port_x = MB1_DX_GPIO_Port,.dir_port_y = MB1_DY_GPIO_Port,
//   .dir_pin_x = MB1_DX_Pin,.dir_pin_y = MB1_DY_Pin,
//   .tim_pwm = &TIM_MOTOR_PWM,.tim_pwm_channel=TIM_CHANNEL_3,
//   .tim_encoder = &TIM_ENCODER_B1,
// };
// motor_info_t motor_B2 = {
//   .m_name = MOTOR_B2,
//   .e_name = ENCODER_B2,
//   .dir_port_x = MB2_DX_GPIO_Port,.dir_port_y = MB2_DY_GPIO_Port,
//   .dir_pin_x = MB2_DX_Pin,.dir_pin_y = MB2_DY_Pin,
//   .tim_pwm = &TIM_MOTOR_PWM,.tim_pwm_channel=TIM_CHANNEL_4,
//   .tim_encoder = &TIM_ENCODER_B2,
// };
motor_info_t motor_A1;
motor_info_t motor_A2;
motor_info_t motor_B1;
motor_info_t motor_B2;

void motor_init(void)
{;
  motor_A1.m_name = MOTOR_A1;
  motor_A1.e_name = ENCODER_A1;
  motor_A1.dir_port_x = MA1_DX_GPIO_Port;
  motor_A1.dir_port_y = MA1_DY_GPIO_Port;
  motor_A1.dir_pin_x = MA1_DX_Pin;
  motor_A1.dir_pin_y = MA1_DY_Pin;
  motor_A1.dir_forward_mode = 1;
  motor_A1.tim_pwm = &TIM_MOTOR_PWM;
  motor_A1.tim_pwm_channel=TIM_CHANNEL_1;
  motor_A1.tim_encoder = &TIM_ENCODER_A1;
  motor_A1.encoder_data.overflow_cnt = 0;
  motor_A1.speed_target = 0;
  motor_A1.speed_current = 0;

  motor_A2.m_name = MOTOR_A2;
  motor_A2.e_name = ENCODER_A2;
  motor_A2.dir_port_x = MA2_DX_GPIO_Port;
  motor_A2.dir_port_y = MA2_DY_GPIO_Port;
  motor_A2.dir_pin_x = MA2_DX_Pin;
  motor_A2.dir_pin_y = MA2_DY_Pin;
  motor_A2.dir_forward_mode = 0;
  motor_A2.tim_pwm = &TIM_MOTOR_PWM;
  motor_A2.tim_pwm_channel=TIM_CHANNEL_2;
  motor_A2.tim_encoder = &TIM_ENCODER_A2;
  motor_A2.encoder_data.overflow_cnt = 0;
  motor_A2.speed_target = 0;
  motor_A2.speed_current = 0;

  motor_B1.m_name = MOTOR_B1;
  motor_B1.e_name = ENCODER_B1;
  motor_B1.dir_port_x = MB1_DX_GPIO_Port;
  motor_B1.dir_port_y = MB1_DY_GPIO_Port;
  motor_B1.dir_pin_x = MB1_DX_Pin;
  motor_B1.dir_pin_y = MB1_DY_Pin;
  motor_B1.dir_forward_mode = 1;
  motor_B1.tim_pwm = &TIM_MOTOR_PWM;
  motor_B1.tim_pwm_channel=TIM_CHANNEL_3;
  motor_B1.tim_encoder = &TIM_ENCODER_B1;
  motor_B1.encoder_data.overflow_cnt = 0;
  motor_B1.speed_target = 0;
  motor_B1.speed_current = 0; 

  motor_B2.m_name = MOTOR_B2;
  motor_B2.e_name = ENCODER_B2;
  motor_B2.dir_port_x = MB2_DX_GPIO_Port;
  motor_B2.dir_port_y = MB2_DY_GPIO_Port;
  motor_B2.dir_pin_x = MB2_DX_Pin;
  motor_B2.dir_pin_y = MB2_DY_Pin;
  motor_B2.tim_pwm = &TIM_MOTOR_PWM;
  motor_B2.tim_pwm_channel=TIM_CHANNEL_4;
  motor_B2.tim_encoder = &TIM_ENCODER_B2;
  motor_B2.encoder_data.overflow_cnt = 0;
  motor_B2.speed_target = 0;
  motor_B2.speed_current = 0;

  HAL_TIM_PWM_Start(motor_A1.tim_pwm,motor_A1.tim_pwm_channel);// note!!!!!
  HAL_TIM_PWM_Start(motor_A2.tim_pwm,motor_A2.tim_pwm_channel);// note!!!!!
  HAL_TIM_PWM_Start(motor_B1.tim_pwm,motor_B1.tim_pwm_channel);// note!!!!!
  HAL_TIM_PWM_Start(motor_B2.tim_pwm,motor_B2.tim_pwm_channel);// note!!!!!

  motor_set_speed_pwm(&motor_A1, MOTOR_DIR_STOP);
  motor_set_speed_pwm(&motor_A2, MOTOR_DIR_STOP);
  motor_set_speed_pwm(&motor_B1, MOTOR_DIR_STOP);
  motor_set_speed_pwm(&motor_B2, MOTOR_DIR_STOP);

  // pid init
  // 厂家值: P = 0.35; I = 0.65; D = 0.2;
  motor_A1.pid_para.P = 0.35;
  motor_A1.pid_para.I = 0.65;
  motor_A1.pid_para.D = 0.2;
  motor_A1.pid_para.PrevError = 0;
  motor_A1.pid_para.SumError = 0;

  motor_A2.pid_para.P = 0.35;
  motor_A2.pid_para.I = 0.65;
  motor_A2.pid_para.D = 0.2;
  motor_A2.pid_para.PrevError = 0;
  motor_A2.pid_para.SumError = 0;

  motor_B1.pid_para.P = 0.35;
  motor_B1.pid_para.I = 0.65;
  motor_B1.pid_para.D = 0.2;
  motor_B1.pid_para.PrevError = 0;
  motor_B1.pid_para.SumError = 0;

  motor_B2.pid_para.P = 0.35;
  motor_B2.pid_para.I = 0.65;
  motor_B2.pid_para.D = 0.2;
  motor_B2.pid_para.PrevError = 0;
  motor_B2.pid_para.SumError = 0;
}

void motor_test_pwm(void)
{
  motor_set_speed_pwm(&motor_A1, 1000);
  motor_set_speed_pwm(&motor_A2, 1000);
  motor_set_speed_pwm(&motor_B1, 1000);
  motor_set_speed_pwm(&motor_B2, 1000);
  HAL_Delay(3000);

  motor_set_speed_pwm(&motor_A1, 0);
  motor_set_speed_pwm(&motor_A2, 0);
  motor_set_speed_pwm(&motor_B1, 0);
  motor_set_speed_pwm(&motor_B2, 0);
  HAL_Delay(1000);

  motor_set_speed_pwm(&motor_A1, -1000);
  motor_set_speed_pwm(&motor_A2, -1000);
  motor_set_speed_pwm(&motor_B1, -1000);
  motor_set_speed_pwm(&motor_B2, -1000);
  HAL_Delay(3000);

  motor_set_speed_pwm(&motor_A1, 0);
  motor_set_speed_pwm(&motor_A2, 0);
  motor_set_speed_pwm(&motor_B1, 0);
  motor_set_speed_pwm(&motor_B2, 0);
  HAL_Delay(1000);
}

void motor_encoder_init(void)
{
  HAL_TIM_Encoder_Start(motor_A1.tim_encoder, TIM_CHANNEL_1|TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(motor_A2.tim_encoder, TIM_CHANNEL_1|TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(motor_B1.tim_encoder, TIM_CHANNEL_1|TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(motor_B2.tim_encoder, TIM_CHANNEL_1|TIM_CHANNEL_2);


  __HAL_TIM_CLEAR_FLAG(motor_A1.tim_encoder, TIM_FLAG_UPDATE); // note!!!防止初始化就进入中断！
  __HAL_TIM_CLEAR_FLAG(motor_A2.tim_encoder, TIM_FLAG_UPDATE);
  __HAL_TIM_CLEAR_FLAG(motor_B1.tim_encoder, TIM_FLAG_UPDATE);
  __HAL_TIM_CLEAR_FLAG(motor_B2.tim_encoder, TIM_FLAG_UPDATE);
  HAL_TIM_Base_Start_IT(motor_A1.tim_encoder);
  HAL_TIM_Base_Start_IT(motor_A2.tim_encoder);
  HAL_TIM_Base_Start_IT(motor_B1.tim_encoder);
  HAL_TIM_Base_Start_IT(motor_B2.tim_encoder);

  __HAL_TIM_CLEAR_FLAG(&TIM_ENCODER_CALC, TIM_FLAG_UPDATE);
  HAL_TIM_Base_Start_IT(&TIM_ENCODER_CALC);//note!!!
}
// 字符串模式纯发送
void motor_test_encoder(void)
{
  // INF_LOG("MA1 dir: %d, raw: %d, cal: %.2f\r\n",motor_A1.encoder_data.dir, motor_A1.encoder_data.raw_value,motor_A1.encoder_data.calc_value);
  // INF_LOG("MA2 dir: %d, raw: %d, cal: %.2f\r\n",motor_A2.encoder_data.dir, motor_A2.encoder_data.raw_value,motor_A2.encoder_data.calc_value);
  // INF_LOG("MB1 dir: %d, raw: %d, cal: %.2f\r\n",motor_B1.encoder_data.dir, motor_B1.encoder_data.raw_value,motor_B1.encoder_data.calc_value);
  // INF_LOG("MB2 dir: %d, raw: %d, cal: %.2f\r\n",motor_B2.encoder_data.dir, motor_B2.encoder_data.raw_value,motor_B2.encoder_data.calc_value);

  KDCOM_send((float)(motor_A1.encoder_data.dir),(float)(motor_A1.encoder_data.raw_value),
    (float)(motor_A2.encoder_data.dir),(float)(motor_A2.encoder_data.raw_value),
    (float)(motor_B1.encoder_data.dir),(float)(motor_B1.encoder_data.raw_value),
    (float)(motor_B2.encoder_data.dir),(float)(motor_B2.encoder_data.raw_value));
}

void motor_encoder_overflow_IRQHandler(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == motor_A1.tim_encoder->Instance){
    if(__HAL_TIM_GET_COUNTER(motor_A1.tim_encoder) < 32768 ) motor_A1.encoder_data.overflow_cnt++; else motor_A1.encoder_data.overflow_cnt--;
  }else if (htim->Instance == motor_A2.tim_encoder->Instance) { 
		if(__HAL_TIM_GET_COUNTER(motor_A2.tim_encoder) < 32768 ) motor_A2.encoder_data.overflow_cnt++; else motor_A2.encoder_data.overflow_cnt--;
  }else if (htim->Instance == motor_B1.tim_encoder->Instance) { 
    if(__HAL_TIM_GET_COUNTER(motor_B1.tim_encoder) < 32768 ) motor_B1.encoder_data.overflow_cnt++; else motor_B1.encoder_data.overflow_cnt--;
  }else if (htim->Instance == motor_B2.tim_encoder->Instance) { 
		if(__HAL_TIM_GET_COUNTER(motor_B2.tim_encoder) < 32768 ) motor_B2.encoder_data.overflow_cnt++; else motor_B2.encoder_data.overflow_cnt--;
  }
}

// 溢出中断，定期获取编码，计算车轮速度
void motor_encoder_parse(void)
{
// todo 这里可以根据实际转向手动调整定义
    motor_A1.encoder_data.dir = (__HAL_TIM_IS_TIM_COUNTING_DOWN(motor_A1.tim_encoder)==0)? MOTOR_DIR_FORWARD:MOTOR_DIR_BACKWARD;// todo 暂时没用到
    motor_A1.encoder_data.raw_value =__HAL_TIM_GET_COUNTER(motor_A1.tim_encoder) + motor_A1.encoder_data.overflow_cnt * 65536;
    // mm/s 轮子和地面接触的线速度
    motor_A1.encoder_data.calc_value = -((float)motor_A1.encoder_data.raw_value)/ENCODER_LINE_NUM/REDUCTION_RATIO/ENCODER_FREQ_DOUBLE*PI*WHEEL_D*20;
    motor_A1.speed_current = (int16_t)(motor_A1.encoder_data.calc_value);
    __HAL_TIM_SetCounter(motor_A1.tim_encoder, 0);
    motor_A1.encoder_data.overflow_cnt =0;

    motor_A2.encoder_data.dir = (__HAL_TIM_IS_TIM_COUNTING_DOWN(motor_A2.tim_encoder)==0)? MOTOR_DIR_FORWARD:MOTOR_DIR_BACKWARD;// todo 暂时没用到
    motor_A2.encoder_data.raw_value =__HAL_TIM_GET_COUNTER(motor_A2.tim_encoder) + motor_A2.encoder_data.overflow_cnt * 65536;
    // mm/s 调整方向
    motor_A2.encoder_data.calc_value = ((float)motor_A2.encoder_data.raw_value)/ENCODER_LINE_NUM/REDUCTION_RATIO/ENCODER_FREQ_DOUBLE*PI*WHEEL_D*20;
    motor_A2.speed_current = (int16_t)(motor_A2.encoder_data.calc_value);
    __HAL_TIM_SetCounter(motor_A2.tim_encoder, 0);
    motor_A2.encoder_data.overflow_cnt =0;

    // 由于b1、b2使用的timer预装载值是16位的，反向计数从65535向下
    // 但是转速不能超过55圈/s，否则这里会溢出
    motor_B1.encoder_data.dir = (__HAL_TIM_IS_TIM_COUNTING_DOWN(motor_B1.tim_encoder)==0)? MOTOR_DIR_FORWARD:MOTOR_DIR_BACKWARD;// todo 暂时没用到
    motor_B1.encoder_data.raw_value = __HAL_TIM_GET_COUNTER(motor_B1.tim_encoder) + motor_B1.encoder_data.overflow_cnt * 65536;
    // mm/s
    motor_B1.encoder_data.calc_value = -((float)motor_B1.encoder_data.raw_value)/ENCODER_LINE_NUM/REDUCTION_RATIO/ENCODER_FREQ_DOUBLE*PI*WHEEL_D*20;
    motor_B1.speed_current = (int16_t)(motor_B1.encoder_data.calc_value);
    __HAL_TIM_SetCounter(motor_B1.tim_encoder, 0); 
    motor_B1.encoder_data.overflow_cnt =0;

    motor_B2.encoder_data.dir = (__HAL_TIM_IS_TIM_COUNTING_DOWN(motor_B2.tim_encoder)==0)? MOTOR_DIR_FORWARD:MOTOR_DIR_BACKWARD;// todo 暂时没用到
    motor_B2.encoder_data.raw_value =  __HAL_TIM_GET_COUNTER(motor_B2.tim_encoder) + motor_B2.encoder_data.overflow_cnt * 65536;
    // mm/s
    motor_B2.encoder_data.calc_value = ((float)motor_B2.encoder_data.raw_value)/ENCODER_LINE_NUM/REDUCTION_RATIO/ENCODER_FREQ_DOUBLE*PI*WHEEL_D*20;
    motor_B2.speed_current = (int16_t)(motor_B2.encoder_data.calc_value);
    __HAL_TIM_SetCounter(motor_B2.tim_encoder, 0);
    motor_B2.encoder_data.overflow_cnt=0;
}

void motor_set_speed_pwm(motor_info_t *m, int32_t s)
{
  if (s > 0)
  {
    motor_set_direction(m, MOTOR_DIR_FORWARD);
    motor_set_pwm(m, s);
  }else if (s < 0){
    motor_set_direction(m, MOTOR_DIR_BACKWARD);
    motor_set_pwm(m, -s);
  }else {
    motor_set_direction(m, MOTOR_DIR_STOP);
    motor_set_pwm(m, 0);
  }
}

void motor_set_direction(motor_info_t *m, MOTOR_DIRECTION d)
{
  GPIO_PinState x, y;
  switch (d)
  {
  case MOTOR_DIR_FORWARD:
    x = GPIO_PIN_SET;
    y = GPIO_PIN_RESET;
    break;
  case MOTOR_DIR_BACKWARD:
    x = GPIO_PIN_RESET;
    y = GPIO_PIN_SET;
    break;
  case MOTOR_DIR_STOP:
    x = GPIO_PIN_RESET;
    y = GPIO_PIN_RESET;
    break;
  default:
    break;
  }
  if (m->dir_forward_mode) {// 交换丄1�7
    GPIO_PinState tmp;
    tmp=x;x=y;y=tmp;
  }

  HAL_GPIO_WritePin(m->dir_port_x,m->dir_pin_x,x);HAL_GPIO_WritePin(m->dir_port_y,m->dir_pin_y,y);
}

void motor_set_pwm(motor_info_t *m, uint32_t s)
{
  uint32_t period = __HAL_TIM_GetAutoreload(m->tim_pwm);
  if (s > period) {
    printf("warning: motor_set_pwm: %d outof %d\r\n",s,period);
    INF_LOG("motor_set_pwm: %d outof %d\r\n",s,period);
    s= period;
  }
  __HAL_TIM_SetCompare(m->tim_pwm,m->tim_pwm_channel,s);
}

void motor_kdr_data(void)
{
  // 测试甄1�7
  // KDRobot kd_a1;
  // HAL_UART_Transmit(&huart3, KDCOM_SetData(&kd_a1, 0, 2.0f, 1.0f),sizeof(KDRobot), 100);
  // KDRobot kd_a2;
  // HAL_UART_Transmit(&huart3,KDCOM_SetData(&kd_a2, 1, 21.0f, 0),sizeof(KDRobot), 100);
  // KDRobot kd_b1;
  // HAL_UART_Transmit(&huart3,KDCOM_SetData(&kd_b1, 2, 42.0f, 111.0f),sizeof(KDRobot), 100);
  // KDRobot kd_b2;
  // HAL_UART_Transmit(&huart3,KDCOM_SetData(&kd_b2, 3, 27.0f, 0),sizeof(KDRobot), 100);

  // INF_LOG("MA1 dir: %d, raw: %d, cal: %.2f\r\n",motor_A1.encoder_data.dir, motor_A1.encoder_data.raw_value,motor_A1.encoder_data.calc_value);
  // INF_LOG("MA2 dir: %d, raw: %d, cal: %.2f\r\n",motor_A2.encoder_data.dir, motor_A2.encoder_data.raw_value,motor_A2.encoder_data.calc_value);
  // INF_LOG("MB1 dir: %d, raw: %d, cal: %.2f\r\n",motor_B1.encoder_data.dir, motor_B1.encoder_data.raw_value,motor_B1.encoder_data.calc_value);
  // INF_LOG("MB2 dir: %d, raw: %d, cal: %.2f\r\n",motor_B2.encoder_data.dir, motor_B2.encoder_data.raw_value,motor_B2.encoder_data.calc_value);

  KDRobot kd_a1;
  KDCOM_SendData(KDCOM_SetData(&kd_a1, '0', motor_A1.speed_current, motor_A1.speed_target),sizeof(KDRobot));
  KDRobot kd_a2;
  KDCOM_SendData(KDCOM_SetData(&kd_a2, '1', motor_A2.speed_current, motor_A2.speed_target),sizeof(KDRobot));
  KDRobot kd_b1;
  KDCOM_SendData(KDCOM_SetData(&kd_b1, '2', motor_B1.speed_current, motor_B1.speed_target),sizeof(KDRobot));
  KDRobot kd_b2;
  KDCOM_SendData(KDCOM_SetData(&kd_b2, '3', motor_B2.speed_current, motor_B2.speed_target),sizeof(KDRobot));
}

void motor_kdr_cmd(uint8_t* b)
{
  kdr_data_t data;
  data.buf = (uint8 *)b;
  data.channel = 0;
  float chxp[] = {motor_A1.pid_para.P,    motor_A2.pid_para.P,    motor_B1.pid_para.P,    motor_B2.pid_para.P};
  float chxi[] = {motor_A1.pid_para.I,    motor_A2.pid_para.I,    motor_B1.pid_para.I,    motor_B2.pid_para.I};
  float chxd[] = {motor_A1.pid_para.D,    motor_A2.pid_para.D,    motor_B1.pid_para.D,    motor_B2.pid_para.D};
  float chxt[] = {motor_A1.speed_target,  motor_A2.speed_target,  motor_B1.speed_target,  motor_B2.speed_target};
  float chxa[] = {motor_A1.speed_current, motor_A2.speed_current, motor_B1.speed_current, motor_B2.speed_current};
  float *pchxp[] = {chxp,chxp+1,chxp+2,chxp+3};
  float *pchxi[] = {chxi,chxi+1,chxi+2,chxi+3};
  float *pchxd[] = {chxd,chxd+1,chxd+2,chxd+3};
  float *pchxt[] = {chxt,chxt+1,chxt+2,chxt+3};
  float *pchxa[] = {chxa,chxa+1,chxa+2,chxa+3};
  memcpy(data.p,pchxp,4*sizeof(float *));
  memcpy(data.i,pchxi,4*sizeof(float *));
  memcpy(data.d,pchxd,4*sizeof(float *));
  memcpy(data.target,pchxt,4*sizeof(float *));
  memcpy(data.actual,pchxa,4*sizeof(float *));
  // printf("a111\r\n");

  uint8 res = KDCOM_Process(data.buf, &(data.channel), data.p, data.i, data.d, data.target, data.actual, 
              &(data.start), &(data.stop), &(data.data1), &(data.data2));

  motor_A1.speed_target = (int16_t)(*data.target[0]);
  motor_A2.speed_target = (int16_t)(*data.target[0]);
  motor_B1.speed_target = (int16_t)(*data.target[0]);
  motor_B2.speed_target = (int16_t)(*data.target[0]);

  motor_A1.pid_para.P = *(data.p[0]);
  motor_A2.pid_para.P = *(data.p[1]);
  motor_B1.pid_para.P = *(data.p[2]);
  motor_B2.pid_para.P = *(data.p[3]);

  motor_A1.pid_para.I = *(data.i[0]);
  motor_A2.pid_para.I = *(data.i[1]);
  motor_B1.pid_para.I = *(data.i[2]);
  motor_B2.pid_para.I = *(data.i[3]);

  motor_A1.pid_para.D = *(data.d[0]);
  motor_A2.pid_para.D = *(data.d[1]);
  motor_B1.pid_para.D = *(data.d[2]);
  motor_B2.pid_para.D = *(data.d[3]); 
}
