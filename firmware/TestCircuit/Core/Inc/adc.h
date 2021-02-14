/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define ADC1_CHANNEL_NUM	(5)
/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void Adc1_StartConvert(void);
bool Adc1_IsStartConvert(void);
uint16_t Adc1_GetAdcChannelValue(uint8_t ucChannelRank);
void Adc1_GetAdcValues(uint16_t *pValue, uint8_t ucStartChannelRank, uint8_t ucRequestSize);
float Adc1_GetRateAdcChannelValue(uint8_t ucChannelRank);
void Adc1_GetRateAdcValues(float *pValue, uint8_t ucStartChannelRank, uint8_t ucRequestSize);

uint16_t Adc_GetResolutionValue(ADC_TypeDef *ADCx);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
