/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define DBG_LED0_Pin GPIO_PIN_13
#define DBG_LED0_GPIO_Port GPIOC
#define DBG_LED1_Pin GPIO_PIN_14
#define DBG_LED1_GPIO_Port GPIOC
#define DBG_LED2_Pin GPIO_PIN_15
#define DBG_LED2_GPIO_Port GPIOC
#define WALL_LED0_Pin GPIO_PIN_0
#define WALL_LED0_GPIO_Port GPIOC
#define WALL_LED1_Pin GPIO_PIN_1
#define WALL_LED1_GPIO_Port GPIOC
#define WALL_LED2_Pin GPIO_PIN_2
#define WALL_LED2_GPIO_Port GPIOC
#define WALL_LED3_Pin GPIO_PIN_3
#define WALL_LED3_GPIO_Port GPIOC
#define ENC_R_A_Pin GPIO_PIN_0
#define ENC_R_A_GPIO_Port GPIOA
#define ENC_R_B_Pin GPIO_PIN_1
#define ENC_R_B_GPIO_Port GPIOA
#define DBG_TX_Pin GPIO_PIN_2
#define DBG_TX_GPIO_Port GPIOA
#define DBG_RX_Pin GPIO_PIN_3
#define DBG_RX_GPIO_Port GPIOA
#define BAT_MONI_Pin GPIO_PIN_4
#define BAT_MONI_GPIO_Port GPIOA
#define IMU_SCK_Pin GPIO_PIN_5
#define IMU_SCK_GPIO_Port GPIOA
#define WALL_SEN0_Pin GPIO_PIN_6
#define WALL_SEN0_GPIO_Port GPIOA
#define WALL_SEN1_Pin GPIO_PIN_7
#define WALL_SEN1_GPIO_Port GPIOA
#define TICK_LED_Pin GPIO_PIN_4
#define TICK_LED_GPIO_Port GPIOC
#define SW0_Pin GPIO_PIN_5
#define SW0_GPIO_Port GPIOC
#define WALL_SEN2_Pin GPIO_PIN_0
#define WALL_SEN2_GPIO_Port GPIOB
#define WALL_SEN3_Pin GPIO_PIN_1
#define WALL_SEN3_GPIO_Port GPIOB
#define SW1_Pin GPIO_PIN_2
#define SW1_GPIO_Port GPIOB
#define MOTOR_R_DIR1_Pin GPIO_PIN_8
#define MOTOR_R_DIR1_GPIO_Port GPIOC
#define MOTOR_R_DIR0_Pin GPIO_PIN_9
#define MOTOR_R_DIR0_GPIO_Port GPIOC
#define MOTOR_R_PWM_Pin GPIO_PIN_8
#define MOTOR_R_PWM_GPIO_Port GPIOA
#define MOTOR_L_PWM_Pin GPIO_PIN_9
#define MOTOR_L_PWM_GPIO_Port GPIOA
#define MOTOR_L_DIR1_Pin GPIO_PIN_10
#define MOTOR_L_DIR1_GPIO_Port GPIOA
#define MOTOR_L_DIR0_Pin GPIO_PIN_11
#define MOTOR_L_DIR0_GPIO_Port GPIOA
#define MOTOR_STBY_Pin GPIO_PIN_12
#define MOTOR_STBY_GPIO_Port GPIOA
#define ENC_L_A_Pin GPIO_PIN_15
#define ENC_L_A_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_2
#define BUZZER_GPIO_Port GPIOD
#define ENC_L_B_Pin GPIO_PIN_3
#define ENC_L_B_GPIO_Port GPIOB
#define IMU_MISO_Pin GPIO_PIN_4
#define IMU_MISO_GPIO_Port GPIOB
#define IMU_MOSI_Pin GPIO_PIN_5
#define IMU_MOSI_GPIO_Port GPIOB
#define IMU_CS_Pin GPIO_PIN_6
#define IMU_CS_GPIO_Port GPIOB
#define IMU_INT_Pin GPIO_PIN_7
#define IMU_INT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/