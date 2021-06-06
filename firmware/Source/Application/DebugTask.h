#ifndef DEBUG_TASK_H
#define DEBUG_TASK_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"

//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t TxQueueId;   //コンソール送信データキュー
}DebugTask_OsFunc_t;

bool DebugTask_Initialize(const DebugTask_OsFunc_t *pOsFunc);
void DebugTask_Update();

#ifdef  __cplusplus
}
#endif

#endif //DEBUG_TASK_H
