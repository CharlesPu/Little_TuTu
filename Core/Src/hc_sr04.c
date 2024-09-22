#include "hc_sr04.h"
#include "tim.h"

void HC_SR04_init(void)
{
  // HAL_TIM_Base_Start_IT(&TIM_HC_SR04);//note!!!
  HAL_GPIO_WritePin(HC_SR04_ECHO_GPIO_Port, HC_SR04_ECHO_Pin, GPIO_PIN_RESET);
}

uint32_t tim_cnt=0;

// note！！！！！: 不要在里面printf，会影响精度
uint32_t HC_SR04_sonar_mm(void)
{
  uint32_t distance = 0;
  uint32_t tim_cnt_end = 0;
  HAL_GPIO_WritePin(HC_SR04_TRIG_GPIO_Port, HC_SR04_TRIG_Pin,GPIO_PIN_RESET);//预先拉低Trig引脚
  HAL_Delay(5);

  HAL_GPIO_WritePin(HC_SR04_TRIG_GPIO_Port, HC_SR04_TRIG_Pin,GPIO_PIN_SET);//拉高Trig引脚
  Tims_delay_us(15); // > 10us
  HAL_GPIO_WritePin(HC_SR04_TRIG_GPIO_Port, HC_SR04_TRIG_Pin,GPIO_PIN_RESET);//拉低Trig引脚

  while(HAL_GPIO_ReadPin(HC_SR04_ECHO_GPIO_Port, HC_SR04_ECHO_Pin) == 0);//如果是低电平，一直等
  HAL_TIM_Base_Start_IT(&TIM_HC_SR04);//note!!!
  tim_cnt = 0;
  while(HAL_GPIO_ReadPin(HC_SR04_ECHO_GPIO_Port, HC_SR04_ECHO_Pin) == 1);//如果是高电平，一直等
  HAL_TIM_Base_Stop_IT(&TIM_HC_SR04);
  tim_cnt_end = tim_cnt;

  if (tim_cnt_end/100 < 38){//判断是否小于38毫秒，大于38毫秒的就是超时，直接调到下面返回0
    distance = tim_cnt_end*346/2;//计算距离，25°C空气中的音速为346m/s = 346mm/ms = 0.346mm/us = 3.46mm/10us
    distance = distance/100;
	}
	return distance;									//返回测距结果
}

// /10us
void HC_SR04_TIM_IRQHandler(void)
{
	tim_cnt++;
}
