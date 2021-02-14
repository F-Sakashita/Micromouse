/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

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
#define DBG_LED0_Pin LL_GPIO_PIN_13
#define DBG_LED0_GPIO_Port GPIOC
#define DBG_LED1_Pin LL_GPIO_PIN_14
#define DBG_LED1_GPIO_Port GPIOC
#define DBG_LED2_Pin LL_GPIO_PIN_15
#define DBG_LED2_GPIO_Port GPIOC
#define WALL_LED0_Pin LL_GPIO_PIN_0
#define WALL_LED0_GPIO_Port GPIOC
#define WALL_LED1_Pin LL_GPIO_PIN_1
#define WALL_LED1_GPIO_Port GPIOC
#define WALL_LED2_Pin LL_GPIO_PIN_2
#define WALL_LED2_GPIO_Port GPIOC
#define WALL_LED3_Pin LL_GPIO_PIN_3
#define WALL_LED3_GPIO_Port GPIOC
#define ENC_R_A_Pin LL_GPIO_PIN_0
#define ENC_R_A_GPIO_Port GPIOA
#define ENC_R_B_Pin LL_GPIO_PIN_1
#define ENC_R_B_GPIO_Port GPIOA
#define DBG_TX_Pin LL_GPIO_PIN_2
#define DBG_TX_GPIO_Port GPIOA
#define DBG_RX_Pin LL_GPIO_PIN_3
#define DBG_RX_GPIO_Port GPIOA
#define BAT_MONI_Pin LL_GPIO_PIN_4
#define BAT_MONI_GPIO_Port GPIOA
#define IMU_SCK_Pin LL_GPIO_PIN_5
#define IMU_SCK_GPIO_Port GPIOA
#define WALL_SEN0_Pin LL_GPIO_PIN_6
#define WALL_SEN0_GPIO_Port GPIOA
#define WALL_SEN1_Pin LL_GPIO_PIN_7
#define WALL_SEN1_GPIO_Port GPIOA
#define TICK_LED_Pin LL_GPIO_PIN_4
#define TICK_LED_GPIO_Port GPIOC
#define SW0_Pin LL_GPIO_PIN_5
#define SW0_GPIO_Port GPIOC
#define WALL_SEN2_Pin LL_GPIO_PIN_0
#define WALL_SEN2_GPIO_Port GPIOB
#define WALL_SEN3_Pin LL_GPIO_PIN_1
#define WALL_SEN3_GPIO_Port GPIOB
#define SW1_Pin LL_GPIO_PIN_2
#define SW1_GPIO_Port GPIOB
#define MOTOR_R_DIR1_Pin LL_GPIO_PIN_8
#define MOTOR_R_DIR1_GPIO_Port GPIOC
#define MOTOR_R_DIR0_Pin LL_GPIO_PIN_9
#define MOTOR_R_DIR0_GPIO_Port GPIOC
#define MOTOR_R_PWM_Pin LL_GPIO_PIN_8
#define MOTOR_R_PWM_GPIO_Port GPIOA
#define MOTOR_L_PWM_Pin LL_GPIO_PIN_9
#define MOTOR_L_PWM_GPIO_Port GPIOA
#define MOTOR_L_DIR1_Pin LL_GPIO_PIN_10
#define MOTOR_L_DIR1_GPIO_Port GPIOA
#define MOTOR_L_DIR0_Pin LL_GPIO_PIN_11
#define MOTOR_L_DIR0_GPIO_Port GPIOA
#define MOTOR_STBY_Pin LL_GPIO_PIN_12
#define MOTOR_STBY_GPIO_Port GPIOA
#define ENC_L_A_Pin LL_GPIO_PIN_15
#define ENC_L_A_GPIO_Port GPIOA
#define BUZZER_Pin LL_GPIO_PIN_2
#define BUZZER_GPIO_Port GPIOD
#define ENC_L_B_Pin LL_GPIO_PIN_3
#define ENC_L_B_GPIO_Port GPIOB
#define IMU_MISO_Pin LL_GPIO_PIN_4
#define IMU_MISO_GPIO_Port GPIOB
#define IMU_MOSI_Pin LL_GPIO_PIN_5
#define IMU_MOSI_GPIO_Port GPIOB
#define IMU_CS_Pin LL_GPIO_PIN_6
#define IMU_CS_GPIO_Port GPIOB
#define IMU_INT_Pin LL_GPIO_PIN_7
#define IMU_INT_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
