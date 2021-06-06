#ifndef DEBUG_QUEUE_H
#define DEBUG_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os.h"
#include "MessageQueueType.h"
#include <stdbool.h>
#include <stdint.h>

bool DebugQueue_Initialize(const osMessageQueueId_t *pQueueId, const osEventFlagsId_t *pEventId);
bool DebugQueue_IsInitialized();
bool DebugQueue_IsFull(void);
bool DebugQueue_IsEmpty(void);
bool DebugQueue_Printf(uint32_t uiTimeout, const char * pFormat, ...);
bool DebugQueue_Push(const char *pWriteData, uint16_t usLength, uint32_t uiTimeout);
bool DebugQueue_Pop(DebugMsg_t *pReadData, uint32_t uiTimeout);
bool DebugQueue_NotifyPushFlag(void);
bool DebugQueue_ClearPushFlag(void);
bool DebugQueue_GetPushFlag(void);
uint32_t DebugQueue_IsStoredDataNum(void);


#ifdef __cplusplus
}
#endif

#endif //DEBUG_QUEUE_H
