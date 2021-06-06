#ifndef TRAJ_CONTROL_TASK_H
#define TRAJ_CONTROL_TASK_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include <stdbool.h>

//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t OdometoryPosQueueId;     //オドメトリキュー
    osMessageQueueId_t VelCmdQueueId;           //速度指令キュー
}TrajControlTask_OsFunc_t;

bool TrajControlTask_Initialize(const TrajControlTask_OsFunc_t *pOsFunc);
void TrajControlTask_Update();
void TrajControlTask_Enable();
void TrajControlTask_Disable();


#ifdef  __cplusplus
}
#endif


#endif //TRAJ_CONTROL_TASK_H