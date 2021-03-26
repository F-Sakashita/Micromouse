/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SystickTimer.h"
#include "adc.h"
#include "WallSensor.h"
#include "Encoder.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
typedef struct{
  GPIO_TypeDef *pGPIOx;
  uint32_t  uiPinNum;
}Port_t;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static const Port_t stWallLedTbl[] = {
  {WALL_LED0_GPIO_Port, WALL_LED0_Pin},
  {WALL_LED1_GPIO_Port, WALL_LED1_Pin},
  {WALL_LED2_GPIO_Port, WALL_LED2_Pin},
  {WALL_LED3_GPIO_Port, WALL_LED3_Pin},
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  SystickTimer_Interrupt();
  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
  static EN_ENCODER_OVERFLOW enOverflow = EN_ENCODER_OVERFLOW_NONE;
  if(LL_TIM_IsActiveFlag_UPDATE(TIM2)){
    LL_TIM_ClearFlag_UPDATE(TIM2);
    if(LL_TIM_COUNTERDIRECTION_DOWN ==  LL_TIM_GetDirection(TIM2)){
      /* 0 -> CounterPeriod */
      enOverflow = EN_ENCODER_OVERFLOW_MINUS;
    }else{
      enOverflow = EN_ENCODER_OVERFLOW_PLUS;
    }
    //LL_GPIO_TogglePin(DBG_LED2_GPIO_Port, DBG_LED2_Pin);
    Encoder_Interrupt(EN_ENCODER_0, enOverflow);
  }
  /* USER CODE END TIM2_IRQn 0 */
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	if(1u == LL_TIM_IsActiveFlag_UPDATE(TIM3)){
		LL_TIM_ClearFlag_UPDATE(TIM3);
		LL_GPIO_TogglePin(DBG_LED0_GPIO_Port, DBG_LED0_Pin);
	}
  /* USER CODE END TIM3_IRQn 0 */
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */
  static EN_ENCODER_OVERFLOW enOverflow = EN_ENCODER_OVERFLOW_NONE;
  if(LL_TIM_IsActiveFlag_UPDATE(TIM5)){
    LL_TIM_ClearFlag_UPDATE(TIM5);
    if(LL_TIM_COUNTERDIRECTION_DOWN ==  LL_TIM_GetDirection(TIM5)){
      /* 0 -> CounterPeriod */
      enOverflow = EN_ENCODER_OVERFLOW_MINUS;
    }else{
      enOverflow = EN_ENCODER_OVERFLOW_PLUS;
    }
    //LL_GPIO_TogglePin(DBG_LED2_GPIO_Port, DBG_LED2_Pin);
    Encoder_Interrupt(EN_ENCODER_1, enOverflow);
  }
  /* USER CODE END TIM5_IRQn 0 */
  /* USER CODE BEGIN TIM5_IRQn 1 */

  /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */
	if( LL_DMA_IsActiveFlag_TC0(DMA2) == 1){
		  LL_DMA_ClearFlag_TC0(DMA2);
	}
  /* USER CODE END DMA2_Stream0_IRQn 0 */

  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */
  static uint16_t usAdcValue[4];
  static uint8_t ucWallLedNum = 0u;
  static Port_t *pWallLed;
  static bool bFirst = true;
  
  //転送完了フラグON
  if( LL_DMA_IsActiveFlag_TC2(DMA2) == 1){
		  LL_DMA_ClearFlag_TC2(DMA2);
      LL_GPIO_TogglePin(DBG_LED1_GPIO_Port, DBG_LED1_Pin);
      //Adc_GetAdcValues(EN_ADC_NUM_2, usAdcValue, 1, 4);
      //printf("%d, %d, %d, %d\n", usAdcValue[0], usAdcValue[1], usAdcValue[2], usAdcValue[3]);
      /*
      printf("%d, %d, %d, %d\n", Adc_GetAdcChannelValue(EN_ADC_NUM_2, 1)
                              ,Adc_GetAdcChannelValue(EN_ADC_NUM_2, 2)
                              ,Adc_GetAdcChannelValue(EN_ADC_NUM_2, 3)
                              ,Adc_GetAdcChannelValue(EN_ADC_NUM_2, 4));*/
      pWallLed = &stWallLedTbl[ucWallLedNum];

      if(0u == LL_GPIO_IsOutputPinSet(pWallLed->pGPIOx, pWallLed->uiPinNum)){
        if(!bFirst){
          WallSensor_SetValueLedOff((EN_WALLSENSOR_POS)ucWallLedNum, Adc_GetAdcChannelValue(EN_ADC_NUM_2, ucWallLedNum+1));
        }
        LL_GPIO_SetOutputPin(pWallLed->pGPIOx, pWallLed->uiPinNum);
      }else{
        WallSensor_SetValueLedOn((EN_WALLSENSOR_POS)ucWallLedNum, Adc_GetAdcChannelValue(EN_ADC_NUM_2, ucWallLedNum+1));
        LL_GPIO_ResetOutputPin(pWallLed->pGPIOx, pWallLed->uiPinNum);
        ucWallLedNum ++;
        if(sizeof(stWallLedTbl)/ sizeof(Port_t) <= ucWallLedNum){
          ucWallLedNum = 0;
          bFirst = false;
        }
      }
	}
  /* USER CODE END DMA2_Stream2_IRQn 0 */

  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
