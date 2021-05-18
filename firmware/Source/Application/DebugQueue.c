#include "DebugQueue.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "SystickTimer.h"

typedef struct{
    osMessageQueueId_t pQueueId;
    osEventFlagsId_t *pEventId;
    bool bInitialized;
}DebugQueueConfig_t;

static DebugQueueConfig_t g_stConfig;

bool DebugQueue_Initialize(osMessageQueueId_t pQueueId, const osEventFlagsId_t *pEventId)
{
    DebugQueueConfig_t *pConfig = &g_stConfig;
    if(NULL == pQueueId){
        return false;
    }
    if(NULL == pEventId){
        return false;
    }
    pConfig->pQueueId = pQueueId;
    pConfig->pEventId = pEventId;

    pConfig->bInitialized = true;

    return true;
}
bool DebugQueue_IsFull(void)
{
    DebugQueueConfig_t *pConfig = &g_stConfig;
    
    if(0u == osMessageQueueGetSpace(pConfig->pQueueId)){
        return true;
    }
    return false;
}
bool DebugQueue_IsEmpty(void)
{
    DebugQueueConfig_t *pConfig = &g_stConfig;

    if(0u == osMessageQueueGetCount(pConfig->pQueueId)){
        return true;
    }
    return false;
}

bool DebugQueue_SetPrintf(uint8_t ucPriority, uint32_t uiTimeout, const char * pFormat, ...)
{
    char cData[256];
    va_list arg;
	va_start(arg, pFormat);
    vsprintf(cData, pFormat, arg);
    va_end(arg);
    return DebugQueue_Push(cData, sizeof(cData), ucPriority, uiTimeout);
}

bool DebugQueue_Push(const char *pWriteData, uint16_t usDataLength, uint8_t ucPriority, uint32_t uiTimeout)
{
    DebugQueueConfig_t *pConfig = &g_stConfig;
    if(!pConfig->bInitialized || 0 >= usDataLength){
        return false;
    }
    DebugQueue_t TxData;
    TxData.uiTimestamp = osKernelGetTickCount();
    TxData.taskId = osThreadGetId();
    if(DEBUG_QUEUE_CHAR_MAX_SIZE <= usDataLength){
        usDataLength = DEBUG_QUEUE_CHAR_MAX_SIZE;
    }
    memcpy(TxData.cData, pWriteData, usDataLength);

    osStatus status = osMessageQueuePut(pConfig->pQueueId, &TxData, ucPriority, uiTimeout);

    if(osOK != status){
        return false;
    }

    return true;
}
bool DebugQueue_Pop(DebugQueue_t *pReadData, uint8_t *pPriority, uint32_t uiTimeout)
{
    DebugQueueConfig_t *pConfig = &g_stConfig;
    if(!pConfig->bInitialized){
        return false;
    }
    osStatus status = osMessageQueueGet(pConfig->pQueueId, pReadData, pPriority, uiTimeout);

    if(osOK != status){
        return false;
    }

    return true;
}
uint32_t DebugQueue_IsStoredDataNum(void)
{
    DebugQueueConfig_t *pConfig = &g_stConfig;
    if(!pConfig->bInitialized){
        return 0u;
    }

    return osMessageQueueGetCount(pConfig->pQueueId);
}

bool DebugQueue_NotifyPushFlag(void)
{
    return false;
}
bool DebugQueue_ClearPushFlag(void)
{
    return false;
}
bool DebugQueue_GetPushFlag(void)
{
    return false;
}
