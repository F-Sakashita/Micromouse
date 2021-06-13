#ifndef TASK_LIST_H
#define TASK_LIST_H

/*
    タスクリスト
    ここで宣言された関数をfreertos.c内でコールする.
    定義は各タスクの.cppファイルで定義される．
    
    初期化関数
    引数：タスクで使用するOS機能のポインタ
    xxxTask_Initialize(const xxxTask_OsFunc_t *pOsFunc)

    更新関数
    xxxTask_Update()
*/

#include "cmsis_os.h"

/*************************** メインタスク **********************************/ 
//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t MotionStartQueueId;  //動作開始イベントID
    osMessageQueueId_t PosCmdQueueId;       //位置指令キューID
}MainTask_OsFunc_t;

bool MainTask_Initialize(const MainTask_OsFunc_t *pOsFunc);
void MainTask_Update();
/***********************************************************************/ 

/*************************** 軌道制御タスク **********************************/ 
//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t OdometoryPosQueueId;     //オドメトリによる位置データキュー
    osMessageQueueId_t VelCmdQueueId;           //速度指令キュー
}TrajControlTask_OsFunc_t;

bool TrajControlTask_Initialize(const TrajControlTask_OsFunc_t *pOsFunc);
void TrajControlTask_Update();
/***********************************************************************/ 

/*************************** 速度制御タスク **********************************/ 
//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t OdometoryVelQueueId;  //オドメトリによる速度データキュー
}VelControlTask_OsFunc_t;

bool VelControlTask_Initialize(const VelControlTask_OsFunc_t *pOsFunc);
void VelControlTask_Update();
/***********************************************************************/ 

/*************************** 壁検出タスク **********************************/ 
//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t *pQueueId;
}WallDetectTask_OsFunc_t;

bool WallDetectTask_Initialize(const WallDetectTask_OsFunc_t *pOsFunc);
void WallDetectTask_Update(void);
/*************************** デバッグタスク **********************************/ 

/*************************** デバッグタスク **********************************/ 
//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t TxQueueId;   //コンソール送信データキュー
}DebugTask_OsFunc_t;

bool DebugTask_Initialize(const DebugTask_OsFunc_t *pOsFunc);
void DebugTask_Update();
/***********************************************************************/ 


#endif /* TASK_LIST_H */