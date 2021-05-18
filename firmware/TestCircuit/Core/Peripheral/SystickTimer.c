/*
 * SystickTimer.c
 *
 *  Created on: Dec 27, 2020
 *      Author: fumihiko-mbp
 */


#include "SystickTimer.h"

/* STM32 Driver */
#include "stm32f4xx_ll_utils.h"		//LL_mDelay()
#include "stm32f4xx_ll_cortex.h"	//LL_SYSTICK_EnableIT()

uint16_t __u16SamplingTimeMs = 0;
uint64_t __u64SystickTimeCount = 0;

#define SYSTICKTIMER_MS_PER_COUNT	(1)		//1Countが何msecかを定義(1ms以上)

//割り込み関数用タイマーカウント関数
void SystickTimer_Interrupt(void)
{
	__u64SystickTimeCount += SYSTICKTIMER_MS_PER_COUNT;
}

//割り込み有効
void SystickTimer_EnableInterrupt(void)
{
	#ifndef ENABLE_FREERTOS
	LL_SYSTICK_EnableIT();
	#else
	
	#endif
}

//割り込み無効
void SystickTimer_DisableInterrpt(void)
{
	//LL_SYSTICK_DisableIT();
}

//サンプリングタイムの設定
void SystickTimer_SetSamplingTime(uint16_t u16SamplingTimeMs)
{
	if(0u == u16SamplingTimeMs){
		//0指定の場合はデフォルト値を設定
		__u16SamplingTimeMs = SYSTICKTIMER_DEFAULT_SAMPLING_TIME_MS;
	}else{
		__u16SamplingTimeMs = u16SamplingTimeMs;
	}
}
//サンプリングタイムの経過判定
bool SystickTimer_IsSamplingTimeElapsed(void)
{
	//常に0以下の場合はFalseを即Return
	if(__u64SystickTimeCount <= 0u){
		return false;
	}
	static uint64_t u64LastTimeMs = 0;
	uint64_t u64CurrentTimeMs = __u64SystickTimeCount;

	if(__u16SamplingTimeMs <= u64CurrentTimeMs - u64LastTimeMs){
		u64LastTimeMs = u64CurrentTimeMs;
		return true;
	}
	return false;
}

//起動時からの時間取得 [ms]
uint64_t SystickTimer_GetTimeMS(void)
{
	return __u64SystickTimeCount;
}

//遅延 [ms]
void SystickTimer_DelayMS(uint32_t u32DelayMs)
{
	LL_mDelay(u32DelayMs);
}

//指定時間経過
bool SystickTimer_IsTimeElapsed(uint64_t u64StartTimeMs ,uint32_t u32IntervalMs)
{
	if((uint64_t)u32IntervalMs <= SystickTimer_GetTimeMS() - u64StartTimeMs){
		return true;
	}
	return false;
}
