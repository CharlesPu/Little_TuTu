/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim5;

extern TIM_HandleTypeDef htim6;

extern TIM_HandleTypeDef htim7;

extern TIM_HandleTypeDef htim8;

extern TIM_HandleTypeDef htim10;

/* USER CODE BEGIN Private defines */
#define TIM_MOTOR_PWM htim1

#define TIM_ENCODER_A1 htim5
#define TIM_ENCODER_A2 htim2
#define TIM_ENCODER_B1 htim3
#define TIM_ENCODER_B2 htim8
#define TIM_ENCODER_CALC htim7 // 周期计算

#define TIM_HC_SR04 htim10 // 超声波模块测�?
/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM5_Init(void);
void MX_TIM6_Init(void);
void MX_TIM7_Init(void);
void MX_TIM8_Init(void);
void MX_TIM10_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
void Tims_delay_us(uint16_t nus);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

