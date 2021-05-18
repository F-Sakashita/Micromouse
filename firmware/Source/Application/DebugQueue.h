#ifndef DEBUG_QUEUE_H
#define DEBUG_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_QUEUE_CHAR_MAX_SIZE   256

#include "stdint.h"
#include "cmsis_os.h"
#include "stdbool.h"

typedef struct{
    osThreadId_t taskId;
    uint32_t uiTimestamp;
    char cData[DEBUG_QUEUE_CHAR_MAX_SIZE];
    uint16_t usLength;
}DebugQueue_t;

bool DebugQueue_Initialize(osMessageQueueId_t pQueueId, const osEventFlagsId_t *pEventId);
bool DebugQueue_IsFull(void);
bool DebugQueue_IsEmpty(void);
bool DebugQueue_SetPrintf(uint8_t ucPriority, uint32_t uiTimeout, const char * pFormat, ...);
bool DebugQueue_Push(const char *pWriteData, uint16_t usLength, uint8_t ucPriority, uint32_t uiTimeout);
bool DebugQueue_Pop(DebugQueue_t *pReadData, uint8_t *pPriority, uint32_t uiTimeout);
bool DebugQueue_NotifyPushFlag(void);
bool DebugQueue_ClearPushFlag(void);
bool DebugQueue_GetPushFlag(void);
uint32_t DebugQueue_IsStoredDataNum(void);

#ifdef __cplusplus
}
#endif

#endif //DEBUG_QUEUE_H
