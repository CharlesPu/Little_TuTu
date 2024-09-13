/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MA1_DX_Pin GPIO_PIN_2
#define MA1_DX_GPIO_Port GPIOE
#define MA1_DY_Pin GPIO_PIN_3
#define MA1_DY_GPIO_Port GPIOE
#define MA2_DX_Pin GPIO_PIN_4
#define MA2_DX_GPIO_Port GPIOE
#define MA2_DY_Pin GPIO_PIN_5
#define MA2_DY_GPIO_Port GPIOE
#define EA1_A_Pin GPIO_PIN_0
#define EA1_A_GPIO_Port GPIOA
#define EA1_B_Pin GPIO_PIN_1
#define EA1_B_GPIO_Port GPIOA
#define EA2_A_Pin GPIO_PIN_5
#define EA2_A_GPIO_Port GPIOA
#define EB1_A_Pin GPIO_PIN_6
#define EB1_A_GPIO_Port GPIOA
#define EB1_B_Pin GPIO_PIN_7
#define EB1_B_GPIO_Port GPIOA
#define MA1_PWM_Pin GPIO_PIN_9
#define MA1_PWM_GPIO_Port GPIOE
#define MA2_PWM_Pin GPIO_PIN_11
#define MA2_PWM_GPIO_Port GPIOE
#define MB1_PWM_Pin GPIO_PIN_13
#define MB1_PWM_GPIO_Port GPIOE
#define MB2_PWM_Pin GPIO_PIN_14
#define MB2_PWM_GPIO_Port GPIOE
#define MPU_I2C_SCL_Pin GPIO_PIN_10
#define MPU_I2C_SCL_GPIO_Port GPIOB
#define MPU_I2C_SDA_Pin GPIO_PIN_11
#define MPU_I2C_SDA_GPIO_Port GPIOB
#define MB2_DX_Pin GPIO_PIN_8
#define MB2_DX_GPIO_Port GPIOD
#define MB1_DX_Pin GPIO_PIN_9
#define MB1_DX_GPIO_Port GPIOD
#define MB2_DY_Pin GPIO_PIN_10
#define MB2_DY_GPIO_Port GPIOD
#define DOGGY_Pin GPIO_PIN_11
#define DOGGY_GPIO_Port GPIOD
#define EB2_A_Pin GPIO_PIN_12
#define EB2_A_GPIO_Port GPIOD
#define MB1_DY_Pin GPIO_PIN_13
#define MB1_DY_GPIO_Port GPIOD
#define EA2_B_Pin GPIO_PIN_3
#define EA2_B_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define EB2_B_Pin GPIO_PIN_7
#define EB2_B_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_9
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
