#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// サンプリング周期設定
#define APP_TASK_SAMPLING_PERIOD_MS    (10u)    // Appタスク
#define DEBUG_TASK_SAMPLING_PERIOD_MS   (1u)    // デバッグタスク
#define WALL_DETECT_TASK_SAMPLING_PERIOD_MS     (20u)   // 壁検出タスク
#define LED_BLINK_TASK_SAMPLING_PERIOD_MS   (100u)      //LED点滅タスク

#ifdef __cplusplus
}
#endif

#endif