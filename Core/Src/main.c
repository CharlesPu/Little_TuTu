/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "oled_u8g2.h"
#include "motor.h"
#include "motion.h"
#include "hc_sr04.h"
#include "buzzer.h"
#include "nrf24l01.h"
#include "com_rc.h"

#ifdef MODULE_MPU6050
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#endif

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	HAL_Delay(100); // for oled

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  MX_USART3_UART_Init();
  MX_TIM7_Init();
  MX_USART2_UART_Init();
  MX_TIM10_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
  uart_dma_it_init();
  OLED_U8G2_init();
  BUZZER_init();
#ifdef MODULE_HC_SR04
  HC_SR04_init();
#endif
#ifdef MODULE_NRF24L01_RX
  NRF24L01_init();
#endif
#ifdef MODULE_MPU6050
  MPU_Init();					       //��ʼ��MPU6050
  uint8_t res = 0;
  do{
    res = mpu_dmp_init();
		ERR_LOG("mpu_dmp_init code: %d\r\n",res);
	}while (res);
#endif
  motor_init();
  motor_encoder_init();

  HAL_Delay(2000);
  BUZZER_beep_twice();
  INF_LOG("little tutu start!\r\n");
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint64_t loop_cnt = 0;
  uint16_t rc_disconnect_cnt = 0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    //////////////////////////////  10ms   /////////////////////////////////
    // HAL_UART_Transmit(&huart1,(uint8_t *)"hello world!\r\n",14,HAL_MAX_DELAY);
    // OLED_U8G2_draw_test();
    // motion_control_test_direction();

#ifdef MODULE_MPU6050
    imu_data_t imu_data;
    res= mpu_dmp_get_data(&imu_data);
    if(!res)
		{
			imu_data.temp = MPU_Get_Temperature();							  //�õ��¶�ֵ
			// MPU_Get_Accelerometer(&imu_data);	//�õ����ٶȴ���������
			// MPU_Get_Gyroscope(&imu_data);	//�õ�����������
		}else {
      ERR_LOG("mpu_dmp_get_data fail: %d\r\n", res);
    }
#endif
    //////////////////////////////  1s   ///////////////////////////////// 
    if (loop_cnt % 100 == 0) {
      HAL_GPIO_TogglePin(DOGGY_GPIO_Port, DOGGY_Pin);

      // motion_control_guardian();
      // motor_test_pwm();
#ifdef MODULE_MPU6050   
      OLED_U8G2_draw_mpu6050(&imu_data);
#endif
    }
    //////////////////////////////  500ms   ///////////////////////////////// 
    if (loop_cnt % 50 == 0) {

    }
    //////////////////////////////  100ms   ///////////////////////////////// 
    if (loop_cnt % 10 == 3) {
#ifdef MODULE_KDR_REPORTER
      motor_kdr_data();
#endif
    }
    //////////////////////////////  50ms   ///////////////////////////////// 
    if (loop_cnt % 5 == 1) {
#ifdef MODULE_NRF24L01_RX
    // 以下写成函数有问题，没有找到原因。。
      rc_data_t rc;
      rc_data_init(&rc);
      // 接受成功才会使用遥控数据，否则断连保护，速度都是0
      if(NRF24L01_RxPacket(rc.buf)==0)// 成功
      {
        rc_disconnect_cnt = 0;
        uint8_t res_dec = rc_data_decode(&rc);
        // OLED_U8G2_draw_rc(&rc);
        if (res_dec) {
          ERR_LOG("rc decode fail!\r\n");
          motion_control_stop();
        }else
          motion_control_kinematics(motion_control_rc_to_kinematics(&rc));
      }else { // 说明遥控断了
        if (rc_disconnect_cnt > 12) // 断了(600ms左右)就停止控制
          motion_control_stop();
        else 
          rc_disconnect_cnt++;
      }
#endif
    }

		HAL_Delay(10);
    loop_cnt++;
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
