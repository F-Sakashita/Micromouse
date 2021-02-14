/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include <stdbool.h>

uint16_t g_usAdc1Value[ADC1_CHANNEL_NUM];
bool g_bAdc1Start = false;
static bool Adc_IsValidReadChannel(uint8_t ucRequestRank, uint8_t ucChannelSize);
static bool Adc_IsValidReadSize(uint8_t ucStartRank, uint8_t ucRequestSize, uint8_t ucChannelSize, uint8_t *pSize);
/* USER CODE END 0 */

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**ADC1 GPIO Configuration
  PA4   ------> ADC1_IN4
  PA6   ------> ADC1_IN6
  PA7   ------> ADC1_IN7
  PB0   ------> ADC1_IN8
  PB1   ------> ADC1_IN9
  */
  GPIO_InitStruct.Pin = BAT_MONI_Pin|WALL_SEN0_Pin|WALL_SEN1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = WALL_SEN2_Pin|WALL_SEN3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* ADC1 DMA Init */

  /* ADC1 Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_0, LL_DMA_CHANNEL_0);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_0, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_0, LL_DMA_PRIORITY_MEDIUM);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_0, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_HALFWORD);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_0);

  /** Common config
  */
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_5RANKS;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_SEQUENCE_CONV);
  ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
  ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_480CYCLES);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_6);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_6, LL_ADC_SAMPLINGTIME_480CYCLES);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_7);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_480CYCLES);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_4, LL_ADC_CHANNEL_8);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_8, LL_ADC_SAMPLINGTIME_480CYCLES);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_5, LL_ADC_CHANNEL_9);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_480CYCLES);

}

/* USER CODE BEGIN 1 */
static bool Adc_IsValidReadChannel(uint8_t ucRequestRank, uint8_t ucChannelSize)
{
  if(ucRequestRank <= 0 || ucRequestRank > ucChannelSize){
    return false;
  }
  return true;
}

static bool Adc_IsValidReadSize(uint8_t ucStartRank, uint8_t ucRequestSize, uint8_t ucChannelSize, uint8_t *pSize)
{
  if(!Adc_IsValidReadChannel(ucStartRank, ucChannelSize)){
    return false;
  }

  if(ucRequestSize > ucChannelSize - (ucStartRank-1)){
    *pSize = ucChannelSize - (ucStartRank-1);
  }else{
    *pSize = ucRequestSize;
  }

  return true;
}

void Adc1_StartConvert(void)
{
  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0);
  LL_ADC_Enable(ADC1);
  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);
  LL_DMA_ConfigAddresses( DMA2, //DMA Channel
                          LL_DMA_STREAM_0, //DMA Stream
                          LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA), //ADC Register(Source) 
                          (uint32_t)&g_usAdc1Value, //Destination
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY //Direction
                          ); 
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, ADC1_CHANNEL_NUM);
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
  LL_ADC_REG_StartConversionSWStart(ADC1);
  g_bAdc1Start = true;
}

bool Adc1_IsStartConvert(void)
{
  return g_bAdc1Start;
}

uint16_t Adc1_GetAdcChannelValue(uint8_t ucChannelRank)
{
  if(!Adc_IsValidReadChannel(ucChannelRank, ADC1_CHANNEL_NUM)){
    return 0;
  }
  return g_usAdc1Value[ucChannelRank-1];
}

void Adc1_GetAdcValues(uint16_t *pValue, uint8_t ucStartChannelRank, uint8_t ucRequestSize)
{
  uint8_t ucSize = 0;
  if(!Adc_IsValidReadSize(ucStartChannelRank, ucRequestSize, ADC1_CHANNEL_NUM, &ucSize)){
    return;
  }
  memcpy(pValue, &g_usAdc1Value[ucStartChannelRank-1], ucSize);
}

float Adc1_GetRateAdcChannelValue(uint8_t ucChannelRank)
{
  if(!Adc_IsValidReadChannel(ucChannelRank, ADC1_CHANNEL_NUM)){
    return 0.0;
  }
  uint16_t uiResolutionValue = Adc_GetResolutionValue(ADC1);
  float fValue = (float)g_usAdc1Value[ucChannelRank-1] / (float)(uiResolutionValue);
  return fValue;
}

void Adc1_GetRateAdcValues(float *pValue, uint8_t ucStartChannelRank, uint8_t ucRequestSize)
{
  uint16_t uiResolutionValue = Adc_GetResolutionValue(ADC1);

  uint8_t ucSize = 0;
  if(!Adc_IsValidReadSize(ucStartChannelRank, ucRequestSize, ADC1_CHANNEL_NUM, &ucSize)){
    return;
  }
  uint16_t *pAdcValue = &g_usAdc1Value[ucStartChannelRank-1];
  while(ucSize > 0){
    *pValue = (float)(*pAdcValue) / (float)(uiResolutionValue);
    pValue ++;
    pAdcValue ++;
    ucSize --;
  }
}

uint16_t Adc_GetResolutionValue(ADC_TypeDef *ADCx)
{
  uint32_t uiResolution = LL_ADC_GetResolution(ADCx);
  uint16_t uiResolutionValue = 0x0FFF;
  switch(uiResolution) {
  case LL_ADC_RESOLUTION_12B:
    uiResolutionValue = 0x0FFF;
    break;
  case LL_ADC_RESOLUTION_10B:
    uiResolutionValue = 0x03FF;
    break;
  case LL_ADC_RESOLUTION_8B:
    uiResolutionValue = 0x00FF;
    break;
  case LL_ADC_RESOLUTION_6B:
    uiResolutionValue = 0x003F;
    break;
  default:
    /* do nothing */
    break;
  }
  return uiResolutionValue;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
