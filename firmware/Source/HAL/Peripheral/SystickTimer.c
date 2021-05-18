/*
 * SystickTimer.c
 *
 *  Created on: Dec 27, 2020
 *      Author: fumihiko-mbp
 */


#include "SystickTimer.h"

/* STM32 Driver */
#ifdef ENABLE_FREERTOS
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#else
#include "stm32f4xx_ll_utils.h"		//LL_mDelay()
#include "stm32f4xx_ll_cortex.h"	//LL_SYSTICK_EnableIT()
#endif

uint16_t __usSamplingTimeMs = 0;
uint64_t __ullSystickTimeCount = 0;

#define SYSTICKTIMER_MS_PER_COUNT	(1)		//1Countが何msecかを定義(1ms以上)

//割り込み関数用タイマーカウント関数
void SystickTimer_Interrupt(void)
{
	__ullSystickTimeCount += SYSTICKTIMER_MS_PER_COUNT;
}

//割り込み有効
void SystickTimer_EnableInterrupt(void)
{
    #ifdef ENABLE_FREERTOS
    HAL_ResumeTick();
    #else
	LL_SYSTICK_EnableIT();
    #endif
}

//割り込み無効
void SystickTimer_DisableInterrpt(void)
{
    #ifdef ENABLE_FREERTOS
    HAL_SuspendTick();
    #else
	LL_SYSTICK_DisableIT();
    #endif
}

//サンプリングタイムの設定
void SystickTimer_SetSamplingTime(uint16_t usSamplingTimeMs)
{
	if(0u == usSamplingTimeMs){
		//0指定の場合はデフォルト値を設定
		__usSamplingTimeMs = SYSTICKTIMER_DEFAULT_SAMPLING_TIME_MS;
	}else{
		__usSamplingTimeMs = usSamplingTimeMs;
	}
}
//サンプリングタイムの経過判定
bool SystickTimer_IsSamplingTimeElapsed(void)
{
	//常に0以下の場合はFalseを即Return
	if(__ullSystickTimeCount <= 0u){
		return false;
	}
	static uint64_t ullLastTimeMs = 0;
	uint64_t ullCurrentTimeMs = __ullSystickTimeCount;

	if(__usSamplingTimeMs <= ullCurrentTimeMs - ullLastTimeMs){
		ullLastTimeMs = ullCurrentTimeMs;
		return true;
	}
	return false;
}

//起動時からの時間取得 [ms]
uint64_t SystickTimer_GetTimeMS(void)
{
	return __ullSystickTimeCount;
}

//遅延 [ms]
void SystickTimer_DelayMS(uint32_t uiDelayMs)
{
    #ifdef ENABLE_FREERTOS
    if(osKernelRunning == osKernelGetState()){
        osDelay(uiDelayMs);
    }else{
        HAL_Delay(uiDelayMs);
    }

    #else
	LL_mDelay(uiDelayMs);
    #endif
}

//指定時間経過
bool SystickTimer_IsTimeElapsed(uint64_t ullStartTimeMs ,uint32_t uiIntervalMs)
{
	if((uint64_t)uiIntervalMs <= SystickTimer_GetTimeMS() - ullStartTimeMs){
		return true;
	}
	return false;
}
