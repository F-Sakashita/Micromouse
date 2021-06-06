#ifndef MESSAGE_QUEUE_TYPE_H
#define MESSAGE_QUEUE_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os.h"
#include <stdint.h>
#include <stdbool.h>
#include "Calculation.h"

//デバッグメッセージ型
#define DEBUG_QUEUE_CHAR_MAX_SIZE   256
typedef struct{
    osThreadId_t taskId;
    uint32_t uiTimestamp;
    char cData[DEBUG_QUEUE_CHAR_MAX_SIZE];
    uint16_t usLength;
}DebugMsg_t;

//オドメトリメッセージ型
typedef struct{
    uint32_t uiTimestamp;
    Posture_t stData;
}OdometoryMsg_t;

//位置制御コマンドメッセージ型
typedef struct{
    uint32_t uiTimestamp;   //タイムスタンプ
    Posture_t stTargetPos;  //目標位置（フィールド座標系)
    Posture_t stTargetVel;  //目標速度（フィールド座標系）
}PosControlCmdMsg_t;

//速度制御コマンドメッセージ型
typedef struct{
    uint32_t uiTimestamp;   //タイムスタンプ
    float fStraightVelCmd;  //直進方向速度指令（V>0:直進, V<0:後進, V=0:停止)
    float fAngVelCmd;       //回転速度指令
}VelControlCmdMsg_t;



#ifdef __cplusplus
}
#endif

#endif //MESSAGE_QUEUE_TYPE_H
