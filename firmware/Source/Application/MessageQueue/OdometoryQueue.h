#ifndef ODOMETORY_QUEUE_H
#define ODOMETORY_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Calculation.h"
#include "cmsis_os.h"
#include <stdbool.h>

typedef struct{
    uint32_t uiTimestamp;
    Posture_t stData;
}OdometoryMsg_t;

typedef enum{
    EN_ODOMETORY_QUEUE_MODE_FIRST = 0,
    EN_ODOMETORY_QUEUE_MODE_POS = 0,
    EN_ODOMETORY_QUEUE_MODE_VEL,
    EN_ODOMETORY_QUEUE_MODE_LAST,
}EN_ODOMETORY_QUEUE_MODE;

bool OdometoryQueue_Initialize(EN_ODOMETORY_QUEUE_MODE enQueueMode, osMessageQueueId_t pQueueId);
bool OdometoryQueue_IsFull(EN_ODOMETORY_QUEUE_MODE enQueueMode);
bool OdometoryQueue_IsEmpty(EN_ODOMETORY_QUEUE_MODE enQueueMode);
bool OdometoryQueue_Push(EN_ODOMETORY_QUEUE_MODE enQueueMode,  const OdometoryMsg_t *pWriteData, uint32_t uiTimeout);
bool OdometoryQueue_Pop(EN_ODOMETORY_QUEUE_MODE enQueueMode,  OdometoryMsg_t *pReadData, uint32_t uiTimeout);
uint32_t OdometoryQueue_IsStoredDataNum(EN_ODOMETORY_QUEUE_MODE enQueueMode);

#ifdef __cplusplus
}
#endif

#endif //ODOMETORY_QUEUE_H