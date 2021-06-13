#ifndef WALL_DETECT_TASK_H
#define WALL_DETECT_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cmsis_os.h>

//Taskで使用するOS機能
/*
typedef struct{
    osMessageQueueId_t *pQueueId;
}WallDetectTask_OsFunc_t;


bool WallDetectTask_Initialize(const WallDetectTask_OsFunc_t *pOsFunc);
void WallDetectTask_Update(void);
*/
void WallDetectTask_Enable();
void WallDetectTask_Disable();

#ifdef __cplusplus
}
#endif

#endif
