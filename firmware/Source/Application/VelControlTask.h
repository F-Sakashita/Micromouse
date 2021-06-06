#ifndef VEL_CONTROL_TASK_H
#define VEL_CONTROL_TASK_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include <stdbool.h>

//Taskで使用するOS機能
typedef struct{
    osMessageQueueId_t OdometoryVelQueueId;  //オドメトリキュー
}VelControlTask_OsFunc_t;

bool VelControlTask_Initialize(const VelControlTask_OsFunc_t *pOsFunc);
void VelControlTask_Update();
void VelControlTask_Enable();
void VelControlTask_Disable();


#ifdef  __cplusplus
}
#endif

#endif //VEL_CONTROL_TASK_H
