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
#include "MainTask.h"
#include "VelControlTask.h"
#include "TrajControlTask.h"
#include "arm_math.h"
#include "MessageQueueType.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
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
/* Definitions for MainTask */
osThreadId_t MainTaskHandle;
uint32_t MainTaskBuffer[ 2048 ];
osStaticThreadDef_t MainTaskControlBlock;
const osThreadAttr_t MainTask_attributes = {
  .name = "MainTask",
  .cb_mem = &MainTaskControlBlock,
  .cb_size = sizeof(MainTaskControlBlock),
  .stack_mem = &MainTaskBuffer[0],
  .stack_size = sizeof(MainTaskBuffer),
  .priority = (osPriority_t) osPriorityAboveNormal1,
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
  .priority = (osPriority_t) osPriorityNormal,
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
/* Definitions for VelControlTask */
osThreadId_t VelControlTaskHandle;
uint32_t VelControlTaskBuffer[ 512 ];
osStaticThreadDef_t VelControlTaskControlBlock;
const osThreadAttr_t VelControlTask_attributes = {
  .name = "VelControlTask",
  .cb_mem = &VelControlTaskControlBlock,
  .cb_size = sizeof(VelControlTaskControlBlock),
  .stack_mem = &VelControlTaskBuffer[0],
  .stack_size = sizeof(VelControlTaskBuffer),
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for TrajControlTask */
osThreadId_t TrajControlTaskHandle;
uint32_t TrajControlTaskBuffer[ 512 ];
osStaticThreadDef_t TrajControlTaskControlBlock;
const osThreadAttr_t TrajControlTask_attributes = {
  .name = "TrajControlTask",
  .cb_mem = &TrajControlTaskControlBlock,
  .cb_size = sizeof(TrajControlTaskControlBlock),
  .stack_mem = &TrajControlTaskBuffer[0],
  .stack_size = sizeof(TrajControlTaskBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for DebugQueue */
osMessageQueueId_t DebugQueueHandle;
uint8_t DebugQueueBuffer[ 100 * sizeof( DebugMsg_t ) ];
osStaticMessageQDef_t DebugQueueControlBlock;
const osMessageQueueAttr_t DebugQueue_attributes = {
  .name = "DebugQueue",
  .cb_mem = &DebugQueueControlBlock,
  .cb_size = sizeof(DebugQueueControlBlock),
  .mq_mem = &DebugQueueBuffer,
  .mq_size = sizeof(DebugQueueBuffer)
};
/* Definitions for OdometoryPosQueue */
osMessageQueueId_t OdometoryPosQueueHandle;
uint8_t OdometoryPosQueueBuffer[ 10 * sizeof( OdometoryMsg_t ) ];
osStaticMessageQDef_t OdometoryPosQueueControlBlock;
const osMessageQueueAttr_t OdometoryPosQueue_attributes = {
  .name = "OdometoryPosQueue",
  .cb_mem = &OdometoryPosQueueControlBlock,
  .cb_size = sizeof(OdometoryPosQueueControlBlock),
  .mq_mem = &OdometoryPosQueueBuffer,
  .mq_size = sizeof(OdometoryPosQueueBuffer)
};
/* Definitions for OdometoryVelQueue */
osMessageQueueId_t OdometoryVelQueueHandle;
uint8_t OdometoryVelQueueBuffer[ 10 * sizeof( OdometoryMsg_t ) ];
osStaticMessageQDef_t OdometoryVelQueueControlBlock;
const osMessageQueueAttr_t OdometoryVelQueue_attributes = {
  .name = "OdometoryVelQueue",
  .cb_mem = &OdometoryVelQueueControlBlock,
  .cb_size = sizeof(OdometoryVelQueueControlBlock),
  .mq_mem = &OdometoryVelQueueBuffer,
  .mq_size = sizeof(OdometoryVelQueueBuffer)
};
/* Definitions for PosCmdQueue */
osMessageQueueId_t PosCmdQueueHandle;
uint8_t PosCmdQueueBuffer[ 10 * sizeof( PosControlCmdMsg_t ) ];
osStaticMessageQDef_t PosCmdQueueControlBlock;
const osMessageQueueAttr_t PosCmdQueue_attributes = {
  .name = "PosCmdQueue",
  .cb_mem = &PosCmdQueueControlBlock,
  .cb_size = sizeof(PosCmdQueueControlBlock),
  .mq_mem = &PosCmdQueueBuffer,
  .mq_size = sizeof(PosCmdQueueBuffer)
};
/* Definitions for VelCmdQueue */
osMessageQueueId_t VelCmdQueueHandle;
uint8_t VelCmdQueueBuffer[ 10 * sizeof( VelControlCmdMsg_t ) ];
osStaticMessageQDef_t VelCmdQueueControlBlock;
const osMessageQueueAttr_t VelCmdQueue_attributes = {
  .name = "VelCmdQueue",
  .cb_mem = &VelCmdQueueControlBlock,
  .cb_size = sizeof(VelCmdQueueControlBlock),
  .mq_mem = &VelCmdQueueBuffer,
  .mq_size = sizeof(VelCmdQueueBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MainTaskLoop(void *argument);
void DebugTaskLoop(void *argument);
void WallDetectTaskLoop(void *argument);
void VelControlTaskLoop(void *argument);
void TrajControlTaskLoop(void *argument);

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
  DebugQueueHandle = osMessageQueueNew (100, sizeof(DebugMsg_t), &DebugQueue_attributes);

  /* creation of OdometoryPosQueue */
  OdometoryPosQueueHandle = osMessageQueueNew (10, sizeof(OdometoryMsg_t), &OdometoryPosQueue_attributes);

  /* creation of OdometoryVelQueue */
  OdometoryVelQueueHandle = osMessageQueueNew (10, sizeof(OdometoryMsg_t), &OdometoryVelQueue_attributes);

  /* creation of PosCmdQueue */
  PosCmdQueueHandle = osMessageQueueNew (10, sizeof(PosControlCmdMsg_t), &PosCmdQueue_attributes);

  /* creation of VelCmdQueue */
  VelCmdQueueHandle = osMessageQueueNew (10, sizeof(VelControlCmdMsg_t), &VelCmdQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of MainTask */
  MainTaskHandle = osThreadNew(MainTaskLoop, NULL, &MainTask_attributes);

  /* creation of DebugTask */
  DebugTaskHandle = osThreadNew(DebugTaskLoop, NULL, &DebugTask_attributes);

  /* creation of WallDetectTask */
  WallDetectTaskHandle = osThreadNew(WallDetectTaskLoop, NULL, &WallDetectTask_attributes);

  /* creation of VelControlTask */
  VelControlTaskHandle = osThreadNew(VelControlTaskLoop, NULL, &VelControlTask_attributes);

  /* creation of TrajControlTask */
  TrajControlTaskHandle = osThreadNew(TrajControlTaskLoop, NULL, &TrajControlTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  SystickTimer_EnableInterrupt();
  bool bInitialize = true;

  DebugTask_OsFunc_t debugTask_OsFunc;
  debugTask_OsFunc.TxQueueId = DebugQueueHandle;
  bInitialize &= DebugTask_Initialize(&debugTask_OsFunc);
  
  MainTask_OsFunc_t mainTask_OsFunc;
  mainTask_OsFunc.PosCmdQueueId = PosCmdQueueHandle;
  bInitialize &= MainTask_Initialize(&mainTask_OsFunc);
  SystickTimer_DelayMS(50u);

  bInitialize &= WallDetectTask_Initialize(NULL);
  SystickTimer_DelayMS(50u);

  TrajControlTask_OsFunc_t trajControlTask_OsFunc;
  trajControlTask_OsFunc.OdometoryPosQueueId = OdometoryPosQueueHandle;
  trajControlTask_OsFunc.VelCmdQueueId = VelCmdQueueHandle;
  bInitialize &= TrajControlTask_Initialize(&trajControlTask_OsFunc);
  SystickTimer_DelayMS(50u);

  VelControlTask_OsFunc_t velControlTask_OsFunc;
  velControlTask_OsFunc.OdometoryVelQueueId = OdometoryVelQueueHandle;
  bInitialize &= VelControlTask_Initialize(&velControlTask_OsFunc);
  SystickTimer_DelayMS(50u);

  DebugConsole_Printf("\nHello World!\n");
  DebugConsole_Printf("RTOS Tasks Initialized : %d\n", bInitialize);
  SystickTimer_DelayMS(3000u);
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_MainTaskLoop */
/**
  * @brief  Function implementing the MainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_MainTaskLoop */
void MainTaskLoop(void *argument)
{
  /* USER CODE BEGIN MainTaskLoop */
  /* Infinite loop */
  for(;;)
  {
    MainTask_Update();
  }
  /* USER CODE END MainTaskLoop */
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

/* USER CODE BEGIN Header_VelControlTaskLoop */
/**
* @brief Function implementing the VelControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_VelControlTaskLoop */
void VelControlTaskLoop(void *argument)
{
  /* USER CODE BEGIN VelControlTaskLoop */
  /* Infinite loop */
  for(;;)
  {
    VelControlTask_Update();
  }
  /* USER CODE END VelControlTaskLoop */
}

/* USER CODE BEGIN Header_TrajControlTaskLoop */
/**
* @brief Function implementing the TrajControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TrajControlTaskLoop */
void TrajControlTaskLoop(void *argument)
{
  /* USER CODE BEGIN TrajControlTaskLoop */
  /* Infinite loop */
  for(;;)
  {
    TrajControlTask_Update();
  }
  /* USER CODE END TrajControlTaskLoop */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
