#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// サンプリング周期設定
#define VEL_CONTROL_TASK_SAMPLING_PERIOD_MS     (1u)    //速度制御タスク
#define TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS    (5u)    //軌道追従制御タスク
#define MAIN_TASK_SAMPLING_PERIOD_MS            (10u)   // Mainタスク
#define DEBUG_TASK_SAMPLING_PERIOD_MS           (1u)    // デバッグタスク
#define WALL_DETECT_TASK_SAMPLING_PERIOD_MS     (20u)   // 壁検出タスク
#define LED_BLINK_TASK_SAMPLING_PERIOD_MS       (100u)  //LED点滅タスク

//デバッグコンソール有効化（未定義にすれば無効）
#define ENABLE_DEBUG_CONSOLE

//各タスクのデバッグ有効化（未定義にすれば無効）
#ifdef ENABLE_DEBUG_CONSOLE
#define ENABLE_VEL_CONTROL_TASK_DEBUG_CONSOLE
//#define ENABLE_TRAJ_CONTROL_TASK_DEBUG_CONSOLE
//#define ENABLE_MAIN_TASK_DEBUG_CONSOLE
//#define ENABLE_WALL_DETECT_TASK_DEBUG_CONSOLE
#endif

//モータ出力有効化
//#define ENABLE_MOTOR_OUTPUT


#ifdef __cplusplus
}
#endif

#endif