/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SystickTimer.h"
#include "DebugTask.h"
#include "WallDetectTask.h"
#include "ApplicationTask.h"
#include "arm_math.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for ApplicationTask */
osThreadId_t ApplicationTaskHandle;
uint32_t ApplicationTaskBuffer[ 2048 ];
osStaticThreadDef_t ApplicationTaskControlBlock;
const osThreadAttr_t ApplicationTask_attributes = {
  .name = "ApplicationTask",
  .cb_mem = &ApplicationTaskControlBlock,
  .cb_size = sizeof(ApplicationTaskControlBlock),
  .stack_mem = &ApplicationTaskBuffer[0],
  .stack_size = sizeof(ApplicationTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for DebugTask */
osThreadId_t DebugTaskHandle;
uint32_t DebugTaskBuffer[ 2048 ];
osStaticThreadDef_t DebugTaskControlBlock;
const osThreadAttr_t DebugTask_attributes = {
  .name = "DebugTask",
  .cb_mem = &DebugTaskControlBlock,
  .cb_size = sizeof(DebugTaskControlBlock),
  .stack_mem = &DebugTaskBuffer[0],
  .stack_size = sizeof(DebugTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for LedBlinkTask */
osThreadId_t LedBlinkTaskHandle;
const osThreadAttr_t LedBlinkTask_attributes = {
  .name = "LedBlinkTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for WallDetectTask */
osThreadId_t WallDetectTaskHandle;
uint32_t WallDetectTaskBuffer[ 512 ];
osStaticThreadDef_t WallDetectTaskControlBlock;
const osThreadAttr_t WallDetectTask_attributes = {
  .name = "WallDetectTask",
  .cb_mem = &WallDetectTaskControlBlock,
  .cb_size = sizeof(WallDetectTaskControlBlock),
  .stack_mem = &WallDetectTaskBuffer[0],
  .stack_size = sizeof(WallDetectTaskBuffer),
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for DebugQueue */
osMessageQueueId_t DebugQueueHandle;
uint8_t DebugQueueBuffer[ 100 * sizeof( DebugQueue_t ) ];
osStaticMessageQDef_t DebugQueueControlBlock;
const osMessageQueueAttr_t DebugQueue_attributes = {
  .name = "DebugQueue",
  .cb_mem = &DebugQueueControlBlock,
  .cb_size = sizeof(DebugQueueControlBlock),
  .mq_mem = &DebugQueueBuffer,
  .mq_size = sizeof(DebugQueueBuffer)
};
/* Definitions for EventDebugQueuePush */
osEventFlagsId_t EventDebugQueuePushHandle;
osStaticEventGroupDef_t EventDebugQueuePushControlBlock;
const osEventFlagsAttr_t EventDebugQueuePush_attributes = {
  .name = "EventDebugQueuePush",
  .cb_mem = &EventDebugQueuePushControlBlock,
  .cb_size = sizeof(EventDebugQueuePushControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ApplicationTaskLoop(void *argument);
void DebugTaskLoop(void *argument);
void LedBlinkTaskLoop(void *argument);
void WallDetectTaskLoop(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of DebugQueue */
  DebugQueueHandle = osMessageQueueNew (100, sizeof(DebugQueue_t), &DebugQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ApplicationTask */
  ApplicationTaskHandle = osThreadNew(ApplicationTaskLoop, NULL, &ApplicationTask_attributes);

  /* creation of DebugTask */
  DebugTaskHandle = osThreadNew(DebugTaskLoop, NULL, &DebugTask_attributes);

  /* creation of LedBlinkTask */
  LedBlinkTaskHandle = osThreadNew(LedBlinkTaskLoop, NULL, &LedBlinkTask_attributes);

  /* creation of WallDetectTask */
  WallDetectTaskHandle = osThreadNew(WallDetectTaskLoop, NULL, &WallDetectTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of EventDebugQueuePush */
  EventDebugQueuePushHandle = osEventFlagsNew(&EventDebugQueuePush_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  SystickTimer_EnableInterrupt();
  bool bInitialize = true;
  bInitialize &= DebugTask_Initialize(DebugQueueHandle, &EventDebugQueuePushHandle);
  bInitialize &= WallDetectTask_Initialize();
  bInitialize &= ApplicationTask_Initialize();
  DebugConsole_Printf("Task Initialized : %d\n", bInitialize);
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ApplicationTaskLoop */
/**
  * @brief  Function implementing the ApplicationTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ApplicationTaskLoop */
void ApplicationTaskLoop(void *argument)
{
  /* USER CODE BEGIN ApplicationTaskLoop */
  /* Infinite loop */
  for(;;)
  {
    ApplicationTask_Update();
  }
  /* USER CODE END ApplicationTaskLoop */
}

/* USER CODE BEGIN Header_DebugTaskLoop */
/**
* @brief Function implementing the DebugTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DebugTaskLoop */
void DebugTaskLoop(void *argument)
{
  /* USER CODE BEGIN DebugTaskLoop */
  /* Infinite loop */
  for(;;)
  {
	  DebugTask_Update();
	  //osDelay(100);
  }
  /* USER CODE END DebugTaskLoop */
}

/* USER CODE BEGIN Header_LedBlinkTaskLoop */
/**
* @brief Function implementing the LedBlinkTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LedBlinkTaskLoop */
void LedBlinkTaskLoop(void *argument)
{
  /* USER CODE BEGIN LedBlinkTaskLoop */
  /* Infinite loop */
  for(;;)
  {
	uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + 500u);
    if(!DebugQueue_IsFull()){
        //char str[] = "Running.";
        //DebugQueue_Push(str, sizeof(str), 1, 0);
        LL_GPIO_TogglePin(TICK_LED_GPIO_Port, TICK_LED_Pin);
    }
  }
  /* USER CODE END LedBlinkTaskLoop */
}

/* USER CODE BEGIN Header_WallDetectTaskLoop */
/**
* @brief Function implementing the WallDetectTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WallDetectTaskLoop */
void WallDetectTaskLoop(void *argument)
{
  /* USER CODE BEGIN WallDetectTaskLoop */
  /* Infinite loop */
  for(;;)
  {
    WallDetectTask_Update();
  }
  /* USER CODE END WallDetectTaskLoop */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
