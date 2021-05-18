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
#include <stdbool.h>

typedef struct{
  EN_ADC_NUM enAdcNum;
  ADC_TypeDef *pADCx;
  uint16_t *pAdcValue;
  uint8_t   ucAdcChannelNum;
}AdcConfig_t;

uint16_t g_usAdc1Value[ADC1_CHANNEL_NUM];
bool g_bAdc1Start = false;
uint16_t g_usAdc2Value[ADC2_CHANNEL_NUM];
bool g_bAdc2Start = false;

static const AdcConfig_t g_AdcConfigTable[] = {
  {EN_ADC_NUM_1, ADC1, g_usAdc1Value, ADC1_CHANNEL_NUM},
  {EN_ADC_NUM_2, ADC2, g_usAdc2Value, ADC2_CHANNEL_NUM},
};

static bool Adc_IsValidReadChannel(uint8_t ucRequestRank, uint8_t ucChannelSize);
static bool Adc_IsValidReadSize(uint8_t ucStartRank, uint8_t ucRequestSize, uint8_t ucChannelSize, uint8_t *pSize);
static void Adc1_StartConvert(void);
static void Adc2_StartConvert(void);

/* 内部関数 */
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

static void Adc1_StartConvert(void)
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

static void Adc2_StartConvert(void)
{
  LL_ADC_Enable(ADC2);			//ADC2 Enabled
  
  LL_TIM_EnableIT_UPDATE(TIM3);	//TIM3 Interrupt Start
  LL_TIM_EnableCounter(TIM3);	//TIM3 Count Up Start

  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
  //LL_ADC_Enable(ADC2);
  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
  LL_DMA_ConfigAddresses( DMA2, //DMA Channel
                          LL_DMA_STREAM_2, //DMA Stream
                          LL_ADC_DMA_GetRegAddr(ADC2, LL_ADC_DMA_REG_REGULAR_DATA), //ADC Register(Source) 
                          (uint32_t)&g_usAdc2Value, //Destination
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY //Direction
                          ); 
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, ADC2_CHANNEL_NUM);
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);

  g_bAdc2Start = true;
}
/* USER CODE END 0 */

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**ADC1 GPIO Configuration
  PA4   ------> ADC1_IN4
  */
  GPIO_InitStruct.Pin = BAT_MONI_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(BAT_MONI_GPIO_Port, &GPIO_InitStruct);

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

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
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
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}
/* ADC2 init function */
void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**ADC2 GPIO Configuration
  PA6   ------> ADC2_IN6
  PA7   ------> ADC2_IN7
  PB0   ------> ADC2_IN8
  PB1   ------> ADC2_IN9
  */
  GPIO_InitStruct.Pin = WALL_SEN1_Pin|WALL_SEN0_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = WALL_SEN2_Pin|WALL_SEN3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* ADC2 DMA Init */

  /* ADC2 Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_2, LL_DMA_CHANNEL_1);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_2, LL_DMA_PRIORITY_VERYHIGH);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_2, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_2, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_2, LL_DMA_MDATAALIGN_HALFWORD);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_2);

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */
  /** Common config
  */
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
  LL_ADC_Init(ADC2, &ADC_InitStruct);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_EXT_TIM3_TRGO;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_4RANKS;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
  LL_ADC_REG_Init(ADC2, &ADC_REG_InitStruct);
  LL_ADC_REG_SetFlagEndOfConversion(ADC2, LL_ADC_REG_FLAG_EOC_SEQUENCE_CONV);
  LL_ADC_REG_StartConversionExtTrig(ADC2, LL_ADC_REG_TRIG_EXT_RISING);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC2, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_7);
  LL_ADC_SetChannelSamplingTime(ADC2, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_84CYCLES);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC2, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_6);
  LL_ADC_SetChannelSamplingTime(ADC2, LL_ADC_CHANNEL_6, LL_ADC_SAMPLINGTIME_84CYCLES);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC2, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_8);
  LL_ADC_SetChannelSamplingTime(ADC2, LL_ADC_CHANNEL_8, LL_ADC_SAMPLINGTIME_84CYCLES);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC2, LL_ADC_REG_RANK_4, LL_ADC_CHANNEL_9);
  LL_ADC_SetChannelSamplingTime(ADC2, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_84CYCLES);
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/* USER CODE BEGIN 1 */
/* 外部関数 */
void Adc_StartConvert(EN_ADC_NUM enAdcNum)
{
  switch(enAdcNum){
    case EN_ADC_NUM_1:
      Adc1_StartConvert();
      break;
    case EN_ADC_NUM_2:
      Adc2_StartConvert();
      break;
    default:
      break;
  }
}

