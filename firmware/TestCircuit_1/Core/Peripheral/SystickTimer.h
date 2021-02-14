/*
 * SystickTimer.h
 *
 *  Created on: Dec 27, 2020
 *      Author: fumihiko-mbp
 */

#ifndef PERIPHERAL_SYSTICKTIMER_H_
#define PERIPHERAL_SYSTICKTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define SYSTICKTIMER_DEFAULT_SAMPLING_TIME_MS	(1)

//割り込み関数用タイマーカウント関数
void SystickTimer_Interrupt(void);

//割り込み有効
void SystickTimer_EnableInterrupt(void);

//割り込み無効
void SystickTimer_DisableInterrpt(void);

//サンプリングタイムの設定
void SystickTimer_SetSamplingTime(uint16_t u16SamplingTimeMs);
//サンプリングタイムの経過判定
bool SystickTimer_IsSamplingTimeElapsed(void);

//起動時からの時間取得 [ms]
uint64_t SystickTimer_GetTimeMS(void);
//起動時からの時間取得 [us]
//uint64_t SystickTimer_GetTimeUS();

//遅延 [ms]
void SystickTimer_DelayMS(uint32_t u32DelayMs);
//遅延 [us]
//void SystickTimer_DelayUS(uint64_t u64DelayUs);

//指定時間経過
bool SystickTimer_IsTimeElapsed(uint64_t u64StartTimeMs ,uint32_t u32IntervalMs);


#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_SYSTICKTIMER_H_ */
