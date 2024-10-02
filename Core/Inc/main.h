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
#include <stdio.h>
#include <stdarg.h>
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
#define MB1_DY_Pin GPIO_PIN_2
#define MB1_DY_GPIO_Port GPIOE
#define MB1_DX_Pin GPIO_PIN_3
#define MB1_DX_GPIO_Port GPIOE
#define MA1_DX_Pin GPIO_PIN_4
#define MA1_DX_GPIO_Port GPIOE
#define MA1_DY_Pin GPIO_PIN_5
#define MA1_DY_GPIO_Port GPIOE
#define BUZZER_Pin GPIO_PIN_13
#define BUZZER_GPIO_Port GPIOC
#define MA12_STBY_Pin GPIO_PIN_0
#define MA12_STBY_GPIO_Port GPIOC
#define EA1_A_Pin GPIO_PIN_0
#define EA1_A_GPIO_Port GPIOA
#define EA1_B_Pin GPIO_PIN_1
#define EA1_B_GPIO_Port GPIOA
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
#define KITTEN_Pin GPIO_PIN_15
#define KITTEN_GPIO_Port GPIOE
#define MPU_I2C_SCL_Pin GPIO_PIN_10
#define MPU_I2C_SCL_GPIO_Port GPIOB
#define MPU_I2C_SDA_Pin GPIO_PIN_11
#define MPU_I2C_SDA_GPIO_Port GPIOB
#define BLE_KDR_TX_Pin GPIO_PIN_8
#define BLE_KDR_TX_GPIO_Port GPIOD
#define BLE_KDR_RX_Pin GPIO_PIN_9
#define BLE_KDR_RX_GPIO_Port GPIOD
#define MA2_DY_Pin GPIO_PIN_10
#define MA2_DY_GPIO_Port GPIOD
#define DOGGY_Pin GPIO_PIN_11
#define DOGGY_GPIO_Port GPIOD
#define MA2_DX_Pin GPIO_PIN_12
#define MA2_DX_GPIO_Port GPIOD
#define MB2_DY_Pin GPIO_PIN_13
#define MB2_DY_GPIO_Port GPIOD
#define MB2_DX_Pin GPIO_PIN_14
#define MB2_DX_GPIO_Port GPIOD
#define EB2_A_Pin GPIO_PIN_6
#define EB2_A_GPIO_Port GPIOC
#define EB2_B_Pin GPIO_PIN_7
#define EB2_B_GPIO_Port GPIOC
#define MB2_PWM_Pin GPIO_PIN_11
#define MB2_PWM_GPIO_Port GPIOA
#define EA2_A_Pin GPIO_PIN_15
#define EA2_A_GPIO_Port GPIOA
#define HC_SR04_TRIG_Pin GPIO_PIN_4
#define HC_SR04_TRIG_GPIO_Port GPIOD
#define BLE_CTRL_TX_Pin GPIO_PIN_5
#define BLE_CTRL_TX_GPIO_Port GPIOD
#define BLE_CTRL_RX_Pin GPIO_PIN_6
#define BLE_CTRL_RX_GPIO_Port GPIOD
#define HC_SR04_ECHO_Pin GPIO_PIN_7
#define HC_SR04_ECHO_GPIO_Port GPIOD
#define EA2_B_Pin GPIO_PIN_3
#define EA2_B_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define DBG 0
#define INF 1
#define ERR 2
#define LOG_LEVEL DBG
 
#define LOG(level, format, ...)                                                             \
    do                                                                                      \
    {                                                                                       \
        if (level < LOG_LEVEL)                                                              \
            break;                                                                          \
        fprintf(stdout, "[%s:%d][%s] " format "\n", __FILE__, __LINE__, __FUNCTION__,##__VA_ARGS__); \
    } while (0)
 
#define INF_LOG(format, ...) LOG(INF, format, ##__VA_ARGS__)
#define DBG_LOG(format, ...) LOG(DBG, format, ##__VA_ARGS__)
#define ERR_LOG(format, ...) LOG(ERR, format, ##__VA_ARGS__)

// #define MODULE_MPU6050
#define MODULE_HC_SR04
#define MODULE_KDR_REPORTER

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
