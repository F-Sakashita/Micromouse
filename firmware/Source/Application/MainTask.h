#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"

//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t MotionStartQueueId;      //動作開始イベントID
    osMessageQueueId_t PosCmdQueueId;         //位置指令キューID
}MainTask_OsFunc_t;

bool MainTask_Initialize(const MainTask_OsFunc_t *pOsFunc);
void MainTask_Update();

#ifdef  __cplusplus
}
#endif

#endif //MAIN_TASK_H
