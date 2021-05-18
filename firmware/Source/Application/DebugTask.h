#ifndef DEBUG_TASK_H
#define DEBUG_TASK_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "DebugQueue.h"

bool DebugTask_Initialize(osMessageQueueId_t pQueueId, const osEventFlagsId_t *pEventId);
void DebugTask_Update();

#ifdef  __cplusplus
}
#endif

#endif //DEBUG_TASK_H
