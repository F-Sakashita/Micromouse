/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include "arm_math.h"
#include "SystickTimer.h"
#include "Button.hpp"
#include "Blink.hpp"
#include "BatteryMonitor.hpp"
#include "ICM_20602.hpp"
#include "WallSensor.h"
#include "Encoder.h"
#include "TB6612FNG.hpp"
#include <vector>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG_PRINTF_USART_TYPEDEF	USART2
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

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  SystickTimer_EnableInterrupt();
  SystickTimer_SetSamplingTime(1u);


  LL_GPIO_SetOutputPin(WALL_LED0_GPIO_Port, WALL_LED0_Pin);
  LL_GPIO_ResetOutputPin(WALL_LED1_GPIO_Port, WALL_LED1_Pin);
  LL_GPIO_ResetOutputPin(WALL_LED2_GPIO_Port, WALL_LED2_Pin);
  LL_GPIO_ResetOutputPin(WALL_LED3_GPIO_Port, WALL_LED3_Pin);

  LL_ADC_Enable(ADC2);			//ADC2 Enabled
  //LL_ADC_EnableIT_EOCS(ADC2);	//ADC2 Interrupt Start

  LL_TIM_EnableIT_UPDATE(TIM3);	//TIM3 Interrupt Start
  LL_TIM_EnableCounter(TIM3);	//TIM3 Count Up Start
  
  Adc_StartConvert(EN_ADC_NUM_2);

  Button Sw[2] = {
    Button(SW0_GPIO_Port, SW0_Pin),
    Button(SW1_GPIO_Port, SW1_Pin),
  };
  Sw[0].SetPushReverse();
  Sw[1].SetPushReverse();
  Blink TickLed(TICK_LED_GPIO_Port, TICK_LED_Pin, 1000);
  TickLed.SetStartState(false);
  /*
  Blink DbgLed[3] = {
    Blink(DBG_LED0_GPIO_Port, DBG_LED0_Pin),
    Blink(DBG_LED1_GPIO_Port, DBG_LED1_Pin),
    Blink(DBG_LED2_GPIO_Port, DBG_LED2_Pin)
  };

  Blink Buzzer(BUZZER_GPIO_Port, BUZZER_Pin, 1000);
  Buzzer.Off();

  for(uint8_t ucCount = 0; ucCount < sizeof(DbgLed)/sizeof(Blink); ucCount++){
    DbgLed[ucCount].SetOnTime(250);
    DbgLed[ucCount].SetOffTime(500);
    DbgLed[ucCount].SetStartDelayTime(250*ucCount);
  }
  DbgLed[0].SetStartState(true);
  DbgLed[1].SetStartState(false);
  DbgLed[2].SetStartState(false);
  */

  /*
  Blink WallLed[4] = {
    Blink(WALL_LED0_GPIO_Port, WALL_LED0_Pin),
    Blink(WALL_LED1_GPIO_Port, WALL_LED1_Pin),
    Blink(WALL_LED2_GPIO_Port, WALL_LED2_Pin),
    Blink(WALL_LED3_GPIO_Port, WALL_LED3_Pin),
  };
  WallLed[0].SetOnTime(20);
  WallLed[0].SetOffTime(2000);
  WallLed[1].SetOnTime(20);
  WallLed[1].SetOffTime(2000);
  WallLed[2].SetOnTime(20);
  WallLed[2].SetOffTime(2000);
  WallLed[3].SetOnTime(20);
  WallLed[3].SetOffTime(2000);
  */

  BatteryMonitor BatteryMoni;
  ICM_20602 IMU(ICM_20602::EN_COMM_MODE_SPI);
  IMU.SetSPIPort(SPI1, IMU_CS_GPIO_Port, IMU_CS_Pin);
  IMU.StartGyroOffestCalc(2000, true, true, true);
  IMU.Setup(2);

  
  Encoder_Setup(EN_ENCODER_0, TIM2, 0xFFFFFFFF);
  Encoder_Setup(EN_ENCODER_1, TIM5, 0xFFFFFFFF);

  TB6612FNG *pMotorDriver = TB6612FNG::GetInstance();
  pMotorDriver->Initialize(0.5f);

  uint64_t ullDebugTimeMs = SystickTimer_GetTimeMS();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(SystickTimer_IsSamplingTimeElapsed()){
		  Sw[0].Update();
      Sw[1].Update();
      BatteryMoni.Update();
      IMU.Update();
      WallSensor_Update();
      Encoder_Update(EN_ENCODER_0);
      Encoder_Update(EN_ENCODER_1);


      if(1 == Sw[0].IsPushCount() % 2){
        TickLed.SetPeriod(500);
      }else{
        TickLed.SetPeriod(1000);
      }

      uint32_t uiSwCount = Sw[1].IsPushCount() % 5;
      switch(uiSwCount){
        case 0:
          pMotorDriver->Stop(TB6612FNG::EN_MOTOR_CH_A);
          pMotorDriver->Stop(TB6612FNG::EN_MOTOR_CH_B);
          break;
        case 1:
          pMotorDriver->SetDuty(TB6612FNG::EN_MOTOR_CH_A, 0.1f);
          pMotorDriver->SetDuty(TB6612FNG::EN_MOTOR_CH_B, 0.0f);
          break;
        case 2:
          pMotorDriver->SetDuty(TB6612FNG::EN_MOTOR_CH_A, -0.1f);
          pMotorDriver->SetDuty(TB6612FNG::EN_MOTOR_CH_B, 0.0f);
          break;
        case 3:
          pMotorDriver->SetDuty(TB6612FNG::EN_MOTOR_CH_A, 0.0f);
          pMotorDriver->SetDuty(TB6612FNG::EN_MOTOR_CH_B, 0.1f);
          break;
        case 4:
          pMotorDriver->SetDuty(TB6612FNG::EN_MOTOR_CH_A, 0.0f);
          pMotorDriver->SetDuty(TB6612FNG::EN_MOTOR_CH_B, -0.1f);
          break;  
        default:
          pMotorDriver->Stop(TB6612FNG::EN_MOTOR_CH_A);
          pMotorDriver->Stop(TB6612FNG::EN_MOTOR_CH_B);
          break;
      }

      

		  if(SystickTimer_IsTimeElapsed(ullDebugTimeMs, 5)){
			  ullDebugTimeMs = SystickTimer_GetTimeMS();
        float fTimer = (float)SystickTimer_GetTimeMS()/1000.0f;
        uint16_t usAdcValue[4];
        float fAdcRate[4];
        //printf("TIM3 Counter %d\n", LL_TIM_GetCounter(TIM3));
        //Adc1_GetAdcValues(usAdcValue, 2, 4);
        //Adc1_GetRateAdcValues(fAdcRate, 2, 4);
        //fValue*3.3 = Vbat * 10k / (10k+20k)
        //Vbat = fValue*3.3*30k/10k
        WallSensor_GetValue(usAdcValue, 4);
        //printf("AD:%d\n", Adc_GetAdcChannelValue(EN_ADC_NUM_2, 1));
       //printf("Enc0, %ld, %ld, %ld, Enc1, %ld, %ld, %ld\n", 	LL_TIM_GetCounter(TIM2),  Encoder_IsRawCount(EN_ENCODER_0), Encoder_IsCount(EN_ENCODER_0),
    	//	   								                          LL_TIM_GetCounter(TIM5),  Encoder_IsRawCount(EN_ENCODER_1), Encoder_IsCount(EN_ENCODER_1));
		//printf("enc(reg), %ld, %ld\n", LL_TIM_GetCounter(TIM2), LL_TIM_GetCounter(TIM5));
        //printf("enc(raw), %ld, %ld\n", Encoder_IsRawCount(EN_ENCODER_0), Encoder_IsRawCount(EN_ENCODER_1));
        printf("enc, %ld, %ld\n", (int32_t)Encoder_IsCount(EN_ENCODER_0), (int32_t)Encoder_IsCount(EN_ENCODER_1));
        /*printf("t,%.3f, sin,%.3f, sw0,%d, Vbat:%.3f GyroZ:%.3f, Yaw[deg],%.3f\n",
                fTimer,
                arm_sin_f32(fTimer),
                Sw[0].IsPushCount(),
                BatteryMoni.GetVoltage(),
                IMU.GetGyroDPS().fValueZ,
                IMU.GetGyroDeg().fValueZ);
*/
        //if(Adc1_GetAdcChannelValue(2) > 1000){
          //printf("%d\n", Adc1_GetAdcChannelValue(2));
          //printf("%f, %f, %f, %f\n", fAdcRate[0], fAdcRate[1], fAdcRate[2], fAdcRate[3]);
        //}

		  }

      TickLed.Update();
      pMotorDriver->Update();
	  } //SamplingTime Elapsed
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_8, 168, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(168000000);
  LL_SetSystemCoreClock(168000000);
}

/* USER CODE BEGIN 4 */
int __io_putchar(int ch)
{
	USART_TransmitByte(DEBUG_PRINTF_USART_TYPEDEF, (uint8_t)ch);
	return ch;
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
