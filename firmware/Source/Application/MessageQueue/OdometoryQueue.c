#include "OdometoryQueue.h"

typedef struct{
    osMessageQueueId_t QueueId;
    bool bInitialized;
}OdometoryQueueConfig_t;

static OdometoryQueueConfig_t g_stConfig[EN_ODOMETORY_QUEUE_MODE_LAST];

bool OdometoryQueue_Initialize(EN_ODOMETORY_QUEUE_MODE enQueueMode, osMessageQueueId_t QueueId)
{
    if(EN_ODOMETORY_QUEUE_MODE_LAST <= enQueueMode){
        return false;
    }
    if(NULL == QueueId){
        return false;
    }

    OdometoryQueueConfig_t *pConfig = &g_stConfig[enQueueMode];
    pConfig->QueueId = QueueId;
    pConfig->bInitialized = true;

    return true;
}

bool OdometoryQueue_IsFull(EN_ODOMETORY_QUEUE_MODE enQueueMode)
{
    if(EN_ODOMETORY_QUEUE_MODE_LAST <= enQueueMode){
        return false;
    }
    OdometoryQueueConfig_t *pConfig = &g_stConfig[enQueueMode];
    if(0u == osMessageQueueGetSpace(pConfig->QueueId)){
        return true;
    }
    return false;
}
bool OdometoryQueue_IsEmpty(EN_ODOMETORY_QUEUE_MODE enQueueMode)
{
    if(EN_ODOMETORY_QUEUE_MODE_LAST <= enQueueMode){
        return false;
    }
    OdometoryQueueConfig_t *pConfig = &g_stConfig[enQueueMode];
    if(0u == osMessageQueueGetCount(pConfig->QueueId)){
        return true;
    }
    return false;
}
bool OdometoryQueue_Push(EN_ODOMETORY_QUEUE_MODE enQueueMode,  const OdometoryMsg_t *pWriteData, uint32_t uiTimeout)
{
    if(EN_ODOMETORY_QUEUE_MODE_LAST <= enQueueMode){
        return false;
    }
    OdometoryQueueConfig_t *pConfig = &g_stConfig[enQueueMode];
    if(!pConfig->bInitialized || NULL == pWriteData){
        return false;
    }
    
    osStatus status = osMessageQueuePut(pConfig->QueueId, pWriteData, uiTimeout);

    if(osOK != status){
        return false;
    }

    return true;
}
bool OdometoryQueue_Pop(EN_ODOMETORY_QUEUE_MODE enQueueMode,  OdometoryMsg_t *pReadData, uint32_t uiTimeout)
{
    if(EN_ODOMETORY_QUEUE_MODE_LAST <= enQueueMode){
        return false;
    }
    OdometoryQueueConfig_t *pConfig = &g_stConfig[enQueueMode];
    if(!pConfig->bInitialized || NULL == pReadData){
        return false;
    }
    osStatus status = osMessageQueueGet(pConfig->QueueId, pReadData, uiTimeout);

    if(osOK != status){
        return false;
    }

    return true;

}
uint32_t OdometoryQueue_IsStoredDataNum(EN_ODOMETORY_QUEUE_MODE enQueueMode)
{
    if(EN_ODOMETORY_QUEUE_MODE_LAST <= enQueueMode){
        return 0u;
    }
    OdometoryQueueConfig_t *pConfig = &g_stConfig[enQueueMode];
    
    return osMessageQueueGetCount(pConfig->QueueId);
}