bool Adc_IsStartConvert(EN_ADC_NUM enAdcNum)
{
  bool bResult = false;
  switch(enAdcNum){
    case EN_ADC_NUM_1:
      bResult = g_bAdc1Start;
      break;
    case EN_ADC_NUM_2:
      bResult = g_bAdc2Start;
      break;
    default:
      break;
  }
  return bResult;
}

uint16_t Adc_GetAdcChannelValue(EN_ADC_NUM enAdcNum, uint8_t ucChannelRank)
{
  if(enAdcNum >= EN_ADC_NUM_LAST){
    return 0;
  }
  const AdcConfig_t *pConfigTbl = &g_AdcConfigTable[enAdcNum];
  if(!Adc_IsValidReadChannel(ucChannelRank, pConfigTbl->ucAdcChannelNum)){
    return 0;
  }
  return (*(pConfigTbl->pAdcValue + ucChannelRank - 1));
}

void Adc_GetAdcValues(EN_ADC_NUM enAdcNum, uint16_t *pValue, uint8_t ucStartChannelRank, uint8_t ucRequestSize)
{
  uint8_t ucSize = 0;
  if(enAdcNum >= EN_ADC_NUM_LAST){
    return;
  }
  const AdcConfig_t *pConfigTbl = &g_AdcConfigTable[enAdcNum];

  if(!Adc_IsValidReadSize(ucStartChannelRank, ucRequestSize, pConfigTbl->ucAdcChannelNum, &ucSize)){
    return;
  }
  
  //memcpy(pValue, &g_usAdc1Value[ucStartChannelRank-1], ucSize);
  memcpy(pValue, (pConfigTbl->pAdcValue + ucStartChannelRank - 1), ucSize * sizeof(uint16_t));
  /*printf("Get:%d, %d, %d, %d\n", *(pConfigTbl->pAdcValue)
		  , *(pConfigTbl->pAdcValue+1)
		  , *(pConfigTbl->pAdcValue+2)
		  , *(pConfigTbl->pAdcValue+3));*/
}

float Adc_GetRateAdcChannelValue(EN_ADC_NUM enAdcNum, uint8_t ucChannelRank)
{
  if(enAdcNum >= EN_ADC_NUM_LAST){
    return 0;
  }
  const AdcConfig_t *pConfigTbl = &g_AdcConfigTable[enAdcNum];
  if(!Adc_IsValidReadChannel(ucChannelRank, pConfigTbl->ucAdcChannelNum)){
    return 0.0;
  }
  uint16_t uiResolutionValue = Adc_GetResolutionValue(enAdcNum);
  float fValue = (float)(*(pConfigTbl->pAdcValue + ucChannelRank - 1)) / (float)(uiResolutionValue);
  return fValue;
}

void Adc_GetRateAdcValues(EN_ADC_NUM enAdcNum, float *pValue, uint8_t ucStartChannelRank, uint8_t ucRequestSize)
{
  if(enAdcNum >= EN_ADC_NUM_LAST){
    return;
  }
  const AdcConfig_t *pConfigTbl = &g_AdcConfigTable[enAdcNum];
  uint16_t uiResolutionValue = Adc_GetResolutionValue(enAdcNum);
  uint8_t ucSize = 0;
  if(!Adc_IsValidReadSize(ucStartChannelRank, ucRequestSize, pConfigTbl->ucAdcChannelNum, &ucSize)){
    return;
  }
  uint16_t *pAdcValue = (pConfigTbl->pAdcValue + ucStartChannelRank - 1);
  while(ucSize > 0){
    *pValue = (float)(*pAdcValue) / (float)(uiResolutionValue);
    pValue ++;
    pAdcValue ++;
    ucSize --;
  }
}

uint16_t Adc_GetResolutionValue(EN_ADC_NUM enAdcNum)
{
  ADC_TypeDef *pADCx;
  switch (enAdcNum)
  {
  case EN_ADC_NUM_1:
    pADCx = ADC1;
    break;
  case EN_ADC_NUM_2:
    pADCx = ADC2;
    break;
  default:
    return 1u;
    break;
  }
  uint32_t uiResolution = LL_ADC_GetResolution(pADCx);
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
